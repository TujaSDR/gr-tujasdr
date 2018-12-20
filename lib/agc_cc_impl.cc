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
#include "agc_cc_impl.h"

namespace gr {
    namespace tujasdr {
        
        float
        fast_magnitude(const gr_complex& c) {
            const float alpha = 0.947543636291;
            const float beta = 0.392485425092;
            float abs_i = fabsf(c.real());
            float abs_q = fabsf(c.imag());
            
            // Mag ~=Alpha * max(|I|, |Q|) + Beta * min(|I|, |Q|)
            if (abs_i > abs_q) {
                return alpha * abs_i + beta * abs_q;
            } else {
                return alpha * abs_q + beta * abs_i;
            }
        }
        
        float
        magnitude_squared(const gr_complex& c) {
            // a complex number multiplied with its conjugate is always real
            return (c * conj(c)).real();
        }
        
        agc_cc::sptr
        agc_cc::make(float attack_rate, float decay_rate, float reference, float gain, float max_gain)
        {
            return gnuradio::get_initial_sptr
            (new agc_cc_impl(attack_rate, decay_rate, reference, gain, max_gain));
        }
        
        /*
         * The private constructor
         */
        agc_cc_impl::agc_cc_impl(float attack_rate, float decay_rate,
                                 float reference, float gain, float max_gain)
        : d_attack_rate(attack_rate),
        d_decay_rate(decay_rate),
        d_reference(reference),
        d_gain(gain),
        d_max_gain(max_gain),
        gr::sync_block("agc_cc",
                         gr::io_signature::make(1, 1, sizeof(gr_complex)),
                         gr::io_signature::make(1, 1, sizeof(gr_complex)))
        {
            d_power_function = magnitude_squared;
            //d_power_function = fast_magnitude;
        }
        
        /*
         * Our virtual destructor.
         */
        agc_cc_impl::~agc_cc_impl()
        {
        }
        
        int
        agc_cc_impl::work(int noutput_items,
                          gr_vector_const_void_star &input_items,
                          gr_vector_void_star &output_items)
        {
            const gr_complex *in = (const gr_complex *) input_items[0];
            gr_complex *out = (gr_complex *) output_items[0];
            
            float power = 0.;
            float diff  = 0.;
            
            for (int n = 0; n < noutput_items; n++) {
                out[n] = in[n] * d_gain;
                // estimate power
                //power = abs(out[n]); //d_power_function(out[n]);
                power = d_power_function(out[n]);
                diff = d_reference - power;
                if(diff > 0) {
                    // power is lower than reference, increase gain
                    d_gain += diff * d_decay_rate;
                } else {
                    // power is higher than reference, decrese gain
                    // why +? Remember diff is negative.
                    d_gain += diff * d_attack_rate;
                }
            }
            
            // Tell runtime system how many output items we produced.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
