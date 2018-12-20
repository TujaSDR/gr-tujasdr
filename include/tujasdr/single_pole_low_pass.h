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


#ifndef INCLUDED_TUJASDR_SINGLE_POLE_LOW_PASS_H
#define INCLUDED_TUJASDR_SINGLE_POLE_LOW_PASS_H

#include <tujasdr/api.h>

namespace gr {
    namespace tujasdr {
        namespace kernel {
            
            /*!
             * \brief <+description+>
             *
             */
            class TUJASDR_API single_pole_low_pass
            {
            private:
                float d_sample_rate;
                float d_tau;
                
                float d_a0;
                float d_b1;
                float d_z1;
            public:
                single_pole_low_pass(float sample_rate, float tau);
                ~single_pole_low_pass();

                float filter(const float& in);
                void filterN(float *out, const float *in, int n);
            };
        }
    } // namespace tujasdr
} // namespace gr

#endif /* INCLUDED_TUJASDR_SINGLE_POLE_LOW_PASS_H */
