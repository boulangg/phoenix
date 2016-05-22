/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

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
