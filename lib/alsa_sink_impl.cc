/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <stdexcept>
#include <volk/volk.h>
#include "alsa_sink_impl.h"

namespace gr {
    namespace tujasdr {
        
        alsa_sink::sptr
        alsa_sink::make(unsigned int sample_rate, const std::string device_name)
        {
            return gnuradio::get_initial_sptr
            (new alsa_sink_impl(sample_rate, device_name));
        }
        
        /*
         * The private constructor
         */
        alsa_sink_impl::alsa_sink_impl(unsigned int sample_rate, const std::string device_name)
        : d_pcm_handle(NULL),
        d_periods(4),          // reasonable defaults
        d_period_frames(2048), // seems to be good defaults
        d_channels(2),
        d_sample_rate(sample_rate),
        d_max_periods_work(2),
        gr::sync_block("alsa_sink",
                       gr::io_signature::make(1, 1, sizeof(gr_complex)), // input
                       gr::io_signature::make(0, 0, 0)) // output
        {
            d_pcm_handle = alsa_pcm_handle(device_name.c_str(),
                                           d_channels,
                                           d_sample_rate,
                                           d_periods,
                                           d_period_frames,
                                           SND_PCM_FORMAT_S32,
                                           SND_PCM_STREAM_PLAYBACK);
            
            if (d_pcm_handle == NULL) {
                throw std::runtime_error("alsa_pcm_handle");
            }
            
            // Complex data 2 samples per frame
            d_buf.resize(d_period_frames * 2 * 2);
            
            // this is helpful for throughput
            set_output_multiple(d_period_frames);
        }
        
        /*
         * Our virtual destructor.
         */
        alsa_sink_impl::~alsa_sink_impl()
        {
            snd_pcm_close(d_pcm_handle);
        }
        
        bool
        alsa_sink_impl::start()
        {
            // TODO: maybe check state here?
            printf("start\n");
            snd_pcm_prepare(d_pcm_handle);
            return true;
        }
        
        bool
        alsa_sink_impl::stop()
        {
            // Stop immediately and drop buffer contents
            printf("stop\n");
            snd_pcm_drop(d_pcm_handle);
            return true;
        }
        
        int
        alsa_sink_impl::work(int noutput_items,
                            gr_vector_const_void_star &input_items,
                            gr_vector_void_star &output_items)
        {
            // One complex input
            const gr_complex *in = (const gr_complex *) input_items[0];
            
            const float scaling_factor = 2147483647.; // S32 = 2^(32-1)-1
            snd_pcm_sframes_t n_err;
            // printf("alsa_sink_impl::work: %d\n", noutput_items);
            
            // TODO: tune this for best performance
            if (noutput_items > d_period_frames * d_max_periods_work) {
                noutput_items = d_period_frames * d_max_periods_work;
            }

            // use volk to convert from float to int32s
            // x2 because this function works on floats
            volk_32f_s32f_convert_32i(d_buf.data(), (const float*)in, scaling_factor, 2 * noutput_items);
            
            // Write to ALSA
            n_err = snd_pcm_writei(d_pcm_handle, d_buf.data(), noutput_items);
            if (n_err < 0) {
                n_err = snd_pcm_recover(d_pcm_handle, (int )n_err, 0);
                printf("recover!\n");
                if (n_err < 0) {
                    // if we could not recover throw an error
                    throw std::runtime_error(snd_strerror((int) n_err));
                }
            }
            
            // Tell runtime system how many output items we consumed?.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
