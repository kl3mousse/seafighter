// ++===================================================================++
// || libvideo.c - Video Library for Neo Geo							||
// ++-------------------------------------------------------------------++
// || $Id: libvideo.c,v 1.4 2001/06/06 12:39:01 fma Exp $				||
// ++-------------------------------------------------------------------++
// || Provides C functions to handle palette, fix and sprites.			||
// ++-------------------------------------------------------------------||
// || BGM: <NOTHING>													||
// ++===================================================================++

//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <video.h>

//------------------------------------------------------------------------------
static const char Id[] = "libVideo Ver 0.4 Build:"__DATE__" "__TIME__;

//------------------------------------------------------------------------------
// Set to 1 by IRQ2
volatile WORD	_vbl_flag;

// Incremented by wait_vbl
volatile DWORD	_vbl_count;

// Used by write_sprite_data
WORD			_current_sprite;

//------------------------------------------------------------------------------
// NOTE: This function is a 'constructor' and therefore called at startup
void __attribute__ ((constructor)) video_init(void)
{
	// Reset VBL counter
	_vbl_count = 0;

	// Switch to palette bank 0
	set_pal_bank(0);

	// Clear all sprites
	clear_spr();

	// Clear fixed text layer
	clear_fix();
}

//------------------------------------------------------------------------------
void textoutf(int x, int y, int pal, int bank, const char *fmt, ...)
{
	char	buf[64];
	
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	
	textout(x, y, pal, bank, buf);	
}
