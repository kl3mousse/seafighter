/* $Id: main.c,v 1.1 2001/04/17 13:53:26 fma Exp $ */

#include <stdlib.h>
#include <video.h>
#include <input.h>

extern PALETTE	palettes[];
extern TILEMAP	mysprite;

int	main(void)
{		
	PFADE_TABLE	mytable;
	FADER		myfader;
	int			step, i;

	// Starting at sprite 2...
	set_current_sprite(2);
	
	// Set palette for textout
	setpalette(0, 2, (const PPALETTE)&palettes);
	
	// ... we write tilemap and coordinates for our sprite into video hardware
	write_sprite_data(150, 0, 15, 255, 32, 1, (const PTILEMAP)&mysprite);

	// Compute fade data
	create_fader((const PPALETTE)&palettes[1], &myfader, 1);

	// Default fade table
	mytable = &_fade_to_black;

	// Start step
	step  = 0;
	
	// Set palette for start step
	do_fade(1, &myfader, mytable, 1, step);
	
	while(1)
	{
		// Write coordinates to the bottom of the screen so we know what we are
		// doing
		textoutf(0,27, 0, 0, "STEP:%02d %s    ", step,
			(mytable == &_fade_to_black) ? "BLACK" : "WHITE");

		// Wait vertical blank
		wait_vbl();
		
		// Read joystick position from BIOS and change sprite coordinates
		// accordingly
		i = poll_joystick(PORT1, READ_BIOS_REPEAT);
		
		// Decrease step
		if ((i & JOY_A)&&(step > 0))
		{
			step--;
			do_fade(1, &myfader, mytable, 1, step);
			continue;
		}

		// Increase step
		if ((i & JOY_B)&&(step < 31))
		{
			step++;
			do_fade(1, &myfader, mytable, 1, step);
			continue;
		}
		
		// Switch to "fade to black" table
		if (i & JOY_C)
		{
			step = 0;
			mytable = &_fade_to_black;
			do_fade(1, &myfader, mytable, 1, step);
			continue;
		}
				
		// Switch to "fade to white" table
		if (i & JOY_D)
		{
			step = 0;
			mytable = &_fade_to_white;
			do_fade(1, &myfader, mytable, 1, step);
			continue;
		}
	}

	return 0;
}
