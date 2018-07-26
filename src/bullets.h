#ifndef __BULLETS_H__
#define __BULLETS_H__

#include <video.h>


#define BULLET_TYPE_PLAYER 0
#define BULLET_TYPE_ENEMY 1

#define BULLET_STATE_ONSCREEN 1
#define BULLET_STATE_NOTUSED 0

typedef struct
{
	// where the sprite is placed on the screen
	int x, y;
	
	// bullet tile number
	int tile;

	// either it's on screen, or it's not used yet
	int state;

	// speed of bullet
	int speedx, speedy;

	// player or enemy bullet
	int type;
} bullet_t;

void disable_bullet(int which);

//set all sprites to notused, called once at beginning
void initialize_bullets();

// update background given frames frames have passed
void update_bullets(int playerx, int playery, int *shield);

// fire new bullet, enemy or player
void fire_new_bullet(int initx, int inity, int speedx, int speedy, int type);

int bullet_sprites_onscreen();

#endif