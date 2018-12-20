/* -*- c++ -*- */

#define TUJASDR_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "tujasdr_swig_doc.i"

%{
#include "tujasdr/alsa_sink.h"
#include "tujasdr/alsa_source.h"
#include "tujasdr/mono_sink.h"
#include "tujasdr/ssb_rx.h"
#include "tujasdr/add_real_imag_cc.h"
#include "tujasdr/mono_source.h"
#include "tujasdr/opus_encoder_f.h"
#include "tujasdr/unix_dgram_sink_b.h"
#include "tujasdr/complex_to_mag_squared_log_vcb.h"
#include "tujasdr/fast_sine_source_c.h"
#include "tujasdr/tx_processor_cc.h"
#include "tujasdr/fast_sine_source_f.h"
#include "tujasdr/single_pole_low_pass_ff.h"
#include "tujasdr/single_pole_low_pass.h"
#include "tujasdr/fast_sine_osc.h"
#include "tujasdr/agc_cc.h"
%}


%include "tujasdr/alsa_sink.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, alsa_sink);
%include "tujasdr/alsa_source.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, alsa_source);
%include "tujasdr/mono_sink.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, mono_sink);
%include "tujasdr/ssb_rx.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, ssb_rx);
%include "tujasdr/add_real_imag_cc.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, add_real_imag_cc);

%include "tujasdr/mono_source.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, mono_source);
%include "tujasdr/opus_encoder_f.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, opus_encoder_f);
%include "tujasdr/unix_dgram_sink_b.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, unix_dgram_sink_b);
%include "tujasdr/complex_to_mag_squared_log_vcb.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, complex_to_mag_squared_log_vcb);
%include "tujasdr/fast_sine_source_c.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, fast_sine_source_c);
%include "tujasdr/tx_processor_cc.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, tx_processor_cc);
%include "tujasdr/fast_sine_source_f.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, fast_sine_source_f);
%include "tujasdr/single_pole_low_pass_ff.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, single_pole_low_pass_ff);

%include "tujasdr/single_pole_low_pass.h"
%include "tujasdr/fast_sine_osc.h"
%include "tujasdr/agc_cc.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, agc_cc);
