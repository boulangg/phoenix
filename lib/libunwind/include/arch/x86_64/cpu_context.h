/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#define DW_X86_64_RAX  0
#define DW_X86_64_RDX  1
#define DW_X86_64_RCX  2
#define DW_X86_64_RBX  3
#define DW_X86_64_RSI  4
#define DW_X86_64_RDI  5
#define DW_X86_64_RBP  6
#define DW_X86_64_RSP  7
#define DW_X86_64_R8   8
#define DW_X86_64_R9   9
#define DW_X86_64_R10  10
#define DW_X86_64_R11  11
#define DW_X86_64_R12  12
#define DW_X86_64_R13  13
#define DW_X86_64_R14  14
#define DW_X86_64_R15  15
#define DW_X86_64_RIP  16
#define DW_X86_64_SIZE 17

#define DW_X86_64_OFFSET_RAX     (DW_X86_64_RAX * 0x8)
#define DW_X86_64_OFFSET_RDX     (DW_X86_64_RDX * 0x8)
#define DW_X86_64_OFFSET_RCX     (DW_X86_64_RCX * 0x8)
#define DW_X86_64_OFFSET_RBX     (DW_X86_64_RBX * 0x8)
#define DW_X86_64_OFFSET_RSI     (DW_X86_64_RSI * 0x8)
#define DW_X86_64_OFFSET_RDI     (DW_X86_64_RDI * 0x8)
#define DW_X86_64_OFFSET_RBP     (DW_X86_64_RBP * 0x8)
#define DW_X86_64_OFFSET_RSP     (DW_X86_64_RSP * 0x8)
#define DW_X86_64_OFFSET_R8      (DW_X86_64_R8 * 0x8)
#define DW_X86_64_OFFSET_R9      (DW_X86_64_R9 * 0x8)
#define DW_X86_64_OFFSET_R10     (DW_X86_64_R10 * 0x8)
#define DW_X86_64_OFFSET_R11     (DW_X86_64_R11 * 0x8)
#define DW_X86_64_OFFSET_R12     (DW_X86_64_R12 * 0x8)
#define DW_X86_64_OFFSET_R13     (DW_X86_64_R13 * 0x8)
#define DW_X86_64_OFFSET_R14     (DW_X86_64_R14 * 0x8)
#define DW_X86_64_OFFSET_R15     (DW_X86_64_R15 * 0x8)
#define DW_X86_64_OFFSET_RIP     (DW_X86_64_RIP * 0x8)
#define DW_X86_64_OFFSET_X87_ENV (DW_X86_64_SIZE * 0x8 + 0x0)
#define DW_X86_64_OFFSET_MXCSR   (DW_X86_64_SIZE * 0x8 + 0x4)
#define DW_X86_64_OFFSET_CFA     (DW_X86_64_SIZE * 0x8 + 0x8)
