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

#ifndef INCLUDED_TUJASDR_ALSASINK_IMPL_H
#define INCLUDED_TUJASDR_ALSASINK_IMPL_H

#include <tujasdr/alsasink.h>

#include "alsa.h"

// TODO: increase compile warning level

namespace gr {
    namespace tujasdr {
        
        class alsasink_impl : public alsasink
        {
        private:
            // typedef for pointer to class work method
            typedef int(alsasink::*work_t)(int noutput_items,
                                            gr_vector_const_void_star &input_items,
                                            gr_vector_void_star &output_items);
            
            snd_pcm_t* d_pcm_handle;
            const unsigned int d_periods;
            const unsigned int d_period_frames;
            const int d_sample_rate;
            std::vector<int32_t> d_ibuf;
            std::vector<float> d_fbuf;
            
        public:
            alsasink_impl(int sampling_rate, const std::string device_name);
            ~alsasink_impl();
            
            bool check_topology (int ninputs, int noutputs);
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

#endif /* INCLUDED_TUJASDR_ALSASINK_IMPL_H */
