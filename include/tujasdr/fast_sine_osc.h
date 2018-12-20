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


#ifndef INCLUDED_TUJASDR_FAST_SINE_OSC_H
#define INCLUDED_TUJASDR_FAST_SINE_OSC_H

#include <tujasdr/api.h>

namespace gr {
    namespace tujasdr {
        namespace kernel {
            
            /*!
             * \brief <+description+>
             *
             */
            class TUJASDR_API fast_sine_osc
            {
            private:
                float d_sample_rate;
                float d_frequency;
                float d_v;
                float d_u;
                float d_k1;
                float d_k2;
                
                inline void _update() {
                    float tmp;
                    tmp = d_u - d_k1 * d_v;
                    d_v = d_v + d_k2 * tmp;
                    d_u = tmp - d_k1 * d_v;
                }
            public:
                fast_sine_osc(float sample_rate, float frequency);
                ~fast_sine_osc();

                gr_complex next();
                float next_i();
                float next_q();
            };
        }
    } // namespace tujasdr
} // namespace gr

#endif /* INCLUDED_TUJASDR_FAST_SINE_OSC_H */

