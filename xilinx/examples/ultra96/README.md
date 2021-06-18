## Building the ultra96 example - RapidWright
 - The first two steps only have to be run once.
 - Run `java -jar rapidwright_bbaexport.jar xczu3eg-sbva484-1-e xilinx/constids.inc xilinx/xczu3eg.bba` from the root of the repo.
 - Run `./bbasm xilinx/xczu3eg.bba xilinx/xczu3eg.bin -b` from the root of the repo.
 - Run `blinky.sh` in `xilinx/examples/ultra96`.

This was tested using the following configuration:

```
No LSB modules are available.
Distributor ID:	LinuxMint
Description:	Linux Mint 19.1 Tessa
Release:	19.1
Codename:	tessa
```

```
openjdk 11.0.7 2020-04-14
OpenJDK Runtime Environment (build 11.0.7+10-post-Ubuntu-2ubuntu218.04)
OpenJDK 64-Bit Server VM (build 11.0.7+10-post-Ubuntu-2ubuntu218.04, mixed mode, sharing)
```

```
Yosys 0.9+3894 (git sha1 eff18a2b, gcc 7.4.0-1ubuntu1~18.04 -fPIC -Os)
```

```
Vivado v2019.2 (64-bit)
SW Build 2708876 on Wed Nov  6 21:39:14 MST 2019
IP Build 2700528 on Thu Nov  7 00:09:20 MST 2019
Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
```

