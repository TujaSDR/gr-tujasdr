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
#include "add_real_imag_cc_impl.h"

namespace gr {
    namespace tujasdr {
        
        add_real_imag_cc::sptr
        add_real_imag_cc::make()
        {
            return gnuradio::get_initial_sptr(new add_real_imag_cc_impl());
        }
        
        /*
         * The private constructor
         */
        add_real_imag_cc_impl::add_real_imag_cc_impl()
        : gr::sync_block("add_real_imag_cc",
                         gr::io_signature::make(1, 1, sizeof(gr_complex)),
                         gr::io_signature::make(1, 1, sizeof(gr_complex)))
        { }
        
        /*
         * Our virtual destructor.
         */
        add_real_imag_cc_impl::~add_real_imag_cc_impl()
        {
        }
        
        int
        add_real_imag_cc_impl::work(int noutput_items,
                                    gr_vector_const_void_star &input_items,
                                    gr_vector_void_star &output_items)
        {
            const gr_complex *in = (const gr_complex *) input_items[0];
            gr_complex *out = (gr_complex *) output_items[0];
            
            // TODO: maybe write a volk kernel
            // Add real + imag and put in real
            // Substract real - imag and put in imag
            // This is useful for SSB demodulation
            for (int n = 0; n < noutput_items; n++) {
                out[n] = gr_complex(in[n].real() + in[n].imag(), 0);
            }
            // Tell runtime system how many output items we produced.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
