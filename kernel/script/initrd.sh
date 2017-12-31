#!/bin/bash

function print_OK {
    printf "    \033[0;32m$1\033[0m\n"
}

function print_INFO {
    printf "    \033[0;33m$1\033[0m\n"
}

function print_ERROR {
    printf "    \033[0;31m$1\033[0m\n"
    exit 1
}

if [ "$#" -ne 1 ]; then
    print_ERROR "Invalid number of arguments\n"
    printf "Usage: $0 diskname\n"
    exit 1
fi

DISK=$1
TMPMOUNT=tmp

rm -f $DISK

# Ramdisk Constants
RDSIZE=1024
BLKSIZE=4096
 
# Create an empty ramdisk image
dd if=/dev/zero of=$DISK bs=$BLKSIZE count=$RDSIZE > /dev/null 2> /dev/null && print_OK "Disk image creation successful" || print_ERROR "Impossible to create disk"
 
# Make it an ext2 mountable file system
/sbin/mke2fs -F -m 0 -b $BLKSIZE $DISK $RDSIZE > /dev/null 2> /dev/null && print_OK "mke2fs successful" || print_ERROR "mke2fs failed"
 
# Mount it so that we can populate
mkdir tmp
sudo mount $DISK $TMPMOUNT -t ext2 -o loop=/dev/loop0 && print_OK "Mount successful" || print_ERROR "Mount failed"

# Allow users to modify mounted disk
sudo chmod a+rwx tmp
 
# Populate the filesystem (subdirectories)
print_INFO "Populate the filesystem"
mkdir $TMPMOUNT/bin
mkdir $TMPMOUNT/sys
mkdir $TMPMOUNT/dev
mkdir $TMPMOUNT/proc
 
# Add executables in /bin
print_INFO "Add executables"
#cp -a /bin/cat $TMPMOUNT/bin
 
# Add some dev files in /dev
#cp -a /dev/console $TMPMOUNT/dev
#cp -a /dev/ramdisk $TMPMOUNT/dev
#cp -a /dev/ram0 $TMPMOUNT/dev
sudo cp -a /dev/null $TMPMOUNT/dev
sudo cp -a /dev/tty $TMPMOUNT/dev
#cp -a /dev/tty1 $TMPMOUNT/dev
#cp -a /dev/tty2 $TMPMOUNT/dev

# Finish up...
sudo umount $TMPMOUNT && print_OK "Unmount successful" || print_ERROR "Unmount failed"
rmdir tmp

