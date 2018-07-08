	* ++====================================================================++
	* || helper.s - Some Helper macros to deal with arguments				||
	* ++--------------------------------------------------------------------++
	* || $Id: helper.s,v 1.2 2001/04/13 10:05:57 fma Exp $					||
	* ++--------------------------------------------------------------------++
	* || BGM: <NOTHING>														||
	* ++====================================================================++

	.macro	SAVECONTEXT	regs nbregs
		.ifeq	\nbregs-1
			move.l	\regs,-(a7)
		.else
			movem.l \regs,-(a7)
		.endif
		
		.set	_ARGS, (\nbregs+1)*4
	.endm

	.macro	RESTORECONTEXT	regs nbregs
		.ifeq	\nbregs-1
			move.l	(a7)+, \regs
		.else
			movem.l (a7)+, \regs
		.endif
	.endm

