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

#ifndef INCLUDED_TUJASDR_SSB_RX_IMPL_H
#define INCLUDED_TUJASDR_SSB_RX_IMPL_H

#include <tujasdr/ssb_rx.h>
#include <tujasdr/add_real_imag_cc.h>
// TODO: change agc
#include <gnuradio/analog/agc2_cc.h>
#include <gnuradio/analog/agc3_cc.h>
#include <gnuradio/filter/fft_filter_ccc.h>

namespace gr {
    namespace tujasdr {
        
        class ssb_rx_impl : public ssb_rx
        {
        private:
            float d_sample_rate;
            
            gr::tujasdr::add_real_imag_cc::sptr d_add_real_imag;
            // gr::analog::agc2_cc::sptr d_agc2;
            gr::analog::agc3_cc::sptr d_agc3;
            gr::filter::fft_filter_ccc::sptr d_fft_filter;
            
        public:
            ssb_rx_impl(float sample_rate);
            ~ssb_rx_impl();
            
            // Where all the action really happens
        };
        
    } // namespace tujasdr
} // namespace gr

#endif /* INCLUDED_TUJASDR_SSB_RX_IMPL_H */
