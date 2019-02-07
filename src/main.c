/***********************************************************************************
		SEA FIGHTER
		a clone of NEO THUNDER (Sebastian Mihai, 2011 (http://www.sebastianmihai.com))
		"Long live the Neogeo!"
***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <DATlib.h>
#include <input.h>

// pallete information
// extern PALETTE	palettes[];
// extern TILEMAP	playership[];

#define NB_SHIP_TYPES 3 //number of types of ships. Must match with sprites.

struct game
  {
     int nbPlayers; // 1 or 2 players
     int mode;      // 0 = Logo screen(s)
                    // 1 = Main Menu
                    // 2 = Attract / demo mode
                    // 3 = Playing
                    // 4 = Game over
  };

// GLOBALS
DWORD i; //joy1
DWORD joy2;
struct game seaFighter;
int p1ship_type; // 0, 1, 2
int p1shipSpriteAddress;
int p2ship_type; // 0, 1, 2
int p2shipSpriteAddress;

int lastscore;


int	main(void)
{
  char* author = "kl3mousse, 2018";

  struct game seaFighter;

  seaFighter.mode = 0;
	p1ship_type = 0; //0 or 1 or 2
  p2ship_type = 1; //0 or 1 or 2


  initGfx();SCClose();
 while(1){waitVBlank();SCClose();}

	return 0;
}
