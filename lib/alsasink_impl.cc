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
#include "alsasink_impl.h"

namespace gr {
    namespace tujasdr {
        
        alsasink::sptr
        alsasink::make(int sampling_rate, const std::string device_name)
        {
            return gnuradio::get_initial_sptr
            (new alsasink_impl(sampling_rate, device_name));
        }
        
        /*
         * The private constructor
         */
        alsasink_impl::alsasink_impl(int sampling_rate, const std::string device_name)
        : d_pcm_handle(nullptr),
        d_periods(4),          // reasonable defaults
        d_period_frames(2048), // seems to be good defaults
        d_sample_rate(sampling_rate),
        gr::sync_block("alsasink",
                       gr::io_signature::make(1, 2, sizeof(float)), // input
                       gr::io_signature::make(0, 0, 0)) // output
        {
            d_pcm_handle = alsa_pcm_handle(device_name.c_str(),
                                           d_sample_rate,
                                           d_periods,
                                           d_period_frames,
                                           SND_PCM_STREAM_PLAYBACK);
            
            if (d_pcm_handle == nullptr)
                throw std::runtime_error("alsa_pcm_handle");
            
            // 2 channels
            d_fbuf.resize(d_period_frames * 2);
            d_ibuf.resize(d_period_frames * 2);
            
            // this is helpful for throughput
            set_output_multiple(d_period_frames);
        }
        
        /*
         * Our virtual destructor.
         */
        alsasink_impl::~alsasink_impl()
        {
            snd_pcm_close(d_pcm_handle);
        }
        
        bool
        alsasink_impl::check_topology (int ninputs, int noutputs)
        {
            // TODO: perhaps apply some optimization here.
            
            return true;
        }
        
        bool
        alsasink_impl::start()
        {
            // TODO: maybe check state here?
            printf("start\n");
            snd_pcm_prepare(d_pcm_handle);
            return true;
        }
        bool
        alsasink_impl::stop()
        {
            // Stop immediately and drop buffer contents
            printf("stop\n");
            snd_pcm_drop(d_pcm_handle);
            return true;
        }
        
        int
        alsasink_impl::work(int noutput_items,
                            gr_vector_const_void_star &input_items,
                            gr_vector_void_star &output_items)
        {
            const float scaling_factor = 4294967294.; // S32 = 2^(32-1)-1
            
            // TODO: create separate work function for 1 and 2 channels
            snd_pcm_sframes_t n_err;
            // printf("alsasink_impl::work: %d\n", noutput_items);
            
            // TODO: tune this for best performance
            if (noutput_items > d_period_frames) {
                noutput_items = d_period_frames;
            }

            // Two channels
            const float *in_0 = (const float *) input_items[0];
            const float *in_1 = (const float *) input_items[1];
            
            // void volk_32f_x2_interleave_32fc(lv_32fc_t* complexVector, const float* iBuffer, const float* qBuffer, unsigned int num_points)
            volk_32f_x2_interleave_32fc((lv_32fc_t*)d_fbuf.data(), in_0, in_1, noutput_items);

            // void volk_32f_s32f_convert_32i(int32_t* outputVector, const float* inputVector, const float scalar, unsigned int num_points)
            // x2 because cast to float but they are complex now
            volk_32f_s32f_convert_32i(d_ibuf.data(), d_fbuf.data(), scaling_factor, 2 * noutput_items);
            
            // Write to ALSA
            n_err = snd_pcm_writei(d_pcm_handle, d_ibuf.data(), noutput_items);
            if (n_err < 0) {
                n_err = snd_pcm_recover(d_pcm_handle, (int )n_err, 0);
                printf("recover!\n");
                if (n_err < 0) {
                    throw std::runtime_error(snd_strerror((int) n_err));
                }
            }
            
            // Tell runtime system how many output items we consumed?.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
