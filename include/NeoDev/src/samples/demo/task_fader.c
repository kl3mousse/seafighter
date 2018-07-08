/* $Id: task_fader.c,v 1.1 2001/05/02 14:51:40 fma Exp $ */

#include "task_fader.h"

void task_fader(PTASK myself, PTASK callback, PPALETTE palette,
	PFADE_TABLE fade_table, int palstart, int nb, int step, int increment)
{
	int		i;
	FADER	fader[nb];

	create_fader(palette, fader, nb);
	
	for(i=0;i<32;i++)
	{
		do_fade(palstart, fader, fade_table, nb, step);
		step += increment;
		_release_timeslice();
	}
	
	if (callback != NULL)
		post_message_wait(myself, callback, MAKE_ID('J','O','B',' '),
			MAKE_ID('D','O','N','E'));
}
