/* # $Id: input.h,v 1.4 2001/07/13 14:02:57 fma Exp $ */

#ifndef __INPUT_H__
#define __INPUT_H__

//-- Includes -----------------------------------------------------------------
#include <stdtypes.h>

#ifdef __cplusplus
	extern "C" {
#endif

//-- Defines ------------------------------------------------------------------
// Port definitions
#define PORT1				0
#define	PORT2				1

// Flags for poll_joystick
#define READ_DIRECT			0
#define	READ_BIOS			1
#define READ_BIOS_CHANGE	2
#define READ_BIOS_REPEAT	3

// Positions : sSDCBARLDU
#define JOY_UP				0x0001
#define JOY_DOWN			0x0002
#define JOY_LEFT			0x0004
#define JOY_RIGHT			0x0008
#define JOY_A				0x0010
#define JOY_B				0x0020
#define JOY_C				0x0040
#define JOY_D				0x0080
#define JOY_START			0x0100
#define JOY_SELECT			0x0200

//-- Exported Functions -------------------------------------------------------
extern DWORD	poll_joystick(DWORD port, DWORD flags);

#ifdef __cplusplus
	}
#endif

#endif // __INPUT_H__
