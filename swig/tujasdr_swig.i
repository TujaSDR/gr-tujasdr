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
#include "tujasdr/complex_sum.h"
#include "tujasdr/mono_source.h"
#include "tujasdr/opus_encoder_f.h"
#include "tujasdr/unix_dgram_sink_b.h"
%}


%include "tujasdr/alsa_sink.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, alsa_sink);
%include "tujasdr/alsa_source.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, alsa_source);
%include "tujasdr/mono_sink.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, mono_sink);
%include "tujasdr/ssb_rx.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, ssb_rx);
%include "tujasdr/complex_sum.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, complex_sum);

%include "tujasdr/mono_source.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, mono_source);
%include "tujasdr/opus_encoder_f.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, opus_encoder_f);
%include "tujasdr/unix_dgram_sink_b.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, unix_dgram_sink_b);
