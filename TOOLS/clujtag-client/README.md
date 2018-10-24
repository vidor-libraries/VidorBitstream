clujtag-client
==============

Client for AVR-based JTAG programmer: https://github.com/ClusterM/clujtag-avr

It's based on Lib(X)SVF (http://www.clifford.at/libxsvf/) so you can use it to flash SVF or XSVF files. Use MinGW (www.mingw.org) to compile it for Windows if need. Linux users are welome too.

How to use:

Usage:
clujtag [ -v ... ] -p portname { -s svf-file | -x xsvf-file | -c }
* -p portname - use specified serial port for communication with programmer (COM*xx* for Windows and /dev/tty*xxx* for Linux)
* -v, -vv, -vvv, -vvvv - verbose, more verbose and even more verbose
* -s svf-file - play the specified SVF file
* -x xsvf-file - play the specified XSVF file
* -c - list devices in JTAG chain


Examples:

    >clujtag -p COM16 -c
    Scanning JTAG chain...
    Device found: idcode=0x170640dd, revision=0x1, part=0x7064, manufactor=0x06e


    >clujtag -v -p /dev/ttyACM0 -s MMC1.svf
    clujtag
    Copyright (C) 2009  RIEGL Research ForschungsGmbH
    Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>
    Copyright (C) 2014  Cluster <clusterrr@clusterrr.com>
    Lib(X)SVF is free software licensed under the ISC license.
    Playing SVF file `MMC1.svf'.
    Done!
