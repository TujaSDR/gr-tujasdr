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
#include <volk/volk.h>
#include "complex_to_mag_squared_log_vcb_impl.h"

namespace gr {
    namespace tujasdr {
        
        complex_to_mag_squared_log_vcb::sptr
        complex_to_mag_squared_log_vcb::make(size_t vlen)
        {
            return gnuradio::get_initial_sptr
            (new complex_to_mag_squared_log_vcb_impl(vlen));
        }
        
        /*
         * The private constructor
         */
        complex_to_mag_squared_log_vcb_impl::complex_to_mag_squared_log_vcb_impl(size_t vlen)
        : d_vlen(vlen),
        gr::sync_block("complex_to_mag_squared_log_vcb",
                         gr::io_signature::make(1, 1, vlen * sizeof(gr_complex)),
                         gr::io_signature::make(1, 1, vlen * sizeof(unsigned char)))
        {
            d_alignment = volk_get_alignment();
            d_mag_squared = (float*)volk_malloc(sizeof(float)*d_vlen, d_alignment);
            d_log2 = (float*)volk_malloc(sizeof(float)*d_vlen, d_alignment);
            
            /*
            unsigned int alignment = volk_get_alignment();
             float* in = (float*)volk_malloc(sizeof(float)*N, alignment);
            float* out = (float*)volk_malloc(sizeof(float)*N, alignment);
             */
        }
        
        /*
         * Our virtual destructor.
         */
        complex_to_mag_squared_log_vcb_impl::~complex_to_mag_squared_log_vcb_impl()
        {
            volk_free(d_mag_squared);
            volk_free(d_log2);
        }
        
        int
        complex_to_mag_squared_log_vcb_impl::work(int noutput_items,
                                                  gr_vector_const_void_star &input_items,
                                                  gr_vector_void_star &output_items)
        {
            const gr_complex *in = (const gr_complex *) input_items[0];
            
            // TODO: change to c++ casting
            // Magnitude squared
            volk_32fc_magnitude_squared_32f(d_mag_squared, in, (unsigned int) d_vlen);
            // Log of that
            volk_32f_log2_32f(d_log2, d_mag_squared, (unsigned int) d_vlen);
            
            // Convert to log 10 and scale by fft length
            // and then somehow compress into 255 bytes
            
            // FFT -> complex_to_mag^2 -> log10 -> -60.20
            // Scale by 20*log10(1024) = 60.20
            // TODO. use volk malloc
            
            // Do <+signal processing+>
            
            // Tell runtime system how many output items we produced.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */

