#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t (*syscall64_t)(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f);

uint64_t syscall64(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t num);

extern syscall64_t syscall64_table[256];

#ifdef __cplusplus
}
#endif




