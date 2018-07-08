/* $Id: main.c,v 1.3 2001/04/17 07:59:54 fma Exp $ */

#include <stdlib.h>
#include <video.h>
#include <math.h>

extern PALETTE	palettes[];
extern TILEMAP	mysprite[];

int	main(void)
{		
	int		x, y;
	DWORD	angle;

	angle = 0;
	
	// Copy our palette into video hardware, palette no 1
	setpalette(0, 2, (const PPALETTE)&palettes);

	// Starting at sprite 2...
	set_current_sprite(2);
	
	// ... we write tilemap and coordinates for our sprite into video hardware
	write_sprite_data(0, 0, 15, 255, 32, 3, (const PTILEMAP)&mysprite);

	while(1)
	{
		// Wait vertical blank
		wait_vbl();
			
		// calculate coordinates
		x = ifmuli( fcos(angle), 96 ) + 150;
		y = ifmuli( fsin(angle), 96 ) + 88;

		// As the tilemap for our sprite never change, we just update its
		// coordinates
		change_sprite_pos(2, x, y, 32);
		
		// Calculate next angle
		angle++;
		angle &= 511;
	}
	
	return 0;
}
