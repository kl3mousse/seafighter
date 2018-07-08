/* $Id: bouncing_block.c,v 1.2 2001/04/26 14:39:17 fma Exp $ */

#include <video.h>
#include <math.h>
#include "bouncing_block.h"

extern TILEMAP	mysprite[];

void boucing_block(PTASK myself, int x, int height, int angle, int step)
{
	int	sprite_num, y;
	
	sprite_num = write_sprite_data(0, 0, 15, 255, 32, 3,
		(const PTILEMAP)&mysprite);
	
	while(1)
	{
		y = 224 - ifmuli( fsin(angle), height ) - 48;
		
		change_sprite_pos(sprite_num, x, y, 32);
		
		angle += 5;
		angle &= 255;

		x += step;
		
		if (x >= (320 - 48))
		{
			x = 320 - 48;
			step = -step;
		}
		
		if (x <= 0)
		{
			x = 0;
			step = -step;
		}
		
		_release_timeslice();
	}
}
