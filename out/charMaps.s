.globl test_pict
test_pict:
	.word	0x0004	;*4 bytes per strip
	.word	0x0002, 0x0001	;*2 strips of 1 tiles
	.long	test_pict_Palettes
	.long	test_pict_Map, test_pict_Map_FlipX, test_pict_Map_FlipY, test_pict_Map_FlipXY
test_pict_Map:
	.word	0x0100,0x0000
	.word	0x0101,0x0100
test_pict_Map_FlipX:
	.word	0x0101,0x0101
	.word	0x0100,0x0001
test_pict_Map_FlipY:
	.word	0x0100,0x0002
	.word	0x0101,0x0102
test_pict_Map_FlipXY:
	.word	0x0101,0x0103
	.word	0x0100,0x0003

