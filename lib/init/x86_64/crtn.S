.section .init
   /* gcc will nicely put the contents of crtend.o's .init section here. */
   popq %rbp
   ret
//.size _init, . - _init

.section .fini
   /* gcc will nicely put the contents of crtend.o's .fini section here. */
   popq %rbp
   ret
//.size _fini, . - _fini
