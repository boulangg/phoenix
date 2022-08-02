/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

.section .init
.global _init
_init:
   push %rbp
   movq %rsp, %rbp
   /* gcc will nicely put the contents of crtbegin.o's .init section here. */

.section .fini
.global _fini
_fini:
   push %rbp
   movq %rsp, %rbp
   /* gcc will nicely put the contents of crtbegin.o's .fini section here. */

 .section .init_array
 .align 8
 .global init_array_start
 init_array_start:


 .section .fini_array
 .align 8
 .global fini_array_start
 fini_array_start:
