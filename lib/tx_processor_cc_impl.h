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

#ifndef INCLUDED_TUJASDR_TX_PROCESSOR_CC_IMPL_H
#define INCLUDED_TUJASDR_TX_PROCESSOR_CC_IMPL_H

#include <tujasdr/tx_processor_cc.h>

namespace gr {
    namespace tujasdr {
        
        class tx_processor_cc_impl : public tx_processor_cc
        {
        private:
            float d_sample_rate;
            
        public:
            tx_processor_cc_impl(float sample_rate);
            ~tx_processor_cc_impl();
            
            // Where all the action really happens
        };
        
    } // namespace tujasdr
} // namespace gr

#endif /* INCLUDED_TUJASDR_TX_PROCESSOR_CC_IMPL_H */
