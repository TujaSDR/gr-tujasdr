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
#include <stdexcept>
#include "opus_encoder_f_impl.h"

namespace gr {
    namespace tujasdr {
        
        opus_encoder_f::sptr
        opus_encoder_f::make(unsigned int sample_rate, unsigned int channels)
        {
            return gnuradio::get_initial_sptr(new opus_encoder_f_impl(sample_rate, channels));
        }
        
        /*
         * The private constructor
         */
        opus_encoder_f_impl::opus_encoder_f_impl(unsigned int sample_rate, unsigned int channels)
        :d_sample_rate(sample_rate),
        d_channels(1),
        d_frame_size(960),
        d_max_data_bytes(4000),
        d_opus_application(OPUS_APPLICATION_AUDIO),
        gr::block("opus_encoder_f",
                       gr::io_signature::make(1, 1, sizeof(float)),
                       gr::io_signature::make(1, 1, sizeof(unsigned char)))
        {
            int err = 0;

            // TODO: how to put in init list?
            d_packet_len_key = pmt::string_to_symbol("packet_len");
            
            // create opus encoder
            d_opus_enc = opus_encoder_create(d_sample_rate, d_channels, d_opus_application, &err);
            if (err) {
                throw std::runtime_error(opus_strerror(err));
            }
            assert(d_opus_enc != NULL);
            
            // important
            set_output_multiple(d_frame_size);
        }
        
        /*
         * Our virtual destructor.
         */
        opus_encoder_f_impl::~opus_encoder_f_impl()
        {
            opus_encoder_destroy(d_opus_enc);
        }
        
        int
        opus_encoder_f_impl::general_work(int noutput_items,
                                          gr_vector_int &ninput_items,
                                          gr_vector_const_void_star &input_items,
                                          gr_vector_void_star &output_items)
        {
            const float *in = (const float *) input_items[0];
            unsigned char *out = (unsigned char *) output_items[0];
            opus_int32 n_err;
            
            
            // TODO: would it be a good idea to implement forecast?
            // TODO: need to check size of output buffer somehow
            // Encode and put directly on the output buffer
            n_err = opus_encode_float(d_opus_enc,
                                      in,
                                      d_frame_size,
                                      out,
                                      d_max_data_bytes);
            
            if (n_err < 0) {
                throw std::runtime_error(opus_strerror(n_err));
            }
            
            //printf("opus packet %d\n", n_err);
            
            // apply packet len tag
            add_item_tag(0, nitems_written(0),
                         d_packet_len_key, pmt::from_long(n_err));
            
            // we call produce
            consume_each(d_frame_size);
            produce(0, n_err);
            
            return WORK_CALLED_PRODUCE;
        }
        
    } /* namespace tujasdr */
} /* namespace gr */
