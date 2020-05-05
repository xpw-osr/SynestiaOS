# SynestiaOS ![CI](https://github.com/SynestiaOS/SynestiaOS/workflows/CI/badge.svg)
## An Open Source, standards-based software platform for IOT devices, including smartwatch, TVs and automotive infotainment platforms.

# Prepare Environment
## for mac:
``` bash
sudo sh Scripts/SetupMac.sh
```
## for linux:
``` bash
sudo bash Scripts/SetupLinux.sh
```
## for windows:

# Build, Run and Debug
To build kernel:
```
make ARCH=arm   #for raspi2 and arm32
make ARCH=arm64 #for raspi3 and arm64
```
To run kernel:
```
qemu-system-arm -M raspi2 -kernel Kernel/kernel -serial stdio      #for raspi2 and arm32
qemu-system-aarch64 -M raspi3 -kernel Kernel/kernel -serial stdio  #for raspi3 and arm64
```
To clean workspace:
```
make clean
```
To debug kernel, arm32 for instance:
openup terminal 1:
```
qemu-system-arm -M raspi2 -kernel Kernel/kernel -s -S
```
openup terminal 2:
```
gdb-multiarch kernel
(gdb) target remote :1234
(gdb) display/i $pc
(gdb) break _start
(gdb) c
(gdb) si
```
# Project Management
[JIRA](https://synestiaos.atlassian.net/)

# Pipeline
[Jenkins](http://ci.synestiaos.org/)
