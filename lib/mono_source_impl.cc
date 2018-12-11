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
#include "mono_source_impl.h"

namespace gr {
    namespace tujasdr {
        
        mono_source::sptr
        mono_source::make(int sampling_rate, const std::string device_name)
        {
            return gnuradio::get_initial_sptr
            (new mono_source_impl(sampling_rate, device_name));
        }
        
        /*
         * The private constructor
         */
        mono_source_impl::mono_source_impl(int sampling_rate, const std::string device_name)
        :d_pcm_handle(nullptr),
        d_periods(5),         // reasonable defaults
        d_period_frames(960), // seems to be good defaults
        d_channels(1),
        d_sample_rate(sampling_rate),
        d_max_periods_work(2),
        gr::sync_block("mono_source",
        gr::io_signature::make(0, 0, 0),
                         gr::io_signature::make(1, 1, sizeof(float)))
        {
            d_pcm_handle = alsa_pcm_handle(device_name.c_str(),
                                           d_channels,
                                           d_sample_rate,
                                           d_periods,
                                           d_period_frames,
                                           SND_PCM_FORMAT_S16,
                                           SND_PCM_STREAM_CAPTURE);
            
            if (d_pcm_handle == nullptr)
                throw std::runtime_error("alsa_pcm_handle");
            
            // Handle at most 2 periods at a time
            d_buf.resize(d_period_frames * d_max_periods_work);
            // This is helpful for throughput
            set_output_multiple(d_period_frames);
        }
        
        /*
         * Our virtual destructor.
         */
        mono_source_impl::~mono_source_impl()
        {
        }
        
        bool
        mono_source_impl::start()
        {
            // TODO: maybe check state here?
            snd_pcm_prepare(d_pcm_handle);
            return true;
        }
        
        bool
        mono_source_impl::stop()
        {
            // Stop immediately and drop buffer contents
            snd_pcm_drop(d_pcm_handle);
            return true;
        }
        
        int
        mono_source_impl::work(int noutput_items,
                               gr_vector_const_void_star &input_items,
                               gr_vector_void_star &output_items)
        {
            float *out = (float *) output_items[0];
            
            const float scaling_factor = 32767.; // S16 = 2^(16-1)-1
            snd_pcm_sframes_t n_err;
            // printf("alsa_sink_impl::work: %d\n", noutput_items);
            
            // TODO: tune this for best performance
            // Right now we are doing no more than 2 periods / work.
            if (noutput_items > d_period_frames * d_max_periods_work) {
                noutput_items = d_period_frames * d_max_periods_work;
            }
            
            // Read from ALSA
            n_err = snd_pcm_readi(d_pcm_handle, d_buf.data(), noutput_items);
            if (n_err < 0) {
                n_err = snd_pcm_recover(d_pcm_handle, (int )n_err, 0);
                printf("recover!\n");
                if (n_err < 0) {
                    // if we could not recover throw an error
                    throw std::runtime_error(snd_strerror((int) n_err));
                }
            }
            
            // use volk to convert from float to int16s
            volk_16i_s32f_convert_32f(out, d_buf.data(), scaling_factor, noutput_items);
            
            // Tell runtime system how many output items we produced.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
