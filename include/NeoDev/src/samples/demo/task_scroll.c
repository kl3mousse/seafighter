/* $Id: task_scroll.c,v 1.3 2001/06/15 12:15:19 fma Exp $ */

#include "task_scroll.h"

extern TILEMAP	map_font[];
extern BYTE		scroll_charmap[46];

void task_scroll(PTASK myself, char *message, int sprite_start, int y, 
	int speed)
{
	char	*msg_ptr, *tmp_ptr;
	int		i, j;
	int		x;
	
	msg_ptr = message - 1;
	x = -32;
	
	while(1)
	{
		if (x <= -32)
		{
			x += 32;
			msg_ptr++;

			if (*msg_ptr == 0)
				msg_ptr = message;
			
			tmp_ptr = msg_ptr;
			set_current_sprite(sprite_start);
			
			for(i=0;i<11;i++)
			{
				if (*tmp_ptr == 0)
					tmp_ptr = message;
				
				for(j=0;j<46;j++)
				{
					if (*tmp_ptr == scroll_charmap[j])
						break;
				}
				
				write_sprite_data(x + (i<<5), y, 15, 255, 32, 2,
					(const PTILEMAP)&map_font[j*2]);
			
				tmp_ptr++;
			}
		}
		else
		{
			for(i=0;i<11;i++)
				change_sprite_pos((i<<1)+sprite_start, x+(i<<5), y, 32);				
		}
		
		x -= speed;

		_release_timeslice();
	}
}
