.include "hdr.asm"

.section ".rodata1" superfree

tilfont:
.incbin "pvsneslibfont.pic"

palfont:
.incbin "pvsneslibfont.pal"

sprites_simple:
.incbin "assets/graphics/sprites/sprites_simple.pic"
sprites_simple_end:

sprites_simple_pal:
.incbin "assets/graphics/sprites/sprites_simple.pal"
sprites_simple_pal_end:

.ends