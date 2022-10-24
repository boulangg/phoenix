#include "exception_frame.h"

#include "dl_helpers.h"
#include "string.h"
#include "dwarf_decode.h"
#include "debug.h"
#include "unwind_internal.h"
#include <stdlib.h>

typedef uint8_t DW_EH_PE;

typedef struct
{
	uint8_t		version;
	DW_EH_PE 	ehFramePtrEnc;
	DW_EH_PE	fdeCountEnc;
	uint8_t 	tableEnc;
	char		content[];
} EF_EHF_Hdr;

typedef struct EF_EHF_LT
{
	int32_t initialLoc;
	int32_t fdePtr;
} EF_EHF_LT;

Elf64_Phdr* getEhFrameHeader(const struct link_map* obj)
{
	for (size_t i = 0; i < obj->l_phnum; ++i) {
		Elf64_Phdr* programHeader = (Elf64_Phdr*)obj->l_phdr + i;
		if (programHeader->p_type == PT_GNU_EH_FRAME) {
			return programHeader;
		}
	}
	return NULL;
}

uint64_t unwind_find_frame_description_entry(uint64_t ip)
{
	struct link_map* obj = _dl_find_dso_for_object(ip);
	if (obj == NULL) {
		return NULL;
	}

	Elf64_Phdr* programHeader = getEhFrameHeader(obj);
	if (programHeader == NULL) {
		return NULL;
	}

	EF_EHF_Hdr* ehFrameHeader = (EF_EHF_Hdr*)programHeader->p_vaddr;
	uint64_t ehFrameData = (uint64_t) & (ehFrameHeader->content);
	uint64_t ehFrameSPtr = unwind_dwarf_read_u32(&ehFrameData);
	(void)ehFrameSPtr;
	uint64_t fde_count = unwind_dwarf_read_u32(&ehFrameData);
	EF_EHF_LT* lookupTable = (EF_EHF_LT*)ehFrameData;
	size_t i = 0;
	// TODO Used the binary search table
	while ((uint64_t)ehFrameHeader + lookupTable[i].initialLoc < ip && i <= fde_count) {
		i++;
	}

	if (i > fde_count || i == 0) {
		return NULL;
	}

	uint64_t fde_ptr = (uint64_t)ehFrameHeader + lookupTable[i - 1].fdePtr;

	return fde_ptr;
}

int unwind_parse_common_information_entry(uint64_t* addr, common_information_entry_t* outCie)
{
	common_information_entry_t cie;
	memset(&cie, 0, sizeof(cie));

	cie.length = unwind_dwarf_read_u32(addr);
	uint64_t cieEnd = *addr + cie.length;

	int32_t CIEID = unwind_dwarf_read_s32(addr);
	(void)CIEID;
	uint8_t version = unwind_dwarf_read_u8(addr);
	(void)version;

	char* augmentString = (char*)*addr;
	size_t augmentStringLen = strlen(augmentString);
	*addr += augmentStringLen + 1;

	cie.codeAlignmentFactor = unwind_dwarf_read_uleb128(addr);
	cie.dataAlignmentFactor = unwind_dwarf_read_sleb128(addr);
	cie.returnAddrReg = unwind_dwarf_read_uleb128(addr);

	cie.personality = 0;
	cie.lsdaEncoding = DW_EH_PE_omit;

	if (augmentString[0] == 'z') {
		uint64_t augmentationLength = unwind_dwarf_read_uleb128(addr);
		(void)augmentationLength;

		for (size_t i = 1; augmentString[i] != '\0'; i++) {
			switch (augmentString[i]) {
			case 'L':
			{
				// L: LSDA encoding (1-bit)
				cie.lsdaEncoding = unwind_dwarf_read_u8(addr);
				continue;
			}
			case 'P':
			{
				// P: personality encoding (1-bit) + encoded_ptr
				uint8_t personalityEncoding = unwind_dwarf_read_u8(addr);
				cie.personality = unwind_dwarf_read_encoded_pointer(addr, personalityEncoding);
				continue;
			}
			case 'R':
			{
				// R: FDE encoding (1-bit)
				cie.fdeEncoding = unwind_dwarf_read_u8(addr);
				continue;
			}
			default:
				break;
			}
		}
	}

	cie.callFrameInstrStart = *addr;
	cie.callFrameInstrEnd = cieEnd;

	memcpy(outCie, &cie, sizeof(cie));
	return 0;
}

