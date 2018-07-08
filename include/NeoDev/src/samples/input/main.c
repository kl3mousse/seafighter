/* $Id: main.c,v 1.4 2001/04/27 11:23:06 fma Exp $ */


#include <stdlib.h>
#include <video.h>
#include <input.h>

extern PALETTE	palettes[];

const char * const pos_names[] = {
	"UP    ",
	"DOWN  ",
	"LEFT  ",
	"RIGHT ",
	"A     ",
	"B     ",
	"C     ",
	"D     ",
	"START ",
	"SELECT"
};

static const DWORD	pos_masks[10] = {
	JOY_UP,
	JOY_DOWN,
	JOY_LEFT,
	JOY_RIGHT,
	JOY_A,
	JOY_B,
	JOY_C,
	JOY_D,
	JOY_START,
	JOY_SELECT
};

int	main(void)
{
	DWORD	direct, bios, onchange, repeat, line, i, j;

	setpalette(0, 1, (const PPALETTE)&palettes);

	textout(0, 0, 0, 0, "** INPUT TEST **");
	
	while(1)
	{
		wait_vbl();
		
		line = 2;

		for(i=0;i<2;i++)
		{
			textoutf(0, line++, 0, 0, "<PORT%d> DIRECT BIOS ONCHANGE REPEAT",
				i+1);
		
			direct = poll_joystick(i, READ_DIRECT);
			bios = poll_joystick(i, READ_BIOS);
			onchange = poll_joystick(i, READ_BIOS_CHANGE);
			repeat = poll_joystick(i, READ_BIOS_REPEAT);
	
			for(j=0;j<10;j++)
			{				
				textoutf(0, line++, 0, 0, "%s  %d      %d    %d        %d",
					pos_names[j],
					(direct & pos_masks[j])		? 1 : 0,
					(bios & pos_masks[j]) 		? 1 : 0,
					(onchange & pos_masks[j])	? 1 : 0,
					(repeat & pos_masks[j])		? 1 : 0);
			}
	
			line++;
		}
	}
}
