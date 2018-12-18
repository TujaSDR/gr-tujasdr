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

#ifndef INCLUDED_TUJASDR_FAST_SINE_SOURCE_C_IMPL_H
#define INCLUDED_TUJASDR_FAST_SINE_SOURCE_C_IMPL_H

#include <tujasdr/fast_sine_source_c.h>

namespace gr {
    namespace tujasdr {
        
        class fast_sine_source_c_impl : public fast_sine_source_c
        {

        private:
            // https://www.vicanek.de/articles/QuadOsc.pdf
            float d_sample_rate;
            float d_frequency;
            float d_amplitude;
            float d_v;
            float d_u;
            float d_k1;
            float d_k2;
            
        public:
            fast_sine_source_c_impl(float sample_rate, float frequency, float amplitude);
            ~fast_sine_source_c_impl();
            
            // Where all the action really happens
            int work(int noutput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items);
        };
        
    } // namespace tujasdr
} // namespace gr

#endif /* INCLUDED_TUJASDR_FAST_SINE_SOURCE_C_IMPL_H */

