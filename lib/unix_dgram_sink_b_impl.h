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

#ifndef INCLUDED_TUJASDR_UNIX_DGRAM_SINK_B_IMPL_H
#define INCLUDED_TUJASDR_UNIX_DGRAM_SINK_B_IMPL_H

#include <tujasdr/unix_dgram_sink_b.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

namespace gr {
    namespace tujasdr {
        
        class unix_dgram_sink_b_impl : public unix_dgram_sink_b
        {
        private:
            int d_socketd;
            struct sockaddr_un d_socketname;
            
        public:
            unix_dgram_sink_b_impl(const std::string& path, const std::string& len_tag_key);
            ~unix_dgram_sink_b_impl();
            
            // Where all the action really happens
            int work(int noutput_items,
                     gr_vector_int &ninput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items);
        };
        
    } // namespace tujasdr
} // namespace gr

#endif /* INCLUDED_TUJASDR_UNIX_DGRAM_SINK_B_IMPL_H */

