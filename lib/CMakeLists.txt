# Copyright 2011,2012,2016 Free Software Foundation, Inc.
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
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(
	${Boost_INCLUDE_DIR}
	${ALSA_INCLUDE_DIR}
	${OPUS_INCLUDE_DIR}
)

link_directories(${Boost_LIBRARY_DIRS})
list(APPEND tujasdr_sources
    alsa.c
    alsa_sink_impl.cc
    alsa_source_impl.cc
    mono_sink_impl.cc
    ssb_rx_impl.cc
    add_real_imag_cc_impl.cc
    mono_source_impl.cc
    opus_encoder_f_impl.cc
    unix_dgram_sink_b_impl.cc
    complex_to_mag_squared_log_vcb_impl.cc
    fast_sine_source_c_impl.cc
    tx_processor_cc_impl.cc
    fast_sine_source_f_impl.cc
    single_pole_low_pass_ff_impl.cc
    single_pole_low_pass.cc
    fast_sine_osc.cc
    agc_cc_impl.cc )

set(tujasdr_sources "${tujasdr_sources}" PARENT_SCOPE)
if(NOT tujasdr_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT tujasdr_sources)

add_library(gnuradio-tujasdr SHARED ${tujasdr_sources})
target_link_libraries(
  gnuradio-tujasdr
  ${ALSA_LIBRARIES}
  ${OPUS_LIBRARIES}
  ${Boost_LIBRARIES}
  ${GNURADIO_ALL_LIBRARIES}
)
set_target_properties(gnuradio-tujasdr PROPERTIES DEFINE_SYMBOL "gnuradio_tujasdr_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-tujasdr PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-tujasdr RUNTIME_COMPONENT "tujasdr_runtime" DEVEL_COMPONENT "tujasdr_devel")

########################################################################
# Build and register unit test
########################################################################
# include(GrTest)

# include_directories(${CPPUNIT_INCLUDE_DIRS})

# list(APPEND test_tujasdr_sources
#     ${CMAKE_CURRENT_SOURCE_DIR}/test_tujasdr.cc
#     ${CMAKE_CURRENT_SOURCE_DIR}/qa_tujasdr.cc
# )

# add_executable(test-tujasdr ${test_tujasdr_sources})

# target_link_libraries(
#   test-tujasdr
#   ${GNURADIO_RUNTIME_LIBRARIES}
#   ${Boost_LIBRARIES}
#   ${CPPUNIT_LIBRARIES}
#   ${ALSA_LIBRARIES}
#   ${OPUS_LIBRARIES}
#   gnuradio-tujasdr
# )

# GR_ADD_TEST(test_tujasdr test-tujasdr)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

