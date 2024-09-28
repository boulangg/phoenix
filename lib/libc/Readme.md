# Libc

Based on N3301 (C latest public working draft on 2024-07-28)

## Support

| Header            | Standard              | Support  | Note                       |
|-------------------|-----------------------|----------|----------------------------|
| **assert.h**      | **ANSI (89)**         | **Full** | **-**                      |
| complex.h         | C99                   | None     |                            |
| **ctype.h**       | **ANSI (89)**         | **Full** | **-**                      |
| **errno.h**       | **ANSI (89) - POSIX** | **Full** | **with POSIX extension**   |
| fenv.h            | C99                   | None     |                            |
| float.h           | ANSI (89)             | None     |                            |
| **inttypes.h**    | **C99**               | **Full** | **-**                      |
| **iso646.h**      | **C95**               | **Full** | **-**                      |
| **limits.h**      | **ANSI (89)**         | **Full** | **-**                      |
| locale.h          | ANSI (89)             | None     |                            |
| math.h            | ANSI (89)             | None     |                            |
| setjmp.h          | ANSI (89)             | None     |                            |
| signal.h          | ANSI (89)             | None     |                            |
| **stdalign.h**    | **C11**               | **N/A**  | **No content**             |
| **stdarg.h**      | **ANSI (89)**         | **Full** | **-**                      |
| stdatomic.h       | C11                   | Partial  | Not tested                 |
| **stdbit.h**      | **C23**               | **Full** | **Tested**                 |
| **stdbool.h**     | **C99**               | **Full** | **-**                      |
| stdckdint         | C23                   | None     |                            |
| **stddef.h**      | **ANSI (89)**         | **Full** | Missing nullptr_t def.     |
| **stdint.h**      | **C99**               | **Full** | **-**                      |
| stdio.h           | ANSI (89)             | Partial  |                            |
| stdlib.h          | ANSI (89)             | Partial  |                            |
| **stdnoreturn.h** | **C11**               | **N/A**  | **Obsolete**               |
| string.h          | ANSI (89)             | Partial  |                            |
| tgmath.h          | C99                   | None     |                            |
| threads.h         | C11                   | None     |                            |
| time.h            | ANSI (89)             | Partial  |                            |
| uchar.h           | C11                   | None     |                            |
| whar.h            | C95                   | Minimal  |                            |
| wctype.h          | C95                   | None     |                            |
