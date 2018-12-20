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
#include <tujasdr/fast_sine_osc.h>

namespace gr {
    namespace tujasdr {
        namespace kernel {
            
            fast_sine_osc::fast_sine_osc(float sample_rate, float frequency)
            : d_sample_rate(sample_rate),
            d_frequency(frequency)
            {
                float w = d_frequency * (2 * M_PI / d_sample_rate);
                // Init state
                d_u = 1.;
                d_v = 0.;
                d_k1 = tanf(w/2);
                d_k2 = 2 * d_k1 / (1 + d_k1 * d_k1);
            }
            
            fast_sine_osc::~fast_sine_osc()
            {
            }
            
            // TODO: maybe turn this into a template class?
            gr_complex
            fast_sine_osc::next() {
                _update();
                return gr_complex(d_u, d_v);
            }
            float
            fast_sine_osc::next_i() {
                _update();
                return d_u;
            }
        
            float
            fast_sine_osc::next_q() {
                _update();
                return d_v;
            }
        }
    } /* namespace tujasdr */
} /* namespace gr */
