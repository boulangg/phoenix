#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	int8_t unwind_dwarf_read_s8(uint64_t* addr);
	uint8_t unwind_dwarf_read_u8(uint64_t* addr);
	int16_t unwind_dwarf_read_s16(uint64_t* addr);
	uint16_t unwind_dwarf_read_u16(uint64_t* addr);
	int32_t unwind_dwarf_read_s32(uint64_t* addr);
	uint32_t unwind_dwarf_read_u32(uint64_t* addr);
	int64_t unwind_dwarf_read_s64(uint64_t* addr);
	uint64_t unwind_dwarf_read_u64(uint64_t* addr);
	uint64_t unwind_dwarf_read_sleb128(uint64_t* addr);
	uint64_t unwind_dwarf_read_uleb128(uint64_t* addr);
	uint64_t unwind_dwarf_read_encoded_pointer(uint64_t* addr, uint8_t encoding);

#ifdef __cplusplus
}
#endif
