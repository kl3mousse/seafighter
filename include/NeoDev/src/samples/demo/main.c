/* $Id: main.c,v 1.9 2001/06/15 12:15:19 fma Exp $ */

#include <stdlib.h>
#include <video.h>
#include <task.h>
#include "task_disp.h"
#include "task_scroll.h"
#include "task_fader.h"
#include "task_luciola.h"

#define __DEBUG__

extern PALETTE	palettes[];
extern PTILEMAP	map_border[];

static const char* const	scroll_message = "supremacy is proud to present the first neogeo demo ever. done for the v.i.p 2 party in st priest near lyon. this is a very simple demo with only two screens but nothing was prepared before the party so i didn't have much time to do better things. hope you enjoy it see you later !!!                 time to wrap                ";

void task_sequencer(PTASK myself);

int	main(void)
{
	setpalette(0, 11, (const PPALETTE)&palettes);
	
	task_create(task_sequencer, 0x0000, MAKE_ID('M','A','I','N'),
		MAKE_ID(' ','S','E','Q'), 0);

#ifdef __DEBUG__
	task_create(task_disp, 0xFFFF, MAKE_ID('T','A','S','K'),
		MAKE_ID('D','I','S','P'), 0);
#endif

	task_create(task_scroll, 0xF000, MAKE_ID('S','C','R','O'),
		MAKE_ID('L','L','0','1'), 4, scroll_message, 339, 192, 1);

	task_create(task_scroll, 0xF000, MAKE_ID('S','C','R','O'),
		MAKE_ID('L','L','0','2'), 4, scroll_message, 361, 0, 1);
		
	set_current_sprite(320);
	write_sprite_data(8, 0, 15, 255, 32, 19, (const PTILEMAP)&map_border);
	
	while(1)
	{
		set_current_sprite(2);
		task_exec();
		wait_vbl();
	}
}

void task_sequencer(PTASK myself)
{
	PTASK	a, b, c, d, e;
	
	while(1)
	{
		a = task_create(task_luciola_small, 0x0001, MAKE_ID('L','U','C','I'),
			MAKE_ID('O','L','A','S'), 0);

		b = task_create(task_luciola_big, 0x0003, MAKE_ID('L','U','C','I'),
			MAKE_ID('O','L','A','B'), 0);

		c = task_create(task_bgdisp, 0x0002, MAKE_ID('B','K','G','N'),
			MAKE_ID('D',' ',' ',' '), 0);
	
		d = task_create(task_fader, 0xFFF0, MAKE_ID('F','A','D','E'),
			MAKE_ID(' ','I','N',' '), 7, myself, &palettes[2], &_fade_to_black,
			2, 3, 31, -1);
	
		do
		{
			read_message_wait(myself, &e, NULL, NULL);
		} while (d != e);
	
		task_sleep(60);
	
		d = task_create(task_fader, 0xFFF0, MAKE_ID('F','A','D','E'),
			MAKE_ID(' ','O','U','T'), 7, myself, &palettes[2], &_fade_to_black,
			2, 3, 0, 1);
	
		do
		{
			read_message_wait(myself, &e, NULL, NULL);
		} while (d != e);
	
		task_kill(a);
		task_kill(b);
		task_kill(c);
	
		task_sleep(60);
	}
}
