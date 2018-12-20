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

#ifndef INCLUDED_TUJASDR_ALSA_SINK_IMPL_H
#define INCLUDED_TUJASDR_ALSA_SINK_IMPL_H

#include <tujasdr/alsa_sink.h>

#include "alsa.h"

// TODO: increase compile warning level

namespace gr {
    namespace tujasdr {
        
        class alsa_sink_impl : public alsa_sink
        {
        private:
            snd_pcm_t* d_pcm_handle;
            const unsigned int d_periods;
            const unsigned int d_frames_per_period;
            const unsigned int d_channels;
            const unsigned int d_sample_rate;
            const unsigned int d_max_periods_work;
            // TODO: use volk malloc instead??
            sample32_t *d_buf;
            //std::vector<int32_t> d_buf;
            
        public:
            alsa_sink_impl(unsigned int sample_rate,
                           const std::string device_name,
                           unsigned int frames_per_period);
            ~alsa_sink_impl();

            // start stop
            bool start();
            bool stop();
            
            // Where all the action really happens
            int work(int noutput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items);
        };
        
    } // namespace tujasdr
} // namespace gr

#endif /* INCLUDED_TUJASDR_ALSA_SINK_IMPL_H */
