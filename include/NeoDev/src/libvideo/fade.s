	* ++====================================================================++
	* || fade.s - Assembler fading routines for libVideo					||
	* ++--------------------------------------------------------------------++
	* || $Id: fade.s,v 1.1 2001/04/17 13:53:26 fma Exp $					||
	* ++--------------------------------------------------------------------++
	* || BGM: Serial Experiments Lain - Just Like A Lizard					||
	* ++====================================================================++

	.include "../system/helper.s"

**** Exported Symbols ****************************************
	.globl	create_fader
	.globl	do_fade

	.align	4

*** void create_fader(PPALETTE pal, PFADER fader, int nb)

create_fader:
	SAVECONTEXT	d2-d3, 2
	move.l	_ARGS(a7),a0
	move.l	_ARGS+4(a7),a1
	move.l	_ARGS+8(a7),d0
	
0:
	.rept	16
	move.w	(a0)+,d1
	move.w	d1,d2
	move.w	d2,d3
	and.w	#0x1000,d2
	and.w	#0x000F,d3
	lsr.w	#7,d2
	lsl.w	#6,d3
	or.w	d3,d2
	move.w	d2,(a1)+

	move.w	d1,d2
	move.w	d2,d3
	and.w	#0x2000,d2
	and.w	#0x00F0,d3
	lsr.w	#8,d2
	lsl.w	#2,d3
	or.w	d3,d2
	move.w	d2,(a1)+

	move.w	d1,d2
	and.w	#0x4000,d1
	and.w	#0x0F00,d2
	lsr.w	#8,d1
	lsr.w	#1,d1
	lsr.w	#2,d2
	or.w	d2,d1
	move.w	d1,(a1)+

	.endr
	
	subq.l	#1,d0
	jne		0b
	
	RESTORECONTEXT	d2-d3, 2
	rts

	.align	4

*** void do_fade(int palstart, PFADER fader, PFADE_TABLE table, int nb,
***		int step)

do_fade:
	SAVECONTEXT	d2-d5/a2, 5
	lea		0x400000,a0
	move.l	_ARGS(a7),d0
	lsl.l	#5,d0
	add.l	d0,a0
	move.l	_ARGS+4(a7),a1
	move.l	_ARGS+8(a7),a2
	move.l	_ARGS+12(a7),d0
	move.l	_ARGS+16(a7),d1
	
0:
	.rept	16
	move.w	(a1)+,d2
	add.w	d1,d2
	move.b	0(a2,d2.w),d2
	move.b	d2,d3
	and.w	#0x0001,d2
	and.w	#0x001E,d3
	lsl.w	#8,d2
	lsl.w	#4,d2
	lsr.w	#1,d3
	or.w	d3,d2

	move.w	(a1)+,d3
	add.w	d1,d3
	move.b	0(a2,d3.w),d3
	move.b	d3,d4
	and.w	#0x0001,d3
	and.w	#0x001E,d4
	lsl.w	#8,d3
	lsl.w	#5,d3
	lsl.w	#3,d4
	or.w	d4,d3
	
	move.w	(a1)+,d4
	add.w	d1,d4
	move.b	0(a2,d4.w),d4
	move.b	d4,d5
	and.w	#0x0001,d4
	and.w	#0x001E,d5
	lsl.w	#8,d4
	lsl.w	#6,d4
	lsl.w	#7,d5
	or.w	d5,d4
	
	or.w	d3,d2
	or.w	d4,d2
	move.w	d2,(a0)+
	.endr	
	
	subq.l	#1,d0
	jne		0b
	
	RESTORECONTEXT	d2-d5/a2, 5
	rts
	
	.end
	