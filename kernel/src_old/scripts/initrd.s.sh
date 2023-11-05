#!/bin/sh

# Arguments: disk_file_name, output_file

rm $2 2> /dev/null

touch $2

printf "/\* This file is generated \*/\n" >> $2
printf ".section .rawdata\n" >> $2
printf "\n" >> $2
printf ".align 4096\n" >> $2
printf ".globl _initrd_start\n" >> $2
printf "_initrd_start:\n" >> $2
printf ".incbin \"$1\"\n" >> $2
printf ".globl _initrd_end\n" >> $2
printf "_initrd_end:\n" >> $2

