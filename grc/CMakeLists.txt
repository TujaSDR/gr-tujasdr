# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
install(FILES
    tujasdr_alsa_sink.xml
    tujasdr_alsa_source.xml
    tujasdr_mono_sink.xml
    tujasdr_ssb_rx.xml
    tujasdr_add_real_imag_cc.xml
    tujasdr_mono_source.xml
    tujasdr_opus_encoder_f.xml
    tujasdr_unix_dgram_sink_b.xml
    tujasdr_complex_to_mag_squared_log_vcb.xml
    tujasdr_fast_sine_source_c.xml
    tujasdr_tx_processor_cc.xml
    tujasdr_fast_sine_source_f.xml
    tujasdr_single_pole_low_pass_ff.xml
    tujasdr_single_pole_low_pass.xml
    tujasdr_fast_sine_osc.xml
    tujasdr_agc_cc.xml DESTINATION share/gnuradio/grc/blocks
)
