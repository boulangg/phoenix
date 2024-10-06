#!/bin/sh

# Arguments: user_apps_dir, output_file

rm $2 2> /dev/null

touch $2
printf "/\* This file is generated \*/\n" >> $2

printf ".section .rawdata\n" >> $2

nb_apps=0
# Generate include in output file
for entry in "$1"/*
do
  	filename=$(basename "$entry")
	filename="${filename%.*}"
	printf "\n" >> $2
	printf ".align 4096\n" >> $2
	printf "_user_apps_start_$filename:\n" : >> $2
	printf ".incbin \"$entry\"\n" >> $2
	printf "_user_apps_end_$filename:\n" >> $2
	nb_apps=$((nb_apps+1))
done

# Generate symbol table
printf "\n" >> $2
printf ".globl user_apps_symbol_table\n" >> $2
printf "user_apps_symbol_table:\n" >> $2
printf "\t.quad $nb_apps\n" >> $2
for entry in "$1"/*
do
  	filename=$(basename "$entry")
	filename="${filename%.*}"
	printf "\t.quad _user_apps_name_$filename\n" >> $2
	printf "\t.quad _user_apps_start_$filename\n" >> $2
	printf "\t.quad _user_apps_end_$filename\n" >> $2
done

# Generate string table

echo >> $2
for entry in "$1"/*
do
  	filename=$(basename "$entry")
	filename="${filename%.*}"
	printf "_user_apps_name_$filename:\n" >> $2
	printf "\t.asciz \"$filename\"\n" >> $2
done

# disable executable stack
printf ".section .note.GNU-stack,\"\",@progbits" >> $2
