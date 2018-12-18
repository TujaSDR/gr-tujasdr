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
#include "fast_sine_source_c_impl.h"

namespace gr {
    namespace tujasdr {
        
        fast_sine_source_c::sptr
        fast_sine_source_c::make(float sample_rate, float frequency, float amplitude)
        {
            return gnuradio::get_initial_sptr
            (new fast_sine_source_c_impl(sample_rate, frequency, amplitude));
        }
        
        /*
         * The private constructor
         */
        fast_sine_source_c_impl::fast_sine_source_c_impl(float sample_rate,
                                                         float frequency,
                                                         float amplitude)
        : d_sample_rate(sample_rate),
        d_frequency(frequency),
        d_amplitude(amplitude),
        gr::sync_block("fast_sine_source_c",
        gr::io_signature::make(0, 0, 0),
                       gr::io_signature::make(1, 1, sizeof(gr_complex)))
        {
            float w = d_frequency * (2 * M_PI / d_sample_rate);
            
            // Init state
            d_u = 1.;
            d_v = 0.;
            d_k1 = tanf(w/2);
            d_k2 = 2 * d_k1 / (1 + d_k1 * d_k1);
        }
        
        /*
         * Our virtual destructor.
         */
        fast_sine_source_c_impl::~fast_sine_source_c_impl()
        {
        }
        
        int
        fast_sine_source_c_impl::work(int noutput_items,
                                      gr_vector_const_void_star &input_items,
                                      gr_vector_void_star &output_items)
        {
            float tmp;
            
            gr_complex *out = (gr_complex *) output_items[0];

            // It's a recursive oscillator
            // https://www.vicanek.de/articles/QuadOsc.pdf
            for(int n = 0; n < noutput_items; n++) {
                tmp = d_u - d_k1 * d_v;
                d_v = d_v + d_k2 * tmp;
                d_u = tmp - d_k1 * d_v;
                out[n] = d_amplitude * gr_complex(d_u, d_v);
            }
            
            // Tell runtime system how many output items we produced.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
