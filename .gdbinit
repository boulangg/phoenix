b boot 
add-symbol-file kernel/user/test1.out 0x4001c0
target remote :1234
directory kernel/ kernel/boot
c

