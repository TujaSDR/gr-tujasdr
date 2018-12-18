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
#include <cmath>
#include "single_pole_low_pass_ff_impl.h"

namespace gr {
    namespace tujasdr {
        
        single_pole_low_pass_ff::sptr
        single_pole_low_pass_ff::make(float sample_rate, float tau)
        {
            return gnuradio::get_initial_sptr
            (new single_pole_low_pass_ff_impl(sample_rate, tau));
        }
        
        /*
         * The private constructor
         */
        single_pole_low_pass_ff_impl::single_pole_low_pass_ff_impl(float sample_rate, float tau)
        :d_sample_rate(sample_rate),
        d_tau(tau),
        gr::sync_block("single_pole_low_pass_ff",
                       gr::io_signature::make(1, 1, sizeof(float)),
                       gr::io_signature::make(1, 1, sizeof(float)))
        {
            float x = std::exp(-1./(d_sample_rate * d_tau));
            d_a0 = 1 - x;
            d_b1 = x;
            d_z1 = 0;
        }
        
        /*
         * Our virtual destructor.
         */
        single_pole_low_pass_ff_impl::~single_pole_low_pass_ff_impl()
        {
        }
        
        int
        single_pole_low_pass_ff_impl::work(int noutput_items,
                                           gr_vector_const_void_star &input_items,
                                           gr_vector_void_star &output_items)
        {
            const float *in = (const float *) input_items[0];
            float *out = (float *) output_items[0];
            
            // Filter
            for (int n = 0; n < noutput_items; n++) {
                out[n] = d_a0 * in[n] + d_b1 * d_z1;
                d_z1 = out[n];
            }
            
            // Tell runtime system how many output items we produced.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
