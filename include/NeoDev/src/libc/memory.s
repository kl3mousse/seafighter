	* ++====================================================================++
	* || memory.s - Memory handling functions for Neo Geo					||
	* ++--------------------------------------------------------------------++
	* || $Id: memory.s,v 1.3 2001/05/16 12:56:22 fma Exp $					||
	* ++--------------------------------------------------------------------++
	* || This is an assembler implementation of memcpy, memset and			||
	* || memmove. Not super optimized, but should be sufficient				||
	* ++--------------------------------------------------------------------||
	* || BGM: Chris Huelsbeck - Bionic Action								||
	* ++====================================================================++

	.globl	memcpy
	.globl	memset
	.globl	memmove

	.include "../system/helper.s"
	
*** void*	memcpy(void *dest, const void *src, size_t count)
	
	.align	4
	
memcpy:
	SAVECONTEXT	d2, 1
	move.l	_ARGS(a7),a1
	move.l	_ARGS+4(a7),a0

9:
*	If size = 0 we have nothing to do
	move.l	_ARGS+8(a7),d0
	jeq		4f

*	Check if addresses are both odd or both even
	move.l	a0,d1
	move.l	a1,d2
	and.l	#1,d1
	and.l	#1,d2

*	If not do unaligned copy
	cmp.l	d1,d2
	jne		5f
	
*	If addresses are both odd, copy one byte to make then even
	tst.b	d1
	jeq		0f
	
	move.b	(a0)+,(a1)+
	subq.l	#1,d0
	
*	Copy Bytes by block of 32
0:	move.l	d0,d1
	lsr.l	#5,d0
	jeq		2f

1:	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	move.l	(a0)+,(a1)+
	subq.l	#1,d0
	jne		1b

*	Copy remainder bytes
2:	and.l	#0x1F,d1
	jeq		4f

3:	move.b	(a0)+,(a1)+
	subq.l	#1,d1
	jne		3b
	
4:	move.l	_ARGS(a7),d0
	RESTORECONTEXT	d2, 1
	rts

5:	move.l	d0,d1
	jra		3b

	.align	4

*** void*	memmove(void *dest, const void *src, size_t count);

memmove:
	SAVECONTEXT	d2, 1
	move.l	_ARGS(a7),a1
	move.l	_ARGS+4(a7),a0

	cmp		a0,a1
	jle		9b

*	If size = 0 we have nothing to do
	move.l	_ARGS+8(a7),d0
	jeq		4f

*	We copy data from 'bottom'
	add.l	d0,a0
	add.l	d0,a1

*	Check if addresses are both odd or both even
	move.l	a0,d1
	move.l	a1,d2
	and.l	#1,d1
	and.l	#1,d2

*	If not do unaligned copy
	cmp.l	d1,d2
	jne		5f
	
*	If addresses are both odd, copy one byte to make then even
	tst.b	d1
	jeq		0f
	
	move.b	-(a0),-(a1)
	subq.l	#1,d0
	
*	Copy Bytes by block of 32
0:	move.l	d0,d1
	lsr.l	#5,d0
	jeq		2f

1:	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)
	move.l	-(a0),-(a1)
	subq.l	#1,d0
	jne		1b

*	Copy remainder bytes
2:	and.l	#0x1F,d1
	jeq		4f

3:	move.b	-(a0),-(a1)
	subq.l	#1,d1
	jne		3b
	
4:	move.l	_ARGS(a7),d0
	RESTORECONTEXT	d2, 1
	rts

5:	move.l	d0,d1
	jra		3b

	.align	4

*** void*	memset(void *dest, int ch, size_t count);

memset:
	SAVECONTEXT	d2-d3, 2
	move.l	_ARGS(a7),a0
	move.l	_ARGS+4(a7),d2
	
*	If size = 0 we have nothing to do
	move.l	_ARGS+8(a7),d0
	jeq		4f

	move.b	d2,d1
	lsl.l	#8,d2
	move.b	d1,d2
	lsl.l	#8,d2
	move.b	d1,d2
	lsl.l	#8,d2
	move.b	d1,d2

*	Check if addresses are both odd or both even
	move.l	a0,d3
	and.l	#1,d3
	jeq		0f
	
	move.b	d2,(a0)+
	subq.l	#1,d0
	
*	Set bytes by block of 32
0:	move.l	d0,d1
	lsr.l	#5,d0
	jeq		2f

1:	move.l	d2, (a0)+
	move.l	d2, (a0)+
	move.l	d2, (a0)+
	move.l	d2, (a0)+
	move.l	d2, (a0)+
	move.l	d2, (a0)+
	move.l	d2, (a0)+
	move.l	d2, (a0)+
	subq.l	#1,d0
	jne		1b

*	Set remainder bytes
2:	and.l	#0x1F,d1
	jeq		4f

3:	move.b	d2, (a0)+
	subq.l	#1,d1
	jne		3b
	
4:	move.l	_ARGS(a7),d0
	RESTORECONTEXT	d2-d3, 2
	rts
	
	.end

