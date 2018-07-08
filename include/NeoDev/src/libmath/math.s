	* ++====================================================================++
	* || math.s - Assembler fixed point routines for Neo Geo				||
	* ++--------------------------------------------------------------------++
	* || $Id: math.s,v 1.4 2001/07/27 14:41:45 fma Exp $					||
	* ++--------------------------------------------------------------------++
	* ||																	||
	* ++--------------------------------------------------------------------||
	* || BGM: Guilty Gear X - Babel Nose (Jam Kuradoberi)					||
	* ++====================================================================++

	.include "../system/helper.s"

	.globl	fmul
	.globl	fmuli
	.globl	ifmuli

	.align	4

*** FIXED fmul(FIXED a, FIXED b)

fmul:
	SAVECONTEXT		d2-d4, 3
	move.l	_ARGS(a7), d0
	move.l	_ARGS+4(a7), d1

	move.l	d0, d2
	swap	d0
	move.l	d0, d3
	mulu	d1, d0
	swap	d1
	move.l	d1, d4
	mulu	d2, d1
	add.l	d1, d0
	muls	d4, d3
	bpl.s	0f
	eor.l	#0xFFFF0000,d0
	add.l	#0x00010000,d0
0:
	swap	d3
	clr.w	d3
	add.l	d3, d0
	RESTORECONTEXT	d2-d4, 3
	rts
	
	.align	4

*** FIXED fmuli(FIXED a, WORD b)

fmuli:
	SAVECONTEXT	d2, 1
	move.l	_ARGS(a7),d0
	move.l	_ARGS+4(a7),d1
	move.w	d1,d2
	mulu	d0,d1
	swap	d0
	mulu	d0,d2
	swap	d2
	clr.w	d2
	add.l	d2,d1
	move.l	d1,d0
	RESTORECONTEXT	d2, 1
	rts

	.align	4

*** DWORD ifmuli(FIXED a, WORD b)

ifmuli:
	SAVECONTEXT	d2, 1
	move.l	_ARGS(a7),d0
	move.l	_ARGS+4(a7),d1
	move.w	d1,d2
	mulu	d0,d1
	swap	d0
	mulu	d0,d2
	swap	d2
	clr.w	d2
	add.l	d2,d1
	move.l	d1,d0
	tst.w	d0
	jpl		1f
	add.l	#0x10000,d0
1:
	clr.w	d0
	swap	d0
	RESTORECONTEXT	d2, 1
	rts
	
	.align	4
