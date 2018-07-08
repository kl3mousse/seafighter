/* $Id: task_scroll.h,v 1.1 2001/05/03 13:43:42 fma Exp $ */

#ifndef __TASK_SCROLL_H__
#define __TASK_SCROLL_H__

#include <task.h>
#include <video.h>

extern void task_scroll(PTASK myself, char *message, int sprite_start,
	int y, int speed);

#endif
