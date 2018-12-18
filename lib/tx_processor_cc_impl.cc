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
#include "tx_processor_cc_impl.h"

#include <tujasdr/fast_sine_source_f.h>
#include <tujasdr/single_pole_low_pass_ff.h>
#include <gnuradio/blocks/complex_to_float.h>
#include <gnuradio/blocks/float_to_complex.h>
#include <gnuradio/blocks/multiply_ff.h>
#include <gnuradio/blocks/add_ff.h>

namespace gr {
    namespace tujasdr {
        
        tx_processor_cc::sptr
        tx_processor_cc::make(float sample_rate)
        {
            return gnuradio::get_initial_sptr
            (new tx_processor_cc_impl(sample_rate));
        }
        
        /*
         * The private constructor
         */
        tx_processor_cc_impl::tx_processor_cc_impl(float sample_rate)
        : d_sample_rate(sample_rate),
        gr::hier_block2("tx_processor_cc",
                        gr::io_signature::make(1, 1, sizeof(gr_complex)),
                        gr::io_signature::make(1, 1, sizeof(gr_complex)))
        {
            auto c_to_f = gr::blocks::complex_to_float::make();
            auto f_to_c = gr::blocks::float_to_complex::make();
            auto fast_sine_source_f = gr::tujasdr::fast_sine_source_f::make(d_sample_rate, 800., 0.7);
            auto add_ff = gr::blocks::add_ff::make();
            auto mult_ff = gr::blocks::multiply_ff::make();
            
            auto key_filter = gr::tujasdr::single_pole_low_pass_ff::make(d_sample_rate, 0.005);
            
            // Split complex to float
            connect(self(), 0, c_to_f, 0);
            
            // Multiply key with sine source
            connect(fast_sine_source_f, 0, mult_ff, 0);
            connect(c_to_f, 1, key_filter, 0);
            connect(key_filter, 0, mult_ff, 1);
            
            // Mix (add) mic input and key
            connect(c_to_f, 0, add_ff, 0);
            connect(mult_ff, 0, add_ff, 1);
            // Put results in real part
            connect(add_ff, 0, f_to_c, 0);
            
            connect(f_to_c, 0, self(), 0);
        }
        
        /*
         * Our virtual destructor.
         */
        tx_processor_cc_impl::~tx_processor_cc_impl()
        {
        }
        
        
    } /* namespace tujasdr */
} /* namespace gr */

