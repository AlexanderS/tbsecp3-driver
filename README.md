# TBSECP3 driver (standalone version)

This is a modified version of the tbsecp3 module (and the dependend modules)
from the [tbsdtv/linux_media](https://github.com/tbsdtv/linux_media)
repository.

These are out-of-tree modules and do **not** require a full v4l media tree.

It is currently only tested on ArchLinux (Linux 4.18) with a _TBS 6281SE
DVB-T/T2/C_.

## Contents

This repo currently contains the following modules:

* tbsecp3
* saa716x
  * saa716x_core
  * saa716x_ff
  * saa716x_hybrid
  * saa716x_tbs-dvb
* dvb-frontends
  * cx24117
  * gx1133
  * gx1503
  * isl6422
  * isl6423
  * mb86a16
  * mn88436
  * mtv23x
  * mxl58x
  * si2183
  * stb6100
  * stid135
  * stv090x
  * stv091x
  * stv6110x
  * tas2101
  * tas2971
  * tda1004x
  * zl10353
* tuners
  * av201x
  * mxl603
  * rda5816
  * stv6120
  * tda18212
  * tda827x
  * tda8290

## Modifications

The upstream drivers are only modified to fix some build failures. Currently
there are two major problems with the upstream drivers:

* The min and max frequency settings [recently changed in the v4l media
  tree](https://patchwork.kernel.org/patch/10507827/). The units of the
  settings were converted to Hz and some defines for MHz and kHz were
  introduced. The media tree in the kernel does not contain this changes yet,
  so we need to add back the old format of the settings inside `#ifdef`.

* The tbsdtv/linux\_media tree contains some patches to add `spi_read` and
  `spi_write` functions to the device operations for the dvb-frontends and
  tuners. This requires a modification of the dvb-core module. Because the aim
  of this repo is to use the dvb-core module from your running linux kernel,
  this does not work.
  
  This modifications are ifdef'ed with `TBS_STANDALONE`. So you cannot use
  `spi_read` and `spi_write` with the modules from this repository. But this
  should no be required for normal operation.

* Some files are missing an include of `linux/version.h`.

## Build

To build the modules just execute:

```
make
```

and install the modules with:

```
make modules_install
```

This will build the modules for the running kernel. If you want to build the
modules for another kernel (maybe during update before rebooting) you can use
the `KDIR` variable:

```
make KDIR=/lib/modules/4.18.12-arch1-1-ARCH/build
make KDIR=/lib/modules/4.18.12-arch1-1-ARCH/build modules_install
```
