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


#ifndef INCLUDED_TUJASDR_FAST_SINE_SOURCE_C_H
#define INCLUDED_TUJASDR_FAST_SINE_SOURCE_C_H

#include <tujasdr/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace tujasdr {

    /*!
     * \brief <+description of block+>
     * \ingroup tujasdr
     *
     */
    class TUJASDR_API fast_sine_source_c : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<fast_sine_source_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of tujasdr::fast_sine_source_c.
       *
       * To avoid accidental use of raw pointers, tujasdr::fast_sine_source_c's
       * constructor is in a private implementation
       * class. tujasdr::fast_sine_source_c::make is the public interface for
       * creating new instances.
       */
      static sptr make(float sample_rate, float frequency, float amplitude=1.0);
    };

  } // namespace tujasdr
} // namespace gr

#endif /* INCLUDED_TUJASDR_FAST_SINE_SOURCE_C_H */

