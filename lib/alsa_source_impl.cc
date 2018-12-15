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
        alsa_source::make(unsigned int sample_rate, const std::string& device_name)
        {
            return gnuradio::get_initial_sptr(new alsa_source_impl(sample_rate, device_name));
        }
        
        /*
         * The private constructor
         */
        alsa_source_impl::alsa_source_impl(unsigned int sample_rate, const std::string& device_name)
        : d_pcm_handle(nullptr),
        d_periods(4),
        d_period_frames(2048),
        d_channels(2),
        d_sample_rate(sample_rate),
        d_max_periods_work(2),
        gr::sync_block("alsa_source",
        gr::io_signature::make(0, 0, 0),
                       gr::io_signature::make(1, 1, sizeof(gr_complex)))
        {
            d_pcm_handle = alsa_pcm_handle(device_name.c_str(),
                                           d_channels,
                                           d_sample_rate,
                                           d_periods,
                                           d_period_frames,
                                           SND_PCM_FORMAT_S32,
                                           SND_PCM_STREAM_CAPTURE);
            
            if (d_pcm_handle == nullptr)
                throw std::runtime_error("alsa_pcm_handle");
            
            // x2 because of complex input
            d_buf.resize(d_period_frames * d_max_periods_work * 2);
            
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
        alsa_source_impl::start()
        {
            // snd_pcm_prepare(d_pcm_handle);
            snd_pcm_start(d_pcm_handle);
            return true;
        }
        bool
        alsa_source_impl::stop()
        {
            snd_pcm_drop(d_pcm_handle);
            return true;
        }
        
        int
        alsa_source_impl::work(int noutput_items,
                               gr_vector_const_void_star &input_items,
                               gr_vector_void_star &output_items)
        {
            gr_complex *out = (gr_complex *) output_items[0];
            
            //const float scaling_factor = 4294967294.; // S32 = 2^(32-1)-1
            const float scaling_factor = 2147483647.;
            snd_pcm_sframes_t n_err;
            
            //printf("read\n");
            
            // TODO: tune this for best performance
            // For now only read a period
            if (noutput_items > d_period_frames * d_max_periods_work) {
                noutput_items = d_period_frames * d_max_periods_work;
            }
            
            // Read from ALSA
            n_err = snd_pcm_readi(d_pcm_handle, d_buf.data(), noutput_items);
            if (n_err < 0) {
                // If there was an error try to recover
                n_err = snd_pcm_recover(d_pcm_handle, (int )n_err, 0);
                printf("recover!\n");
                if (n_err < 0) {
                    // if we could not recover throw an error
                    throw std::runtime_error(snd_strerror((int) n_err));
                }
            }
            
            // Ok we have read the data, convert it and write it to output buffer
            // Again, 2*noutput_items because there are two samples / frame.
            volk_32i_s32f_convert_32f((float*) out, d_buf.data(), scaling_factor, 2 * noutput_items);
            
            // Tell runtime system how many output items we produced.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
