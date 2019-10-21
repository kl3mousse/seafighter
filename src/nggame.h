#ifndef __NGGAME_H__
#define __NGGAME_H__

#include <stdtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <input.h>
#include <DATlib.h>
#include "..\gfxout\charInclude.h" // include sprite metadata from DATlib
#include "..\gfxout\fixData.h"     // include fix metadata from DATlib


BYTE p1,p2,ps,p1e,p2e;

picture demomode_title;
picture seafighter_ship;

void ngUserModeInit();
void ngDemoModeLoop(int demo_timer, int isMVSorAES, int flash_timer, uchar mvs_demosound);
void ngTitleModeLoop(int isMVSorAES, int flash_timer, uchar title_timer, uchar dev_mode, uchar creditsP1, uchar USmode, uchar creditsP2);

#endif // __NGGAME_H__