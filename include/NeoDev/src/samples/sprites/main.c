/* $Id: main.c,v 1.4 2001/07/18 14:45:58 fma Exp $ */

#include <stdlib.h>
#include <video.h>
#include <input.h>

extern PALETTE	palettes[];
extern TILEMAP	mysprite[];

int	main(void)
{		
	int		x, y;
	DWORD	xz, yz, clip, i;

	// Start coordinates for our sprite
	x = y = 0;
	xz = 15;
	yz = 255;
	clip = 32;

	// Copy our palette into video hardware, palette no 1
	setpalette(0, 2, (const PPALETTE)&palettes);

	// Starting at sprite 2...
	set_current_sprite(2);
	
	// ... we write tilemap and coordinates for our sprite into video hardware
	write_sprite_data(x, y, xz, yz, clip, 1, (const PTILEMAP)&mysprite);

	while(1)
	{
		// Wait vertical blank
		wait_vbl();
		
		// Read joystick position from BIOS and change sprite coordinates
		// accordingly
		i = poll_joystick(PORT1, READ_BIOS);
		
		if (i & JOY_UP)
			y--;
		
		if (i & JOY_DOWN)
			y++;
		
		if (i & JOY_LEFT)
			x--;
		
		if (i & JOY_RIGHT)
			x++;

		if (i & JOY_START)
		{
			x = y = 0;
			xz = 15;
			yz = 255;
			clip = 32;
		}

		if (i & JOY_B)
			yz--;
		
		if (i & JOY_C)
			yz++;
		
		// Reread joystick position, using BIOS 'auto-repeat' feature
		i = poll_joystick(PORT1, READ_BIOS_REPEAT);

		if (i & JOY_A)
			xz++;
		
		if (i & JOY_D)
			clip++;
		
		if (i & JOY_SELECT)
			clip--;
			
		// Limit values
		xz &= 15;
		yz &= 255;
		clip &= 63;
		
		// As the tilemap for our sprite never change, we just update its
		// coordinates
		change_sprite_pos(2, x, y, clip);
		change_sprite_zoom(2, xz, yz, 1);
		
		// Write coordinates to the bottom of the screen so we know what we are
		// doing
		textoutf(0,27, 0, 0, "X:%03d Y:%03d XZ:%02d YZ:%03d C:%02d    ",
			x, y, xz, yz, clip);
	}
	
	return 0;
}
