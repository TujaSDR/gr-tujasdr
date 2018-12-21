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
#include "tujavolk.h"
#include "mono_sink_impl.h"

namespace gr {
    namespace tujasdr {
        
        mono_sink::sptr
        mono_sink::make(unsigned int sample_rate, const std::string device_name)
        {
            return gnuradio::get_initial_sptr(new mono_sink_impl(sample_rate, device_name));
        }
        
        /*
         * The private constructor
         */
        mono_sink_impl::mono_sink_impl(unsigned int sample_rate, const std::string device_name)
        :d_pcm_handle(NULL),
        d_periods(5),          // reasonable defaults
        d_frames_per_period(960), // seems to be good defaults
        d_channels(1),
        d_sample_rate(sample_rate),
        d_max_periods_work(1),
        d_buf(NULL),
        gr::sync_block("mono_sink",
        gr::io_signature::make(1, 1, sizeof(float)),
                         gr::io_signature::make(0, 0, 0))
        {
            d_pcm_handle = alsa_pcm_handle(device_name.c_str(),
                                           d_channels,
                                           d_sample_rate,
                                           d_periods,
                                           d_frames_per_period,
                                           SND_PCM_FORMAT_S16,
                                           SND_PCM_STREAM_PLAYBACK);
            
            if (d_pcm_handle == NULL) {
                throw std::runtime_error("alsa_pcm_handle");
            }
            
            // Handle at most 2 periods at a time
            //d_buf.resize(d_frames_per_period * d_max_periods_work);
            size_t alignment = volk_get_alignment();
            d_buf = (int16_t*)volk_malloc(sizeof(int16_t) * d_frames_per_period * d_max_periods_work, alignment);
            assert(d_buf != NULL);
            
            // This is helpful for throughput
            set_output_multiple(d_frames_per_period);
        }
        
        /*
         * Our virtual destructor.
         */
        mono_sink_impl::~mono_sink_impl()
        {
            snd_pcm_close(d_pcm_handle);
            volk_free(d_buf);
        }
        
        bool
        mono_sink_impl::start()
        {
            // TODO: maybe check state here?
            snd_pcm_prepare(d_pcm_handle);
            return true;
        }
        
        bool
        mono_sink_impl::stop()
        {
            // Stop immediately and drop buffer contents
            snd_pcm_drop(d_pcm_handle);
            return true;
        }
        
        int
        mono_sink_impl::work(int noutput_items,
                             gr_vector_const_void_star &input_items,
                             gr_vector_void_star &output_items)
        {
            const float *in = (const float *) input_items[0];
            
            const float scaling_factor = 32767.; // S16 = 2^(16-1)-1
            snd_pcm_sframes_t n_err;
            // printf("alsa_sink_impl::work: %d\n", noutput_items);
            
            // TODO: tune this for best performance
            // Right now we are doing no more than 2 periods / work.
            if (noutput_items > d_frames_per_period * d_max_periods_work) {
                noutput_items = d_frames_per_period * d_max_periods_work;
            }

            volk_32f_s32f_convert_16i_neon((int16_t*)d_buf, in, scaling_factor, noutput_items);
            
            // Write to ALSA
            n_err = snd_pcm_writei(d_pcm_handle, d_buf, noutput_items);
            if (n_err < 0) {
                printf("recover!\n");
                n_err = snd_pcm_recover(d_pcm_handle, (int )n_err, 0);
                if (n_err < 0) {
                    // if we could not recover throw an error
                    throw std::runtime_error(snd_strerror((int) n_err));
                }
            }
            
            // Tell runtime system how many output items we produced.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
