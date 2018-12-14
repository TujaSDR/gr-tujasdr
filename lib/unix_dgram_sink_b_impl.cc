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
#include "unix_dgram_sink_b_impl.h"

#include <stdexcept>

namespace gr {
    namespace tujasdr {
        
        unix_dgram_sink_b::sptr
        unix_dgram_sink_b::make(const std::string& path, const std::string& len_tag_key)
        {
            return gnuradio::get_initial_sptr(new unix_dgram_sink_b_impl(path, len_tag_key));
        }
        
        /*
         * The private constructor
         */
        unix_dgram_sink_b_impl::unix_dgram_sink_b_impl(const std::string& path, const std::string& len_tag_key)
        : gr::tagged_stream_block("unix_dgram_sink_b",
                                  gr::io_signature::make(1, 1, sizeof(unsigned char)),
                                  gr::io_signature::make(0, 0, 0),
                                  len_tag_key)
        {
            // Get a socket
            d_socketd = socket(AF_UNIX, SOCK_DGRAM, 0);
            if(d_socketd < 0) {
                throw std::runtime_error("socket");
            }
            
            // Set destination
            d_socketname.sun_family = AF_UNIX;
            strcpy(d_socketname.sun_path, path.c_str());
            // not in linux d_socketname.sun_len = path.length();
        }
        
        /*
         * Our virtual destructor.
         */
        unix_dgram_sink_b_impl::~unix_dgram_sink_b_impl()
        {
            close(d_socketd);
        }
        
        int
        unix_dgram_sink_b_impl::work(int noutput_items,
                                     gr_vector_int &ninput_items,
                                     gr_vector_const_void_star &input_items,
                                     gr_vector_void_star &output_items)
        {
            const unsigned char *in = (const unsigned char *) input_items[0];

            // printf("in: %d out %d\n", ninput_items[0], noutput_items);
            
            // No blocking
            // We prefer packet loss to blocking
            ssize_t err;
            err = sendto(d_socketd,
                                 in,
                                 (size_t)ninput_items[0],
                                 MSG_DONTWAIT, // don't block flag
                                 (struct sockaddr *)&d_socketname,
                                 sizeof(struct sockaddr_un));
            
            if(err < 0) {
                perror("sending datagram message");
            }

            // Tell runtime system how many output items we consumed.
            // Should be 0 because we are sinking.
            return noutput_items;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
