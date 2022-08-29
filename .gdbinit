b boot 
add-symbol-file bin/kernel/init 0x4001c0
target remote :1234
directory kernel/src
c
