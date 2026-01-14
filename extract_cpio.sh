#!/bin/sh
mkdir root; cd root; cpio -idv < ../rootfs.cpio; cd ..