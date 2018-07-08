/* $Id: task_fader.h,v 1.1 2001/05/02 14:51:40 fma Exp $ */

#ifndef __TASK_FADER_H__
#define __TASK_FADER_H__

#include <task.h>
#include <video.h>

extern void task_fader(PTASK myself, PTASK callback, PPALETTE palette,
	PFADE_TABLE fade_table, int palstart, int nb, int step, int increment);

#endif
