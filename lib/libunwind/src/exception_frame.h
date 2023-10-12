/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

#include "arch/x86_64/cpu_context.h"

typedef struct unwind_cursor
{
    uint64_t regs[DW_X86_64_SIZE];
    uint32_t x87_env;
    uint32_t mxcsr;
} unwind_cursor_t;

// DWARF Exception Header value format
#define DW_EH_PE_absptr      0x00 /* The Value is a literal pointer whose size is determined by the architecture. */
#define DW_EH_PE_uleb128     0x01 /* Unsigned value is encoded using LEB128 */
#define DW_EH_PE_udata2      0x02 /* A 2 bytes unsigned value */
#define DW_EH_PE_udata4      0x03 /* A 4 bytes unsigned value */
#define DW_EH_PE_udata8      0x04 /* An 8 bytes unsigned value */
#define DW_EH_PE_sleb128     0x09 /* Signed value is encoded using LEB128 */
#define DW_EH_PE_sdata2      0x0A /* A 2 bytes signed value */
#define DW_EH_PE_sdata4      0x0B /* A 4 bytes signed value */
#define DW_EH_PE_sdata8      0x0C /* An 8 bytes signed value */
#define DW_EH_PE_format_mask 0x0F /* Value format mask */

// DWARF Exception Header application
#define DW_EH_PE_pcrel     0x10 /* Value is relative to the current program counter. */
#define DW_EH_PE_textrel   0x20 /* Value is relative to the beginning of the.text section. */
#define DW_EH_PE_datarel   0x30 /* Value is relative to the beginning of the.got or .eh_frame_hdr section. */
#define DW_EH_PE_funcrel   0x40 /* Value is relative to the beginning of the function. */
#define DW_EH_PE_aligned   0x50 /* Value is aligned to an address unit sized boundary. */
#define DW_EH_PE_indirect  0x80 /* Value has to be dereferenced */
#define DW_EH_PE_appl_mask 0x70 /* Application mask */

#define DW_EH_PE_omit 0xff /* No value is present */

// DWARF CFA operations
#define DW_CFA_nop                0x00
#define DW_CFA_set_loc            0x01 /* address */
#define DW_CFA_advance_loc1       0x02 /* 1 - byte delta */
#define DW_CFA_advance_loc2       0x03 /* 2 - byte delta */
#define DW_CFA_advance_loc4       0x04 /* 4 - byte delta */
#define DW_CFA_offset_extended    0x05 /* ULEB128 register */ /* ULEB128 offset */
#define DW_CFA_restore_extended   0x06 /* ULEB128 register */
#define DW_CFA_undefined          0x07 /* ULEB128 register */
#define DW_CFA_same_value         0x08 /* ULEB128 register */
#define DW_CFA_register           0x09 /* ULEB128 register */ /* ULEB128 offset */
#define DW_CFA_remember_state     0x0A
#define DW_CFA_restore_state      0x0B
#define DW_CFA_def_cfa            0x0C /* ULEB128 register */ /* ULEB128 offset */
#define DW_CFA_def_cfa_register   0x0D /* ULEB128 register */
#define DW_CFA_def_cfa_offset     0x0E /* ULEB128 offset */
#define DW_CFA_def_cfa_expression 0x0F /* BLOCK */
#define DW_CFA_expression         0x10 /* ULEB128 register */ /* BLOCK */
#define DW_CFA_offset_extended_sf 0x11 /* ULEB128 register */ /* SLEB128 offset */
#define DW_CFA_def_cfa_sf         0x12 /* ULEB128 register */ /* SLEB128 offset */
#define DW_CFA_def_cfa_offset_sf  0x13 /* SLEB128 offset */
#define DW_CFA_val_offset         0x14 /* ULEB128 ULEB128 */
#define DW_CFA_val_offset_sf      0x15 /* ULEB128 SLEB128 */
#define DW_CFA_val_expression     0x16 /* ULEB128 BLOCK */
#define DW_CFA_lo_user            0x1C
#define DW_CFA_hi_user            0x3F
#define DW_CFA_advance_loc        0x40
#define DW_CFA_offset             0x80 /* low 6 bit: delta */
#define DW_CFA_restore            0xC0 /* low 6 bit: register */ /* ULEB128 offset */
#define DW_CFA_base_op_mask       0xC0 /* low 6 bit: register */
#define DW_CFA_base_param_mask    0x3F

typedef struct common_information_entry
{
    uint64_t length;
    uint64_t codeAlignmentFactor;
    int64_t dataAlignmentFactor;
    uint64_t returnAddrReg;
    uint8_t lsdaEncoding;
    uint8_t fdeEncoding;
    uint64_t personality;
    uint64_t callFrameInstrStart;
    uint64_t callFrameInstrEnd;
} common_information_entry_t;

typedef struct frame_description_entry
{
    uint64_t length;
    common_information_entry_t cie;
    uint64_t ipStart;
    uint64_t ipEnd;
    uint64_t lsda;
    uint64_t callFrameInstrStart;
    uint64_t callFrameInstrEnd;
} frame_description_entry_t;

typedef enum CFA_RULE_TYPE
{
    CFA_RULE_UNDEF = 0,
    CFA_RULE_SAME,
    CFA_RULE_OFFSET,
    CFA_RULE_VAL_OFFSET,
    CFA_RULE_REGISTER,
    CFA_RULE_CFA_REGISTER_OFFSET,
    CFA_RULE_EXPR,
    CFA_RULE_VAL_EXPR,
    CFA_RULE_ARCH,
} CFA_RULE_TYPE;

typedef struct cfa_rule
{
    CFA_RULE_TYPE type;
    uint64_t reg;
    int64_t offset;
} cfa_rule_t;

typedef struct cfa_rules
{
    cfa_rule_t cfa;
    cfa_rule_t rules[DW_X86_64_SIZE];
} cfa_rules_t;
