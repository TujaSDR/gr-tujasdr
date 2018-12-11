# gr-tujasdr

GNURadio blocks for TujaSDR trx

## blocks

### alsa_sink and alsa_source

Intended to be used with the TujaSDR board fixed S32 and some performance hacks.

### mono_sink and mono_source

Intended to be used with the alsa loopback device with wsjt-x and fldigi etc. Opens the ALSA loopback soundcard using a format that wsjt-x and fldigi likes.

### ssb_rx

SSB receiver block with AGC

### complex_sum

Complex input, output float = real + imag. This is useful for SSB demodulation.

## build

There's some bug in the documentation at this point to build without it.

```bash
mkdir build
cd build
cmake -DENABLE_DOXYGEN=OFF ..
make -j4
```
