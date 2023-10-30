.section .rodata

.global fonts_start
fonts_start:

.global base_font
base_font:
    .incbin "font.psf"
.global base_font_end
base_font_end:

base_font_size:
    .quad base_font_end - base_font

.global _fonts_end
fonts_end:
