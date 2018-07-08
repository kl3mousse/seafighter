// ++===================================================================++
// ||            stdlib.c - C Runtime functions for Neo Geo				||
// ++-------------------------------------------------------------------++
// || $Id: stdlib.c,v 1.5 2001/07/13 14:02:58 fma Exp $				    ||
// ++-------------------------------------------------------------------++
// || Provides standard C functions.									||
// || This is far from being a complete implementation of libc, only	||
// || often used stuff is here.											||
// ++-------------------------------------------------------------------||
// || BGM: Guitar Vader - Persecution Mania								||
// ++===================================================================++

//------------------------------------------------------------------------------
#include <stdtypes.h>
#include <stdlib.h>
//------------------------------------------------------------------------------
static const char	Id[] = "libC Ver 0.4 Build:"__DATE__" "__TIME__;
char				*___strtok;
BYTE				*_sbrk_base1;
BYTE				*_sbrk_base2;
//------------------------------------------------------------------------------
void* sbrk(DWORD increment)
{
	BYTE	*base;
	
	if ((DWORD)(_sbrk_base1 + increment) > 0x100000)
	{
		if ((DWORD)(_sbrk_base2 + increment) > 0x200000)
			return NULL;

		base = _sbrk_base2;
		_sbrk_base2 += (increment/4)*4 + 4;
		return base;
	}
	
	base = _sbrk_base1;
	_sbrk_base1 += (increment/4)*4 + 4;
	return base;
}
