/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

.section .init
   /* gcc will nicely put the contents of crtend.o's .init section here. */
   leaveq
   retq

.section .fini
   /* gcc will nicely put the contents of crtend.o's .fini section here. */
   leaveq
   retq

 .section .init_array
 .align 8
 .global init_array_end
 init_array_end:


 .section .fini_array
 .align 8
 .global fini_array_end
 fini_array_end:

