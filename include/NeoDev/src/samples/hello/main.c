/* $Id: main.c,v 1.3 2001/04/17 07:59:54 fma Exp $ */

#include <stdlib.h>
#include <video.h>

extern PALETTE	palettes;

int	main(void)
{		
	setpalette(0, 1, (const PPALETTE)&palettes);
	textout(0, 0, 0, 0, "Hello, world !");
	
	while(1)
	{
		wait_vbl();
		textoutf(0, 2, 0, 0, "Vbl Counter:%d", _vbl_count);
	}
}
