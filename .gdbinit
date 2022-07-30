b boot 
add-symbol-file kernel/build/user_apps/init 0x4001c0
target remote :1234
directory kernel/src
c

