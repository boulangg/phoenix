/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "InterruptDescTable.h"

namespace kernel::core {

namespace IDT {
idt_desc idt[IDT_ENTRIES] __attribute__((aligned(4096)));

void setupIDT()
{
    set_IDT(IDT_SIZE, idt);
}

}

}
