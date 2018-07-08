// ++===================================================================++
// ||           libcpp.c - C++ Runtime functions for Neo Geo			||
// ++-------------------------------------------------------------------++
// || $Id: libcpp.c,v 1.4 2001/07/27 14:41:45 fma Exp $				    ||
// ++-------------------------------------------------------------------++
// || Provides needed functions for C++.								||
// ++-------------------------------------------------------------------||
// || BGM: Guitar Vader - Persecution Mania								||
// ++===================================================================++

//------------------------------------------------------------------------------
#ifndef __cd__
#define __cd__
#endif
//------------------------------------------------------------------------------
#include <stdtypes.h>
#include <stdlib.h>
#include <video.h>
//------------------------------------------------------------------------------
static const char	Id[] = "libC++ Ver 0.2 Build:"__DATE__" "__TIME__;

const PALETTE	_system_palette = {
	0x0000, 0x7fff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
//------------------------------------------------------------------------------
void*	__builtin_new(size_t size)
{
	return malloc(size);
}
//------------------------------------------------------------------------------
void	__builtin_delete(void *ptr)
{
	free(ptr);
}
//------------------------------------------------------------------------------
void __NORETURN__ abort(void)
{
	setpalette(0, 1, (const PPALETTE)&_system_palette);
	clear_fix();
	clear_spr();
	textout(0, 0, 0, 0, "abort() has been called.");
	while(1);
}
//------------------------------------------------------------------------------
void __NORETURN__ terminate__Fv(void)
{
	setpalette(0, 1, (const PPALETTE)&_system_palette);
	clear_fix();
	clear_spr();
	textout(0, 0, 0, 0, "terminate() has been called.");
	while(1);
}
