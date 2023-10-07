.pushsection ".debug_gdb_scripts", "MS",@progbits,1
.byte 4 /* Python Text */
.ascii "gdb.inlined-script\n"
.incbin "/home/boulangg/.vs/phoenix/lib/libstdcpp/debug/gdb-printers.py"
.byte 0
.popsection
