#!/bin/sh
cd root; find . -print0 | cpio -o --format=newc --null > ../rootfs_updated.cpio; cd ..