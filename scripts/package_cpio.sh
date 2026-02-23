#!/bin/sh

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <rootfs-dir> <output-name.cpio>"
    exit 255
fi

rootfs_dir="$PWD/$1"
output_name="$PWD/$2"
cur_dir="$PWD"

cd $rootfs_dir
find . -print0 | cpio -o --format=newc --null > $output_name
cd $cur_dir

# cd root; find . -print0 | cpio -o --format=newc --null > ../rootfs_updated.cpio; cd ..