/* -*- c++ -*- */

#define TUJASDR_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "tujasdr_swig_doc.i"

%{
#include "tujasdr/alsasink.h"
%}


%include "tujasdr/alsasink.h"
GR_SWIG_BLOCK_MAGIC2(tujasdr, alsasink);