int unwind_parse_frame_description_entry(uint64_t* addr, frame_description_entry_t* outFde)
{
	frame_description_entry_t fde;
	memset(&fde, 0, sizeof(fde));

	fde.length = unwind_dwarf_read_u32(addr);
	uint64_t fdeEnd = *addr + fde.length;

	uint64_t cieBaseAddr = *addr;
	uint64_t cieAddr = cieBaseAddr - unwind_dwarf_read_s32(addr);

	unwind_parse_common_information_entry(&cieAddr, &fde.cie);

	uint8_t fde_encoding = 0x1B; // = cie.fde_encoding
	fde.ipStart = unwind_dwarf_read_encoded_pointer(addr, fde_encoding);
	uint64_t ip_range = unwind_dwarf_read_encoded_pointer(addr, fde_encoding & DW_EH_PE_format_mask);
	fde.ipEnd = fde.ipStart + ip_range;

	fde.lsda = 0;
	// TODO add check for lsda presence before CFI
	uint64_t augmentationLength = unwind_dwarf_read_uleb128(addr);
	(void)augmentationLength;
	fde.lsda = unwind_dwarf_read_encoded_pointer(addr, fde.cie.lsdaEncoding);

	fde.callFrameInstrStart = *addr;
	fde.callFrameInstrEnd = fdeEnd;

	memcpy(outFde, &fde, sizeof(fde));
	return 0;
}

int unwind_get_frame_description_entry(uint64_t ip, frame_description_entry_t* outFde)
{
	uint64_t fdeAddr = unwind_find_frame_description_entry(ip);
	unwind_parse_frame_description_entry(&fdeAddr, outFde);
	DEBUG_LOG("FDE (0x%08x -> 0x%08x) found for ip 0x%08x\n",
			  outFde->ipStart,
			  outFde->ipEnd,
			  ip);
	return 0;
}

int unwind_parse_cfi(
	common_information_entry_t cie, uint64_t start, uint64_t end,
	uint64_t base, uint64_t ip, cfa_rules_t initialRules, cfa_rules_t* rules)
{
	uint64_t addr = start;
	int8_t op;
	uint64_t reg, reg2, offset;
	int64_t s_offset;
	while (addr < end && base < ip && (op = unwind_dwarf_read_u8(&addr)) != DW_CFA_nop) {
		if ((op & DW_CFA_base_op_mask) == DW_CFA_advance_loc) {
			base += op & DW_CFA_base_param_mask;
			continue;
		} else if ((op & DW_CFA_base_op_mask) == DW_CFA_offset) {
			reg = op & DW_CFA_base_param_mask;
			offset = unwind_dwarf_read_uleb128(&addr);
			rules->rules[reg] = (cfa_rule_t){ .type = CFA_RULE_OFFSET, .reg = reg, .offset = offset * cie.dataAlignmentFactor };
			continue;
		} else if ((op & DW_CFA_base_op_mask) == DW_CFA_restore) {
			uint8_t reg = op & DW_CFA_base_param_mask;
			rules->rules[reg] = initialRules.rules[reg];
			continue;
		}

		switch (op) {
		case DW_CFA_set_loc:
			base = unwind_dwarf_read_uleb128(&addr);
			continue;
		case DW_CFA_advance_loc1:
			base += unwind_dwarf_read_u8(&addr);
			continue;
		case DW_CFA_advance_loc2:
			base += unwind_dwarf_read_u16(&addr);
			continue;
		case DW_CFA_advance_loc4:
			base += unwind_dwarf_read_u32(&addr);
			continue;
		case DW_CFA_def_cfa:
			reg = unwind_dwarf_read_uleb128(&addr);
			offset = unwind_dwarf_read_uleb128(&addr);
			rules->cfa = (cfa_rule_t){ .type = CFA_RULE_CFA_REGISTER_OFFSET, .reg = reg, .offset = offset };
			continue;
		case DW_CFA_def_cfa_sf:
			reg = unwind_dwarf_read_uleb128(&addr);
			s_offset = unwind_dwarf_read_sleb128(&addr);
			rules->cfa = (cfa_rule_t){ .type = CFA_RULE_CFA_REGISTER_OFFSET, .reg = reg, .offset = s_offset * cie.dataAlignmentFactor };
			continue;
		case DW_CFA_def_cfa_register:
			reg = unwind_dwarf_read_uleb128(&addr);
			rules->cfa.type = CFA_RULE_CFA_REGISTER_OFFSET;
			rules->cfa.reg = reg;
			continue;
		case DW_CFA_def_cfa_offset:
			offset = unwind_dwarf_read_uleb128(&addr);
			rules->cfa.type = CFA_RULE_CFA_REGISTER_OFFSET;
			rules->cfa.offset = offset;
			continue;
		case DW_CFA_def_cfa_offset_sf:
			s_offset = unwind_dwarf_read_sleb128(&addr);
			rules->cfa.type = CFA_RULE_CFA_REGISTER_OFFSET;
			rules->cfa.offset = s_offset * cie.dataAlignmentFactor;
			continue;
		case DW_CFA_def_cfa_expression:
			DEBUG_LOG("CFA operation %x (DW_CFA_def_cfa_expression) not supported\n", op);
			abort();
		case DW_CFA_undefined:
			reg = unwind_dwarf_read_uleb128(&addr);
			rules->rules[reg] = (cfa_rule_t){ .type = CFA_RULE_UNDEF, .reg = reg, .offset = 0 };
			continue;
		case DW_CFA_same_value:
			reg = unwind_dwarf_read_uleb128(&addr);
			rules->rules[reg] = (cfa_rule_t){ .type = CFA_RULE_SAME, .reg = reg, .offset = 0 };
			continue;
		case DW_CFA_offset_extended:
			reg = unwind_dwarf_read_uleb128(&addr);
			offset = unwind_dwarf_read_uleb128(&addr);
			rules->rules[reg].offset = offset * cie.dataAlignmentFactor;
			continue;
		case DW_CFA_offset_extended_sf:
			reg = unwind_dwarf_read_uleb128(&addr);
			s_offset = unwind_dwarf_read_sleb128(&addr);
			rules->rules[reg].offset = s_offset * cie.dataAlignmentFactor;
			continue;
		case DW_CFA_val_offset:
			reg = unwind_dwarf_read_uleb128(&addr);
			offset = unwind_dwarf_read_uleb128(&addr);
			rules->rules[reg].offset = offset * cie.dataAlignmentFactor;
			continue;
		case DW_CFA_val_offset_sf:
			reg = unwind_dwarf_read_uleb128(&addr);
			s_offset = unwind_dwarf_read_sleb128(&addr);
			rules->rules[reg].offset = s_offset * cie.dataAlignmentFactor;
			continue;
		case DW_CFA_register:
			reg = unwind_dwarf_read_uleb128(&addr);
			reg2 = unwind_dwarf_read_uleb128(&addr);
			rules->rules[reg] = (cfa_rule_t){ .type = CFA_RULE_REGISTER, .reg = reg2, .offset = 0 };
			continue;
		case DW_CFA_expression:
			DEBUG_LOG("CFA operation %x (DW_CFA_expression) not supported\n", op);
			abort();
			DEBUG_LOG("CFA operation %x (DW_CFA_val_expression) not supported\n", op);
			abort();
			reg = unwind_dwarf_read_uleb128(&addr);
			rules->rules[reg] = initialRules.rules[reg];
			continue;
		case DW_CFA_remember_state:
			DEBUG_LOG("CFA operation %x (DW_CFA_remember_state) not supported\n", op);
			abort();
		case DW_CFA_restore_state:
			DEBUG_LOG("CFA operation %x (DW_CFA_restore_state) not supported\n", op);
			abort();
		default:
			DEBUG_LOG("CFA operation %x not supported\n", op);
			abort();
		}
	}

	return 0;
}

int unwind_parse_cfa_rules(frame_description_entry_t fde, uint64_t ip, cfa_rules_t* outRules)
{
	cfa_rules_t initalRules;
	memset(&initalRules, 0, sizeof(initalRules));
	unwind_parse_cfi(
		fde.cie, fde.cie.callFrameInstrStart, fde.cie.callFrameInstrEnd,
		fde.ipStart, ip, initalRules, &initalRules);

	memcpy(outRules, &initalRules, sizeof(initalRules));
	unwind_parse_cfi(
		fde.cie, fde.callFrameInstrStart, fde.callFrameInstrEnd,
		fde.ipStart, ip, initalRules, outRules);

	return 0;
}

uint64_t unwind_apply_cfa_rule(uint64_t reg, uint64_t cfa, unwind_cursor_t cursor, cfa_rule_t rule)
{
	switch (rule.type) {
	case CFA_RULE_UNDEF:
		return 0;
	case CFA_RULE_SAME:
		if (reg >= DW_X86_64_SIZE) {
			DEBUG_LOG("CFA rule %x (CFA_RULE_SAME) parameter 'reg' is invalid: %x\n", rule.type, reg);
			abort();
		}
		return cursor.regs[reg];
	case CFA_RULE_OFFSET:
		return *(uint64_t*)(cfa + rule.offset);
	case CFA_RULE_VAL_OFFSET:
		return cfa + rule.offset;
	case CFA_RULE_REGISTER:
		if (rule.reg >= DW_X86_64_SIZE) {
			DEBUG_LOG("CFA rule %x (CFA_RULE_REGISTER) parameter 'rule.reg' is invalid: %x\n", rule.type, reg);
			abort();
		}
		return cursor.regs[rule.reg];
	case CFA_RULE_CFA_REGISTER_OFFSET:
		if (rule.reg >= DW_X86_64_SIZE) {
			DEBUG_LOG("CFA rule %x (CFA_RULE_CFA_REGISTER_OFFSET) parameter 'rule.reg' is invalid: %x\n", rule.type, reg);
			abort();
		}
		return cursor.regs[rule.reg] + rule.offset;
	case CFA_RULE_EXPR:
		DEBUG_LOG("CFA rule %x (CFA_RULE_EXPR) not supported\n", rule.type);
		abort();
	case CFA_RULE_VAL_EXPR:
		DEBUG_LOG("CFA rule %x (CFA_RULE_VAL_EXPR) not supported\n", rule.type);
		abort();
	case CFA_RULE_ARCH:
		DEBUG_LOG("CFA rule %x (CFA_RULE_ARCH) not supported\n", rule.type);
		abort();
	default:
		DEBUG_LOG("CFA rule %x not supported\n", rule.type);
		abort();
	}
}

int unwind_apply_cfa_rules(cfa_rules_t rules, unwind_cursor_t cursor, unwind_cursor_t* next_cursor)
{
	uint64_t cfa = unwind_apply_cfa_rule(-1, 0, cursor, rules.cfa);

	for (int i = 0; i < DW_X86_64_SIZE; i++) {
		next_cursor->regs[i] = unwind_apply_cfa_rule(i, cfa, cursor, rules.rules[i]);
	}
	next_cursor->regs[DW_X86_64_RSP] = cfa;

	return 0;
}

int64_t unwind_load_fde(struct _Unwind_Context* context)
{
	uint64_t ip = _Unwind_GetIP(context);

	frame_description_entry_t fde;
	memset(&fde, 0, sizeof(fde));
	unwind_get_frame_description_entry(ip, &fde);

	context->personality = fde.cie.personality;
	context->lsda = fde.lsda;
	context->ipStart = fde.ipStart;

	return ip;
}

int64_t unwind_next_step(struct _Unwind_Context* context)
{
	uint64_t ip = _Unwind_GetIP(context);

	frame_description_entry_t fde;
	memset(&fde, 0, sizeof(fde));
	unwind_get_frame_description_entry(ip, &fde);

	cfa_rules_t rules;
	memset(&rules, 0, sizeof(rules));
	unwind_parse_cfa_rules(fde, ip, &rules);

	unwind_cursor_t next_cursor;
	memset(&next_cursor, 0, sizeof(next_cursor));
	unwind_apply_cfa_rules(rules, context->cursor, &next_cursor);

	memcpy(&(context->cursor), &next_cursor, sizeof(next_cursor));

	unwind_load_fde(context);

	return context->cursor.regs[fde.cie.returnAddrReg];
}
