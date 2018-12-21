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
#include <complex>
#include <volk/volk.h>
#include "tujavolk.h"
#include "alsa_source_impl.h"

namespace gr {
    namespace tujasdr {
        
        alsa_source::sptr
        alsa_source::make(unsigned int sample_rate,
                          const std::string& device_name,
                          unsigned int frames_per_period)
        {
            return gnuradio::get_initial_sptr(new alsa_source_impl(sample_rate, device_name, frames_per_period));
        }
        
        /*
         * The private constructor
         */
        alsa_source_impl::alsa_source_impl(unsigned int sample_rate,
                                           const std::string& device_name,
                                           unsigned int frames_per_period)
        : d_pcm_handle(NULL),
        d_periods(2),
        d_frames_per_period(frames_per_period),
        d_channels(2),
        d_sample_rate(sample_rate),
        d_max_periods_work(1),
        d_mode(COMPLEX),
        d_buf(NULL),
        d_lowpass(sample_rate, 0.002), // tau
        d_sine_source(sample_rate, 800.0), // freq.
        gr::sync_block("alsa_source",
                       gr::io_signature::make(0, 0, 0),
                       gr::io_signature::make(1, 1, sizeof(gr_complex)))
        {
            // Open ALSA device
            d_pcm_handle = alsa_pcm_handle(device_name.c_str(),
                                           d_channels,
                                           d_sample_rate,
                                           d_periods,
                                           d_frames_per_period,
                                           SND_PCM_FORMAT_S32,
                                           SND_PCM_STREAM_CAPTURE);
            
            if (d_pcm_handle == NULL) {
                throw std::runtime_error("alsa_pcm_handle");
            }
            
            size_t alignment = volk_get_alignment();
            d_buf = (sample32_t*)volk_malloc(sizeof(sample32_t) * d_frames_per_period * d_max_periods_work, alignment);
            assert(d_buf != NULL);
            
            set_output_multiple(d_frames_per_period);
        }
        
        /*
         * Our virtual destructor.
         */
        alsa_source_impl::~alsa_source_impl()
        {
            snd_pcm_close(d_pcm_handle);
            volk_free(d_buf);
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
        
        unsigned int
        alsa_source_impl::frames_per_period() const {
            return d_frames_per_period;
        }
        
        void
        alsa_source_impl::set_frames_per_period(unsigned int frames_per_period) {
            d_frames_per_period = frames_per_period;
            // TODO update ALSA!!
            // throw on error!!
        }
        
        void
        alsa_source_impl::set_mode(alsa_source_mode_t mode) {
            d_mode = mode;
        }
        
        alsa_source_mode_t
        alsa_source_impl::mode() const {
            return d_mode;
        }
        
        int
        alsa_source_impl::work(int noutput_items,
                               gr_vector_const_void_star &input_items,
                               gr_vector_void_star &output_items)
        {
            gr_complex *out = (gr_complex *) output_items[0];
            
            //const float scaling_factor = 4294967294.; // S32 = 2^(32-1)-1
            const float scaling_factor = 2147483647.;
            const float recip_scaling_factor = 1. / scaling_factor;
            
            snd_pcm_sframes_t n_err;
            
            //printf("read\n");
            
            // TODO: tune this for best performance
            // For now only read a period
            if (noutput_items > d_frames_per_period * d_max_periods_work) {
                noutput_items = d_frames_per_period * d_max_periods_work;
            }
            
            // Read from ALSA
            n_err = snd_pcm_readi(d_pcm_handle, (int32_t*)d_buf, noutput_items);
            if (n_err < 0) {
                // If there was an error try to recover
                n_err = snd_pcm_recover(d_pcm_handle, (int )n_err, 0);
                printf("recover!\n");
                if (n_err < 0) {
                    // if we could not recover throw an error
                    throw std::runtime_error(snd_strerror((int) n_err));
                }
            }
            
            // Just use volk x2 because this function deals with float not complex.
            /*volk_32i_s32f_convert_32f((float*) out,
             (int32_t*)d_buf,
             scaling_factor, 2 * noutput_items);*/
            
            switch (d_mode) {
                case COMPLEX:
                {
                    volk_32i_s32f_convert_32f_neon((float*) out,
                                                   (int32_t*)d_buf,
                                                   scaling_factor, 2 * noutput_items);
                }
                    break;
                case REAL_KEY:
                {
                    // TODO: Write NEON function for this
                    for (int n = 0; n < noutput_items; n++) {
                        out[n] = gr_complex((float)d_buf[n].l * recip_scaling_factor, (float)(d_buf[n].l & 1));
                    }
                }
                    break;
                default:
                    break;
            }
            
            /*
             break;
             case KEY_ENVELOPE_IMAG:
             {
             // TODO: volk neon optimize
             for (int n = 0; n<noutput_items; n++) {
             out[n] = gr_complex(d_buf[n].l * one_over_scaling_factor, (d_buf[n].l & 1));
             }
             }
             break;
             case KEY_ENVELOPE_FILTERED_IMAG:
             {
             // TODO: volk neon optimize
             for (int n = 0; n<noutput_items; n++) {
             out[n] = gr_complex(d_buf[n].l * one_over_scaling_factor,
             d_lowpass.filter(d_buf[n].l & 1));
             }
             }
             break;
             default:
             break;*/
            // }
            
            // Tell runtime system how many output items we produced.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
