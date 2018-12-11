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
#include "complex_sum_impl.h"

namespace gr {
    namespace tujasdr {
        
        complex_sum::sptr
        complex_sum::make(int sign)
        {
            return gnuradio::get_initial_sptr(new complex_sum_impl(sign));
        }
        
        /*
         * The private constructor
         */
        complex_sum_impl::complex_sum_impl(int sign)
        : d_sign(sign),
        gr::sync_block("complex_sum",
                       gr::io_signature::make(1, 1, sizeof(gr_complex)),
                       gr::io_signature::make(1, 1, sizeof(gr_complex)))
        {
            // TODO
            if (abs(d_sign) != 1)
                throw std::runtime_error("sign has to be 1 or -1");
        }
        
        /*
         * Our virtual destructor.
         */
        complex_sum_impl::~complex_sum_impl()
        {
        }
        
        int
        complex_sum_impl::work(int noutput_items,
                               gr_vector_const_void_star &input_items,
                               gr_vector_void_star &output_items)
        {
            const gr_complex *in = (const gr_complex *) input_items[0];
            gr_complex *out = (gr_complex *) output_items[0];
            
            // TODO: maybe write a volk kernel
            // Add or substractc real + imag and put in real which is also the left channel
            if (d_sign > 0) {
                for (int n; n < noutput_items; n++) {
                    out[n] = in[n].real() + in[n].imag();
                }
            } else {
                for (int n; n < noutput_items; n++) {
                    out[n] = in[n].real() - in[n].imag();
                }
            }
            
            // Tell runtime system how many output items we produced.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
