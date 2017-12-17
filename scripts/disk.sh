#!/bin/bash

usage() {
	cat <<HELP_USAGE
Usage: $0 CREATE disk_name disk_size
       $0 MOUNT disk_name mount_point
    $0 UMOUNT mount_point loopback_device
HELP_USAGE
}

function create_disk {
	# Arguments: disk_name, disk_size
	# Enter su witout changing dir
	# Create flat disk
	dd if=/dev/zero bs=$2 count=1 > $1
	# Create partition
	echo $1
	fdisk $1 <<FDISK_CMD 
n
p



w
FDISK_CMD
	# Create loopback device
	loop0="$(sudo losetup --partscan --show --find $1)"
	loop0p1=$loop0"p1"
	# Make ext4 partition
	sudo mkfs -t ext2 $loop0p1
	# Disconnect loopback device
	sudo losetup -d $loop0
}

function mount_disk {
	# Arguments: disk_name, mount_point
	# Create loopback device
	loop0="$(sudo losetup --partscan --show --find $1)"
	loop0p1=$loop0"p1"
	# Mount first partition
	mkdir -p $2
	sudo mount $loop0p1 $2
	sudo chmod 777 $2
	printf "loopback device: $loop0\n"
}

function umount_disk {
	# Arguments: mount_point loopback_device
	# Umount partition
	sudo umount $1
	# Disconnect loopback device
	sudo losetup -d $2
}

if [ "$#" -le 1 ]; then
    usage
	exit 1
fi

if [ "$1" == "CREATE" ]; then
	if [ "$#" -ne 3 ]; then
    	usage
		exit 1
	fi
	create_disk $2 $3
fi


if [ "$1" == "MOUNT" ]; then
	if [ "$#" -ne 3 ]; then
    	usage
		exit 1
	fi
	mount_disk $2 $3
fi


if [ "$1" == "UMOUNT" ]; then
	if [ "$#" -ne 3 ]; then
    	usage
		exit 1
	fi
	umount_disk $2 $3
fi
