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

#ifndef INCLUDED_TUJASDR_AGC_CC_IMPL_H
#define INCLUDED_TUJASDR_AGC_CC_IMPL_H

#include <tujasdr/agc_cc.h>

namespace gr {
    namespace tujasdr {
        
        float fast_magnitude(const gr_complex& c);
        float magnitude_squared(const gr_complex& c);
        
        class agc_cc_impl : public agc_cc
        {
        private:
            typedef float (*power_function_t)(const gr_complex&);
            
            power_function_t d_power_function;
            float d_attack_rate;
            float d_decay_rate;
            float d_reference;
            float d_gain;
            float d_max_gain;
            
        public:
            agc_cc_impl(float attack_rate, float decay_rate, float reference, float gain, float max_gain);
            ~agc_cc_impl();
            
            // Where all the action really happens
            int work(int noutput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items);
        };
        
    } // namespace tujasdr
} // namespace gr

#endif /* INCLUDED_TUJASDR_AGC_CC_IMPL_H */
