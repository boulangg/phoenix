#!/bin/sh

# Arguments: user_apps_dir, output_file

rm $2 2> /dev/null

touch $2
echo /\* This file is generated \*/ >> $2

echo .section .rawdata >> $2

nb_apps=0
# Generate include in output file
for entry in "$1"/*.out
do
  	filename=$(basename "$entry")
	filename="${filename%.*}"
	echo >> $2
	echo .align 4096 >> $2
	echo _user_apps_start_$filename : >> $2
  	echo .incbin \"$entry\" >> $2
	echo _user_apps_end_$filename: >> $2
	nb_apps=$((nb_apps+1))
done

# Generate symbol table
echo >> $2
echo .globl user_apps_symbol_table >> $2
echo user_apps_symbol_table: >> $2
echo -e \\t.quad $nb_apps >> $2
for entry in "$1"/*.out
do
  	filename=$(basename "$entry")
	filename="${filename%.*}"
	echo -e """\t.quad _user_apps_name_$filename""" >> $2
	echo -e """\t.quad _user_apps_start_$filename""" >> $2
	echo -e """\t.quad _user_apps_end_$filename""" >> $2
done

# Generate string table

echo >> $2
for entry in "$1"/*.out
do
  	filename=$(basename "$entry")
	filename="${filename%.*}"
	echo _user_apps_name_$filename: >> $2
	echo -e \\t.ascii \"$filename\\\\0\" >> $2
done
