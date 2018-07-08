	* ++====================================================================++
	* || task_asm.s - Task <-> System Context Switching						||
	* ++--------------------------------------------------------------------++
	* || $Id: task_asm.s,v 1.1 2001/04/19 14:45:34 fma Exp $				||
	* ++--------------------------------------------------------------------++
	* || BGM: KOF95 - Yagami Stage											||
	* ++====================================================================++

	.globl	_system_context
	.globl	_current_task
	.globl	_save_reg

	.globl	_exec_task
	.globl	_release_timeslice

	.text
	
	.align	4
	
_exec_task:

	move.w	sr,-(a7)
	movem.l	d0-d7/a0-a7,_system_context
	
	move.l	_current_task,a0
	move.l	(a0)+,a7
	move.l	(a0)+,-(a7)
	move.w	(a0)+,-(a7)
	movem.l	(a0),d0-d7/a0-a6
	rte
	
	.align	4
	
_release_timeslice:
	move.l	a0, _save_reg
	move.l	_current_task,a0
	move.l	(a7)+,4(a0)
	move.l	a7,(a0)
	lea		8(a0),a7
	move.w	sr,(a7)+
	move.l	_save_reg,a0
	movem.l	d0-d7/a0-a6,(a7)

	movem.l	_system_context, d0-d7/a0-a7
	rte

	.end
