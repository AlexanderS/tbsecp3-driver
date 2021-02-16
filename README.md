# TBSECP3 driver (standalone version)

This is a modified version of the tbsecp3 and saa716x modules (and the
dependend modules) from the
[tbsdtv/linux_media](https://github.com/tbsdtv/linux_media) repository.

These are out-of-tree modules and do **not** require a full v4l media tree.

It is currently only tested on ArchLinux (Linux 5.8) with a _TBS 6281SE
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
  * m88rs6060
  * mb86a16
  * mn88436
  * mtv23x
  * mxl58x
  * si2168
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
  * si2157
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

* The tbsdtv/linux\_media tree contains some patches to add `spi_read`,
  `spi_write`, `eeprom_read` and `eeprom_write` functions to the device
  operations for the dvb-frontends. This requires a modification of the
  dvb-core module. Because the aim of this repo is to use the dvb-core
  module from your running linux kernel, this does not work.
  
  This modifications are ifdef'ed with `TBS_STANDALONE`. So you cannot use
  `spi_read`, `spi_write`, `eeprom_read` and `eeprom_write` with the modules
  from this repository. But this should no be required for normal operation.

* Some files are missing an include of `linux/version.h`.

* The saa716x driver contains a custom autorepeat handling for ir commands.
  Because the kernel changed the timer API in 4.14 the custom handling is
  removed when building for newer kernels (the autorepeat is handled in the
  input layer now). This is the kernel commit
  [5aeaa3e668de0782d1502f3d5751e2266a251d7c](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=5aeaa3e668de0782d1502f3d5751e2266a251d7c)
  applied to the saa716x driver.

* The saa716 driver contains a handler for the `AUDIO_GET_PTS` ioctl.
  Support for this ioctl was removed in kernel commit
  [d21c249b26311dd193b100e65fc9e7ae96233d40](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=d21c249b26311dd193b100e65fc9e7ae96233d40).
  The ioctl handler is removed via ifdef if `AUDIO_GET_PTS` is not availabe.

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
