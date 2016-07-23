Linux Master: [![Linux Build Status](https://travis-ci.org/AchimTuran/asplib.svg?branch=add-spectrum-vis-processor)](https://travis-ci.org/AchimTuran/asplib)


asplib (Achim's Signal Processing LIBrary)
-------
Is a small and lightweight C++ library for digital signal processing.

Projects using asplib
-------
* [adsp.xconvolver](https://github.com/AchimTuran/adsp.xconvolver) (Convolution Engine AudioDSP-Addon for Kodi with several platform optimizations)
* [adsp.biquad.filters](https://github.com/AchimTuran/adsp.biquad.filters) (Graphical Equalizer AudioDSP-Addon for Kodi with several platform optimizations)

How to build aslpib
-------
- Under Windows add system variable "MATLAB_PATH" to build Matlab dll.

Features
-------
* all needed functions for [BiQuad-Filter](http://en.wikipedia.org/wiki/Digital_biquad_filter) calculation
* constant Q peaking filter coefficient calculation
* Native (C++-Classes), SSE2, SSE3 and AVX-optimizations
* Easy and well designed API

Planned Features
-------
* [LibXConvolver](https://github.com/AchimTuran/LibXConvolver) (FIR-Filters, fast non-uniform/uniform convolution) integration
* More IIR-Filter design functions (Low-, High-, Band-, Notch-, Allpass filters)
* Benchmark framework
* logging functions
* dynamic library and static library builds
* REW (Room Equalizer Wizard) filter file import
* Digital Delay Lines
* Dynamic Processors (AGC, Compressor, Limiter, Expander and Noise-Gate)
* Peak Level estimation

