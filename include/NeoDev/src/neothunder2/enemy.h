#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <video.h>


#define ENEMYSPR_STATE_EXPLODING 2
#define ENEMYSPR_STATE_ONSCREEN 1
#define ENEMYSPR_STATE_NOTUSED 0

typedef struct
{
	// where the sprite is placed on the screen
	int x, y;
	
	// enemy tile number
	int tile;

	// either it's on screen, or it's not used yet
	int state;

	// explosion frames left -- this counts down to 0
	int expframes;
} enemy_t;

//set all sprites to notused, called once at beginning
void initialize_enemies();

void update_enemies(int frames, int playerx, int playery, int *shield);

int enemy_sprites_onscreen();
int maximum_frame_enemy_sprite();

void disable_enemy(int which);
void enemy_open_fire(int which, int playerx, int playery);

#endif