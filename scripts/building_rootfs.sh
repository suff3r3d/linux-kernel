#!/bin/sh

# config busybox with 'make menuconfig'

cd busybox-1.37.0;
make CONFIG_PREFIX=../root install; 
cd ../root
chmod 755 init
mkdir -p var dev etc lib proc tmp sys
find . | cpio -o --format=newc > ../rootfs.cpio
cd ..