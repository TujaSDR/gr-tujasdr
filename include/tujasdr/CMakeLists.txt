# Copyright 2011,2012 Free Software Foundation, Inc.
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

########################################################################
# Install public header files
########################################################################
install(FILES
    api.h
    alsa_sink.h
    alsa_source.h
    mono_sink.h
    ssb_rx.h
    add_real_imag_cc.h
    mono_source.h
    opus_encoder_f.h
    unix_dgram_sink_b.h
    complex_to_mag_squared_log_vcb.h
    fast_sine_source_c.h
    tx_processor_cc.h
    fast_sine_source_f.h
    single_pole_low_pass_ff.h
    single_pole_low_pass.h
    fast_sine_osc.h
    agc_cc.h DESTINATION include/tujasdr
)
