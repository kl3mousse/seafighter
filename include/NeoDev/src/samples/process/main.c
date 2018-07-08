/* $Id: main.c,v 1.5 2001/04/26 14:39:18 fma Exp $ */

#include <stdlib.h>
#include <video.h>
#include <task.h>
#include "task_disp.h"
#include "bouncing_block.h"

extern PALETTE	palettes[];

int	main(void)
{
	setpalette(0, 2, (const PPALETTE)&palettes);
	
	set_current_sprite(2);

	task_create(task_disp, 0x0000, MAKE_ID('T','A','S','K'),
		MAKE_ID('D','I','S','P'), 0);
	task_create(boucing_block, 0x1000, MAKE_ID('B','O','U','N'),
		MAKE_ID('C','E','0','1'), 4, 0, 100, 0, 1);
	task_create(boucing_block, 0x1000, MAKE_ID('B','O','U','N'),
		MAKE_ID('C','E','0','2'), 4, 100, 200, 70, 2);
	task_create(boucing_block, 0x1000, MAKE_ID('B','O','U','N'),
		MAKE_ID('C','E','0','3'), 4, 150, 220, 100, 1);
	task_create(boucing_block, 0x1000, MAKE_ID('B','O','U','N'),
		MAKE_ID('C','E','0','4'), 4, 200, 150, 160, 3);
	
	while(1)
	{
		task_exec();
		wait_vbl();
	}
}

