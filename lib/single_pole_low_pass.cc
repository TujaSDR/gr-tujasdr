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
#include <tujasdr/single_pole_low_pass.h>

namespace gr {
    namespace tujasdr {
        namespace kernel {
            single_pole_low_pass::single_pole_low_pass(float sample_rate, float tau)
            : d_sample_rate(sample_rate),
            d_tau(tau)
            {
                float x = std::exp(-1./(d_sample_rate * d_tau));
                d_a0 = 1 - x;
                d_b1 = x;
                d_z1 = 0;
            }
            
            single_pole_low_pass::~single_pole_low_pass()
            {}
            
            float
            single_pole_low_pass::filter(const float& in) {
                return d_z1 = d_a0 * in + d_b1 * d_z1;
            }
            
            void
            single_pole_low_pass::filterN(float *out, const float *in, int n) {
                // Filter
                for (int i = 0; i < n; i++) {
                    out[n] = filter(in[n]);
                }
            }
        }
    } /* namespace tujasdr */
} /* namespace gr */

