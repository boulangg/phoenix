b boot 
add-symbol-file kernel/user/init.out 0x4001c0
target remote :1234
directory kernel/ kernel/boot
c

