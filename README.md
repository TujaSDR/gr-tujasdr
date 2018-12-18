# gr-tujasdr

GNURadio blocks for TujaSDR trx

## cross compile

```bash
# how to update/create sysroot
rsync -rl --delete-after --safe-links pi@sdr.local:/{lib,usr} $HOME/src/sysroot 
```

```bash
# Notes for myself mostly
sudo chown -R pi:pi /usr/local # not safe but convenient
mkdir build
cd build
cmake -DENABLE_DOXYGEN=OFF -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchains/rbpi3.cmake ..
sshfs -o allow_other,default_permissions pi@sdr.local:/ /mnt/pi
make -j4 install DESTDIR=/mnt/pi
```

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
