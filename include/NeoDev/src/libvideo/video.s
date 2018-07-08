	* ++====================================================================++
	* || video.s - Assembler functions for libVideo							||
	* ++--------------------------------------------------------------------++
	* || $Id: video.s,v 1.8 2001/07/18 14:45:58 fma Exp $					||
	* ++--------------------------------------------------------------------++
	* || BGM: Favorite Blue - True Gate										||
	* ++====================================================================++

**** Exported Symbols ****************************************
	.globl	textout
	.globl	setpalette
	.globl	wait_vbl
	.globl	clear_fix
	.globl	clear_spr
	.globl	write_sprite_data
	.globl	change_sprite_pos
	.globl	change_sprite_zoom
	.globl	set_pal_bank
	.globl	erase_sprites

**** Imported Symbols ****************************************
	.globl	_vbl_flag
	.globl	_vbl_count
	.globl	_current_sprite

	.align	4

* Clear fix layer
clear_fix:
	lea		0x3C0002, a0
	move.w	#1727, d0
	move.w	#1, 2(a0)
	move.w	#0x7002, -2(a0)
0:
	move.w	#0x00FF, (a0)
	dbf		d0,0b
	rts

	.align	4

* Clear all sprites
clear_spr:
	lea		0x3C0002, a0
	moveq	#0,d1
	move.w	#1, 2(a0)
	move.w	#12287, d0
	move.w	d1, -2(a0)
0:
	move.w	d1, (a0)
	move.w	d1, (a0)
	dbf		d0, 0b

	move.w	#383, d0
	move.w	#0x8000, -2(a0)
1:
	move.w	d1, (a0)
	dbf		d0, 1b

	move.w	#383, d0
	move.w	#0x8200, -2(a0)
2:
	move.w	d1, (a0)
	dbf		d0, 2b
	
	move.w	#383, d0
	move.w	#0x8400, -2(a0)
3:
	move.w	d1, (a0)
	dbf		d0,3b
	rts

	.align	4

**** void textout(x, y, pal, bank, *text) ********************
textout:
	.set	_ARGS, 4
	lea		0x3c0002, a1
	move.w	#0x0020, 2(a1)
	move.l	_ARGS(a7), d0
	move.l	_ARGS+4(a7), d1
	addq.w	#1, d0
	addq.w	#2, d1
	lsl.w	#5, d0
	or.w	d1, d0
	or.w	#0x7000, d0
	move.w	d0, -2(a1)
	move.l	_ARGS+8(a7), d0
	move.l	_ARGS+12(a7), d1
	lsl.w	#8, d0
	lsl.w	#4, d0
	lsl.w	#8, d1
	or.w	d1, d0
	move.l	_ARGS+16(a7), a0
0:
	move.b	(a0)+, d0
	jeq		3f
	
	cmp.b	#13,d0
	jne		1f

	move.w	-2(a1),d1
	and.w	#0x701F,d1
	add.w	#0x0020, d1
	move.w	d1,-2(a1)
	jra		0b
	
1:
	cmp.b	#10,d0
	jne		2f
	
	move.w	-2(a1),d1
	addq.w	#1,d1
	move.w	d1,-2(a1)
	jra		0b
	
2:	
	move.w	d0, (a1)
	jra		0b
3:
	rts

	.align	4

**** void setpalette(palno, nb, *palette) ************************
setpalette:
	.set	_ARGS, 4
	lea		0x400000,a1
	move.l	_ARGS(a7), d0
	move.l	_ARGS+4(a7),d1
	move.l	_ARGS+8(a7),a0
	lsl.l	#5,d0
	and.l	#0x1FFF,d0
	add.l	d0,a1
0:
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	subq.l	#1,d1
	jne		0b
	rts
	
	.align	4

**** void wait_vbl(void) *************************************
wait_vbl:
	tst.w	_vbl_flag
	jeq		wait_vbl
	clr.w	_vbl_flag
	addq.l	#1, _vbl_count
	rts
	
	.align	4
	
**** WORD	write_sprite_data(int x, int y, int xz, int yz, int clipping,
****	int nb, PTILEMAP tilemap) ****************************
	
write_sprite_data:
	.set	_ARGS, 4

	lea		0x3C0002,a1

	* setup vid_modulo and vid_ptr
	move.w	#1,2(a1)
	move.w	_current_sprite,d0
	lsl.w	#6,d0
	move.w	d0,-2(a1)
	
	* copy sprite data
	move.l	_ARGS+24(a7),a0
	move.l	_ARGS+20(a7),d0
