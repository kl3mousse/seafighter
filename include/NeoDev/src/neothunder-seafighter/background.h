#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include <video.h>


//////////////////
// 
//	How background sprites will be held in memory
//	A limited number of background sprites are used, so they get re-used quite often
//
//////////////////

#define BCKSPR_STATE_ONSCREEN 1
#define BCKSPR_STATE_NOTUSED 0

typedef struct
{
	// where the sprite is placed on the screen
	int y;
	float x;
	
	// background tile number
	int tile;

	// either it's on screen, or it's not used yet
	int state;
} starfield_t;

//set all sprites to notused, called once at beginning
void initialize_background();

// update background given frames frames have passed
void update_background(int frames);

int background_sprites_onscreen();
int maximum_frame_background_sprite();

#endif