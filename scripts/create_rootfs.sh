#!/bin/sh

# config busybox with 'make menuconfig'

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <busybox-src> <output-name>"
    exit 255
fi

busybox_src="$PWD/$1"
output_name="$PWD/$2"
cur_dir="$PWD"

cd $busybox_src;
make CONFIG_PREFIX=$output_name install; 
cd $output_name
# touch init
# chmod 755 init
mkdir -p var dev etc lib proc tmp sys
# find . | cpio -o --format=newc > "$cur_dir/rootfs.cpio"
cd $cur_dir

# cd busybox-1.37.0;
# make CONFIG_PREFIX=../root install; 
# cd ../root
# chmod 755 init
# mkdir -p var dev etc lib proc tmp sys
# find . | cpio -o --format=newc > ../rootfs.cpio
# cd ..