0:
	.rept	32
	move.w	(a0)+,(a1)	
	move.w	(a0)+,(a1)	
	.endr
	subq.l	#1,d0
	jne		0b

	* setup vid_ptr
	move.w	_current_sprite,d0
	add.w	#0x8000,d0
	move.w	d0,-2(a1)
	
	* concatenate xz and yz
	move.l	_ARGS+8(a7),d0
	move.l	_ARGS+12(a7),d1
	lsl.w	#8,d0
	or.w	d0,d1

	* copy reduction factors
	move.l	_ARGS+20(a7),d0
1:
	move.w	d1,(a1)
	subq.l	#1,d0
	jne		1b
	
	* setup vid_ptr
	move.w	_current_sprite,d0
	add.w	#0x8200,d0
	move.w	d0,-2(a1)

	* reverse y and concatenate to clipping
	move.l	_ARGS+4(a7),d1
	not.w	d1
	and.w	#0x1FF,d1
	sub.w	#0xF,d1
	lsl.w	#7,d1
	move.l	_ARGS+16(a7),d0
	or.w	d0,d1
	
	* copy ypos and clipping
	move.l	_ARGS+20(a7),d0
2:
	move.w	d1,(a1)
	
	* set sticky bit for following sprite strips
	or.w	#0x0040,d1
	
	subq.l	#1,d0
	jne		2b	

	* setup vid_ptr
	move.w	_current_sprite,d0
	add.w	#0x8400,d0
	move.w	d0,-2(a1)

	* setup x coord for write
	move.l	_ARGS(a7),d1
	lsl.w	#7,d1
	
	* copy x coord
	move.l	_ARGS+20(a7),d0
3:
	move.w	d1,(a1)
	subq.l	#1,d0
	jne		3b

	* increment current_sprite
	move.w	_current_sprite,d0
	move.l	_ARGS+20(a7),d1
	add.w	d0,d1
	move.w	d1,_current_sprite
	
	rts
	
	.align	4

**** void change_sprite_pos(int sprite, int x, int y, int clipping)

change_sprite_pos:
	.set	_ARGS, 4

	lea		0x3C0002,a1

	* setup vid_ptr
	move.l	_ARGS(a7),d0
	add.w	#0x8200,d0
	move.w	d0,-2(a1)

	* reverse y and concatenate to clipping
	move.l	_ARGS+8(a7),d1
	not.w	d1
	and.w	#0x1FF,d1
	sub.w	#0xF,d1
	lsl.w	#7,d1
	move.l	_ARGS+12(a7),d0
	or.w	d0,d1
	
	* copy ypos and clipping
	move.w	d1,(a1)
	
	* setup vid_ptr
	move.l	_ARGS(a7),d0
	add.w	#0x8400,d0
	move.w	d0,-2(a1)

	* setup x coord for write
	move.l	_ARGS+4(a7),d1
	lsl.w	#7,d1
	
	* copy x coord
	move.w	d1,(a1)
	
	rts

	.align	4

*** void	change_sprite_zoom(int sprite, int xz, int yz, int nb)
change_sprite_zoom:
	.set	_ARGS, 4
	
	lea		0x3C0002,a1

	* setup vid_ptr
	move.l	_ARGS(a7),d0
	add.w	#0x8000,d0
	move.w	d0,-2(a1)
	
	* concatenate xz and yz
	move.l	_ARGS+4(a7),d0
	move.l	_ARGS+8(a7),d1
	lsl.w	#8,d0
	or.w	d0,d1

	* Get number of sprite(s) to change
	move.l	_ARGS+12(a7),d0

	* copy reduction factors
0:
	move.w	d1,(a1)
	subq.l	#1,d0
	jne		0b
	rts	
	
	.align	4
	
*** void set_pal_bank(int palno)

set_pal_bank:
	.set	_ARGS, 4

	lea		0x3A000E,a0
	move.l	_ARGS(a7),d0
	and.w	#1,d0
	lsl.w	#4,d0
	move.b	d0,0(a0, d0.w)
	rts

	.align	4

*** void erase_sprites(int nb)

erase_sprites:
	.set	_ARGS, 4

	lea		0x3C0002,a1

	* setup vid_ptr
	move.w	_current_sprite,d0
	add.w	#0x8200,d0
	move.w	d0,-2(a1)

	move.l	_ARGS(a7),d0
0:
	clr.w	(a1)
	subq.l	#1,d0
	jne		0b
	rts

	.end
