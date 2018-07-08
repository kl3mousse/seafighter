#include <video.h>
#include "background.h"

// this determines how many hardware sprites are used for the background
// if the system needs to allocate new sprites and no hardware sprites are available,
// they will just be ignored
#define FIRST_BCK_HARDWARE_SPRITE 2
#define MAX_BCK_HARDWARE_SPRITES 50

// our background, displayed on sprites 0-49
//
// const starfield_t bcksprites[50];
// 
// Readme says this is a pitfall, and that it should be a const
// the only difference I noticed was that when const is used, warnings are generated when
// values are assigned to this array, so I'm leaving it as non-const for now
starfield_t bcksprites[MAX_BCK_HARDWARE_SPRITES];

// this determine when sprites appear on the screen, and at which y coordinate
#define BCKSPR_ENTRY_COUNT 53

const int bckspritespawn_frame[BCKSPR_ENTRY_COUNT] = {30, 50, 90, 100, 100, 110, 120, 154, 154, 170, 200, 200, 230, 280, 300, 350, 364, 400, 410, 410, 414, 424, 450, 464, 480, 550, 600, 610, 624, 680, 700, 720, 750, 800, 800, 810, 820, 830, 900, 950, 1000, 1020, 1100, 1130, 1200, 1280, 1380, 1380, 1400, 1500, 1500, 1520, 1600};
const int bckspritespawn_y[BCKSPR_ENTRY_COUNT] = {10, 30, 200, 180, 100, 16, 30, 60, 90, 180, 200, 150, 158, 40, 88, 66, 40, 108, 200, 0, 10, 50, 36, 130, 136, 22, 58, 0, 194, 160, 20, 80, 0, 200, 100, 90, 130, 140, 204, 160, 116, 150, 68, 90, 122, 104, 190, 146, 112, 20, 30, 100, 0};
const int bckspritespawn_tile[BCKSPR_ENTRY_COUNT] = {1, 2, 10, 15, 10, 13, 2, 4, 12, 11, 15, 2, 6, 1, 7, 5, 2, 2, 10, 15, 3, 7, 2, 14, 14, 9, 2, 8, 8, 1, 3, 4, 2, 2, 4, 7, 15, 15, 2, 15, 6, 2, 12, 13, 14, 15, 10, 10, 12, 15, 13, 10, 14 };

// stars, planets, and whatever else appears on the background
extern TILEMAP	starfield[];

// this dictates the maximum frame when we're still spawning a background sprite
// when we reach this, we can reset the frame counter to zero, so we cycle back to the
// beginning
// This number is basically the largest number in the bckspritespawn_frame array
const int maximum_frame_of_spawn = 1600;

void update_background(int frames)
{
	int i, j;
	
	for(i=FIRST_BCK_HARDWARE_SPRITE; i<MAX_BCK_HARDWARE_SPRITES; i++)
	{
		if(bcksprites[i].state == BCKSPR_STATE_ONSCREEN)
		{
			// first thing we do is shift all background sprites to the left by one pixel
			bcksprites[i].x -= 0.5f;
	
			// then we see if any have reached an x of -16, which means they're now fully off-screen and can be
			// freed up (or de-allocated, in a way)		
			if(bcksprites[i].x == -16.0f)
			{
				bcksprites[i].state = BCKSPR_STATE_NOTUSED;
				// move it offscreen somewhere ...
				change_sprite_pos(i, -16, 0, 1);
			}
		}
	}

	// now we check to see if we're supposed to add new sprites for this frame
	for(i=0; i<BCKSPR_ENTRY_COUNT; i++)
	{
		
		if(bckspritespawn_frame[i] == frames)
		{
			//find first unused background sprite
			for(j=FIRST_BCK_HARDWARE_SPRITE; j<MAX_BCK_HARDWARE_SPRITES; j++)
			{
				if(bcksprites[j].state == BCKSPR_STATE_NOTUSED)
				{
					// found one!
					bcksprites[j].state = BCKSPR_STATE_ONSCREEN;
					bcksprites[j].x = 319.0f;
					bcksprites[j].y = bckspritespawn_y[i];
					bcksprites[j].tile = bckspritespawn_tile[i];
					
					// write sprite data too
					set_current_sprite(j);
					write_sprite_data((int)(bcksprites[j].x), bcksprites[j].y, 15, 255, 1, 1, (const PTILEMAP)&starfield[bcksprites[j].tile]);

					// and stop searching, since we found one
					break;
				}
			}
		}
	}

	// and finally, move all active background sprites on screen
	for(i=FIRST_BCK_HARDWARE_SPRITE; i<MAX_BCK_HARDWARE_SPRITES; i++)
	{
		if(bcksprites[i].state == BCKSPR_STATE_ONSCREEN)
		{
			change_sprite_pos(i, (int)(bcksprites[i].x), bcksprites[i].y, 1);
		}
	}	
}

void initialize_background()
{
	int i;
	
	for(i=FIRST_BCK_HARDWARE_SPRITE; i<MAX_BCK_HARDWARE_SPRITES; i++)
	{
		bcksprites[i].state = BCKSPR_STATE_NOTUSED;
		bcksprites[i].x = 500.0f;
		bcksprites[i].y = 500;
		bcksprites[i].tile = 0;
	}
}

int background_sprites_onscreen()
{
	int i, count=0;
	
	for(i=FIRST_BCK_HARDWARE_SPRITE; i<MAX_BCK_HARDWARE_SPRITES; i++)
	{
		if(bcksprites[i].state == BCKSPR_STATE_ONSCREEN) count++;
	}
	return count;
}

int maximum_frame_background_sprite()
{
	return maximum_frame_of_spawn;
}