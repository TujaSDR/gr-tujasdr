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
#include "alsa_source_impl.h"

namespace gr {
    namespace tujasdr {
        
        alsa_source::sptr
        alsa_source::make(int sampling_rate, const std::string device_name)
        {
            return gnuradio::get_initial_sptr
            (new alsa_source_impl(sampling_rate, device_name));
        }
        
        /*
         * The private constructor
         */
        alsa_source_impl::alsa_source_impl(int sampling_rate, const std::string device_name)
        : d_pcm_handle(nullptr),
        d_periods(4),
        d_period_frames(2048),
        d_sample_rate(sampling_rate),
        gr::sync_block("alsa_source",
        gr::io_signature::make(0, 0, 0),
                       gr::io_signature::make(2, 2, sizeof(float)))
        {
            d_pcm_handle = alsa_pcm_handle(device_name.c_str(),
                                           d_sample_rate,
                                           d_periods,
                                           d_period_frames,
                                           SND_PCM_STREAM_CAPTURE);
            
            if (d_pcm_handle == nullptr)
                throw std::runtime_error("alsa_pcm_handle");
            
            d_ibuf.resize(d_period_frames * 2 * 4);
            d_fbuf.resize(d_period_frames * 2 * 4);
            
            // not sure about this
            set_output_multiple(d_period_frames);
        }
        
        /*
         * Our virtual destructor.
         */
        alsa_source_impl::~alsa_source_impl()
        {
            snd_pcm_close(d_pcm_handle);
        }
        bool
        alsa_source_impl::check_topology (int ninputs, int noutputs) {
            
            return true;
        }
        
        bool
        alsa_source_impl::start()
        {
            return true;
        }
        bool
        alsa_source_impl::stop()
        {
            return true;
        }
        
        int
        alsa_source_impl::work(int noutput_items,
                               gr_vector_const_void_star &input_items,
                               gr_vector_void_star &output_items)
        {
            float *out = (float *) output_items[0];
            
            // Do <+signal processing+>
            
            // Tell runtime system how many output items we produced.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */

