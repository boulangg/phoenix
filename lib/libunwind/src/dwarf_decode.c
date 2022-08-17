#include "dwarf_decode.h"

#include "exception_frame.h"
#include "debug.h"
#include <stdlib.h>

int8_t unwind_dwarf_read_s8(uint64_t* addr)
{
	int8_t val = *(int8_t*)*addr;
	*addr += sizeof(int8_t);
	return val;
}

uint8_t unwind_dwarf_read_u8(uint64_t* addr)
{
	uint8_t val = *(uint8_t*)*addr;
	*addr += sizeof(uint8_t);
	return val;
}

int16_t unwind_dwarf_read_s16(uint64_t* addr)
{
	int16_t val = *(int16_t*)*addr;
	*addr += sizeof(int16_t);
	return val;
}

uint16_t unwind_dwarf_read_u16(uint64_t* addr)
{
	uint16_t val = *(uint16_t*)*addr;
	*addr += sizeof(uint16_t);
	return val;
}

int32_t unwind_dwarf_read_s32(uint64_t* addr)
{
	int32_t val = *(int32_t*)*addr;
	*addr += sizeof(int32_t);
	return val;
}

uint32_t unwind_dwarf_read_u32(uint64_t* addr)
{
	uint32_t val = *(uint32_t*)*addr;
	*addr += sizeof(uint32_t);
	return val;
}

int64_t unwind_dwarf_read_s64(uint64_t* addr)
{
	int64_t val = *(int64_t*)*addr;
	*addr += sizeof(int64_t);
	return val;
}

uint64_t unwind_dwarf_read_u64(uint64_t* addr)
{
	uint64_t val = (*(uint64_t*)*addr);
	*addr += sizeof(uint64_t);
	return val;
}

uint64_t unwind_dwarf_read_sleb128(uint64_t* addr)
{
	int64_t result = 0;
	uint8_t shift = 0;
	uint8_t byte = 0;
	do {
		byte = unwind_dwarf_read_u8(addr);
		result |= ((byte & 0x7f) << shift);
		shift += 7;
	} while ((byte & 0x80) != 0);

	if ((shift < sizeof(int64_t) * 8) && ((byte & 0x40) != 0)) {
		/* sign extend */
		result |= -(((int64_t)1) << shift);
	}
	return result;
}

uint64_t unwind_dwarf_read_uleb128(uint64_t* addr)
{
	uint64_t result = 0;
	uint8_t shift = 0;
	uint8_t byte = 0;
	do {
		byte = unwind_dwarf_read_u8(addr);
		result |= ((byte & 0x7f) << shift);
		shift += 7;
	} while ((byte & 0x80) != 0);
	return result;
}

uint64_t unwind_dwarf_read_encoded_pointer(uint64_t* addr, uint8_t encoding)
{
	if (encoding == DW_EH_PE_omit) {
		return 0;
	}

	uint64_t inital_addr = *addr;

	uint64_t ptr = 0;
	uint8_t format = encoding & DW_EH_PE_format_mask;
	switch (format) {
	case DW_EH_PE_absptr:
		ptr = (uint64_t)unwind_dwarf_read_u64(addr);
		break;
	case DW_EH_PE_uleb128:
		ptr = (uint64_t)unwind_dwarf_read_uleb128(addr);
		break;
	case DW_EH_PE_udata2:
		ptr = (uint64_t)unwind_dwarf_read_u16(addr);
		break;
	case DW_EH_PE_udata4:
		ptr = (uint64_t)unwind_dwarf_read_u32(addr);
		break;
	case DW_EH_PE_udata8:
		ptr = (uint64_t)unwind_dwarf_read_u64(addr);
		break;
	case DW_EH_PE_sleb128:
		ptr = (uint64_t)unwind_dwarf_read_sleb128(addr);
		break;
	case DW_EH_PE_sdata2:
		ptr = (uint64_t)unwind_dwarf_read_s16(addr);
		break;
	case DW_EH_PE_sdata4:
		ptr = (uint64_t)unwind_dwarf_read_s32(addr);
		break;
	case DW_EH_PE_sdata8:
		ptr = (uint64_t)unwind_dwarf_read_s64(addr);
		break;
	default:
		DEBUG_LOG("Format type %i not supported", format);
		abort();
	}

	uint8_t appl = encoding & DW_EH_PE_appl_mask;
	switch (appl) {
	case DW_EH_PE_absptr:
		break;
	case DW_EH_PE_pcrel:
		ptr += inital_addr;
		break;
	case DW_EH_PE_textrel:
	case DW_EH_PE_datarel:
	case DW_EH_PE_funcrel:
	case DW_EH_PE_aligned:
	default:
		DEBUG_LOG("Application type %i not supported", appl);
		abort();
	}

	if ((encoding & DW_EH_PE_indirect) == DW_EH_PE_indirect) {
		return *(uint64_t*)ptr;
	}

	return ptr;
}
