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


#ifndef INCLUDED_TUJASDR_ALSA_SOURCE_H
#define INCLUDED_TUJASDR_ALSA_SOURCE_H

#include <tujasdr/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
    namespace tujasdr {
        
        // Source processing type
        typedef enum {
            COMPLEX = 1,
            KEY_COMPLEX_SINE,
            KEY_ENVELOPE_IMAG,
            KEY_ENVELOPE_FILTERED_IMAG
        } alsa_source_mode_t;
        
        /*!
         * \brief ALSA source for TujaSDR board
         * \ingroup tujasdr
         *
         */
        class TUJASDR_API alsa_source : virtual public gr::sync_block
        {
        public:
            typedef boost::shared_ptr<alsa_source> sptr;
            
            /*!
             * \brief Return a shared_ptr to a new instance of tujasdr::alsa_source.
             *
             * To avoid accidental use of raw pointers, tujasdr::alsa_source's
             * constructor is in a private implementation
             * class. tujasdr::alsa_source::make is the public interface for
             * creating new instances.
             */
            static sptr make(unsigned int sample_rate,
                             const std::string& device_name = "",
                             unsigned int frames_per_period = 2048);
            
            virtual unsigned int frames_per_period() const = 0;
            virtual void set_frames_per_period(unsigned int frames_per_period) = 0;

            virtual alsa_source_mode_t mode() const = 0;
            virtual void set_mode(alsa_source_mode_t mode) = 0;
        };
        
    } // namespace tujasdr
} // namespace gr

#endif /* INCLUDED_TUJASDR_ALSA_SOURCE_H */
