// ++===================================================================++
// || libinput.c - Input devices reading for Neo Geo					||
// ++-------------------------------------------------------------------++
// || $Id: libinput.c,v 1.3 2001/04/13 13:51:36 fma Exp $				||
// ++-------------------------------------------------------------------++
// || BGM: <NOTHING>													||
// ++===================================================================++

//------------------------------------------------------------------------------
#include <stdio.h>
#include <input.h>

//------------------------------------------------------------------------------
static const char Id[] = "libInput Ver 0.8 Build:"__DATE__" "__TIME__;
//------------------------------------------------------------------------------
DWORD	poll_joystick(DWORD port, DWORD flags)
{
	DWORD	data1 = 0, data2 = 0;
	
	switch(port)
	{
	case	PORT1:
		switch(flags)
		{
		case	READ_DIRECT:
			data1 = (*((PBYTE)0x300000))^0xFF;
			data2 = (*((PBYTE)0x380000))^0xFF;
			break;

		case	READ_BIOS:
			data1 = *((PBYTE)0x10FD95);
			data2 = *((PBYTE)0x10FDAC);
			break;

		case	READ_BIOS_CHANGE:
			data1 = *((PBYTE)0x10FD97);
			data2 = *((PBYTE)0x10FDAD);
			break;

		case	READ_BIOS_REPEAT:
			data1 = *((PBYTE)0x10FD98);
			data2 = *((PBYTE)0x10FDAD);
			break;
		}	
		break;

	case	PORT2:
		switch(flags)
		{
		case	READ_DIRECT:
			data1 = (*((PBYTE)0x340000))^0xFF;
			data2 = ((*((PBYTE)0x380000))^0xFF)>>2;
			break;

		case	READ_BIOS:
			data1 = *((PBYTE)0x10FD9B);
			data2 = (*((PBYTE)0x10FDAC))>>2;
			break;

		case	READ_BIOS_CHANGE:
			data1 = *((PBYTE)0x10FD9D);
			data2 = (*((PBYTE)0x10FDAD))>>2;
			break;

		case	READ_BIOS_REPEAT:
			data1 = *((PBYTE)0x10FD9E);
			data2 = (*((PBYTE)0x10FDAD))>>2;
			break;
		}	
		break;
	}
	
	return (data2<<8)|data1;
}

