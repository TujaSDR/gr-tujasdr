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
#include "ssb_rx_impl.h"

#include <gnuradio/blocks/rotator.h>
#include <gnuradio/filter/firdes.h>

namespace gr {
    namespace tujasdr {
        
        ssb_rx::sptr
        ssb_rx::make(float sample_rate)
        {
            return gnuradio::get_initial_sptr(new ssb_rx_impl(sample_rate));
        }
        
        /*
         * The private constructor
         */
        ssb_rx_impl::ssb_rx_impl(float sample_rate)
        : d_sample_rate(sample_rate),
        gr::hier_block2("ssb_rx",
                        gr::io_signature::make(1, 1, sizeof(gr_complex)),
                        gr::io_signature::make(1, 1, sizeof(gr_complex)))
        {
            // Create a prototype filter
            //std::vector<float> q = gr::filter::firdes::low_pass(1.0, d_sample_rate, 1500., 100.0);
            
            std::vector<gr_complex> Q =
            gr::filter::firdes::complex_band_pass(1.0,
                                                  d_sample_rate, 300.,
                                                  3300.,
                                                  100.0);
            
            // Create FFT filter
            //d_fft_filter = gr::filter::fft_filter_ccc::make(1, rotate(q, 1700.));
            d_fft_filter = gr::filter::fft_filter_ccc::make(1, Q);
            // Create AGC
            d_agc2 = gr::analog::agc2_cc::make(5e-1, 1e-2, 0.2, 10.0);
            
            d_complex_sum = gr::tujasdr::complex_sum::make(1);
            
            // connect other blocks
            connect(self(), 0, d_fft_filter, 0);
            connect(d_fft_filter, 0, d_agc2, 0);
            connect(d_agc2, 0, d_complex_sum, 0);
            connect(d_complex_sum, 0, self(), 0);
        }
        
        /*
         * Our virtual destructor.
         */
        ssb_rx_impl::~ssb_rx_impl()
        {
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
