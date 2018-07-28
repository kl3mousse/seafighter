/*

		SEA FIGHTER
		a clone of NEO THUNDER (Sebastian Mihai, 2011 (http://www.sebastianmihai.com))

		"Long live the Neogeo!"
*/

#include <stdlib.h>
#include <video.h>
#include <input.h>
#include "background.h"
#include "bullets.h"
#include "enemy.h"

// pallete information
extern PALETTE	palettes[];

extern TILEMAP	playership[];

// used to read in the joystick
DWORD i;
DWORD joy2;

int p1ship; // 0, 1, 2
int lastscore;

void render_playership(int x, int y, int playermoving)
{
	// 200: sprite for back of playership
	// 201: sprite for front of playership
	// SHIP 1
	// playership[0] : back, no flame
	// playership[1] : back, small flame
	// playership[2] : back, big flame
	// playership[3] : front, standard
	// playership[4] : front, reverse turbo
	// SHIP 2
	// playership[5] : back, no flame
	// playership[6] : back, small flame
	// playership[7] : back, big flame
	// playership[8] : front, standard
	// playership[9] : front, reverse turbo

  int p1shipSpriteAddress = p1ship * 5;

	set_current_sprite(201);
	write_sprite_data(x+16, y, 15, 255, 1, 1, (const PTILEMAP)&playership[3 + p1shipSpriteAddress]);

	set_current_sprite(200);
	if(playermoving == 1)
	{
		if (joy2 & JOY_LEFT)
		{
			write_sprite_data(x, y, 15, 255, 1, 1, (const PTILEMAP)&playership[0 + p1shipSpriteAddress]);
			set_current_sprite(201);
			if(_vbl_count % 2 == 0)
				write_sprite_data(x+16, y, 15, 255, 1, 1, (const PTILEMAP)&playership[3 + p1shipSpriteAddress]);
			else
				write_sprite_data(x+16, y, 15, 255, 1, 1, (const PTILEMAP)&playership[4 + p1shipSpriteAddress]);
		}
		else
		{
			if(_vbl_count % 2 == 0)
				write_sprite_data(x, y, 15, 255, 1, 1, (const PTILEMAP)&playership[2 + p1shipSpriteAddress]);
			else
				write_sprite_data(x, y, 15, 255, 1, 1, (const PTILEMAP)&playership[0 + p1shipSpriteAddress]);
		}
	}
	else
	{

		if(_vbl_count % 2 == 0)
			write_sprite_data(x, y, 15, 255, 1, 1, (const PTILEMAP)&playership[1 + p1shipSpriteAddress]);
		else
			write_sprite_data(x, y, 15, 255, 1, 1, (const PTILEMAP)&playership[0 + p1shipSpriteAddress]);
	}

	//change_sprite_pos(200, x, y, 1);

	change_sprite_pos(201, x+16, y, 1);
}

void menu()
{
		setpalette(0, 2, (const PPALETTE)&palettes);
		clear_fix();
		clear_spr();
		_vbl_count = 0;
		do
		{
			i = poll_joystick(PORT1, READ_DIRECT);
			textoutf(9,12, 0, 0, "Sea Fighter 0.01.006");
			textoutf(5,15, 0, 0, "a NEOTHUNDER clone by kl3mousse");
			textoutf(11,18, 0, 0, "Press B to start!");

			if(lastscore >= 31)
				textoutf(0,27, 0, 0, "Last attempt: COMPLETED GAME!!");
			else
				textoutf(0,27, 0, 0, "Last attempt: %d light years travelled", lastscore);

			wait_vbl();

			set_current_sprite(379);
			if(_vbl_count % 2 == 0)
				write_sprite_data(132, 160, 15, 255, 1, 1, (const PTILEMAP)&playership[0]);
			else
				write_sprite_data(132, 160, 15, 255, 1, 1, (const PTILEMAP)&playership[2]);
			set_current_sprite(380);
			write_sprite_data(148, 160, 15, 255, 1, 1, (const PTILEMAP)&playership[3]);

		}while(!(i & JOY_B));

		_vbl_count = 0;
		clear_fix();
		clear_spr();
}

void selectPlayerShip()
{
		setpalette(0, 2, (const PPALETTE)&palettes);
		clear_fix();
		clear_spr();
		_vbl_count = 0;
		do
		{
			i = poll_joystick(PORT1, READ_DIRECT);
			textoutf(13,12, 0, 0, "Select your ship!!");
			textoutf(11,18, 0, 0, "Press A to start!");



			set_current_sprite(279);
			if(_vbl_count % 2 == 0)
				write_sprite_data(64, 160, 15, 255, 1, 1, (const PTILEMAP)&playership[0]);
			else
				write_sprite_data(64, 160, 15, 255, 1, 1, (const PTILEMAP)&playership[2]);
			set_current_sprite(280);
			write_sprite_data(80, 160, 15, 255, 1, 1, (const PTILEMAP)&playership[3]);

			set_current_sprite(281);
			if(_vbl_count % 2 == 0)
				write_sprite_data(128, 160, 15, 255, 1, 1, (const PTILEMAP)&playership[0+5]);
			else
				write_sprite_data(128, 160, 15, 255, 1, 1, (const PTILEMAP)&playership[2+5]);
			set_current_sprite(282);
			write_sprite_data(144, 160, 15, 255, 1, 1, (const PTILEMAP)&playership[3+5]);

			set_current_sprite(283);
			if(_vbl_count % 2 == 0)
				write_sprite_data(196, 160, 15, 255, 1, 1, (const PTILEMAP)&playership[0+10]);
			else
				write_sprite_data(196, 160, 15, 255, 1, 1, (const PTILEMAP)&playership[2+10]);
			set_current_sprite(284);
			write_sprite_data(212, 160, 15, 255, 1, 1, (const PTILEMAP)&playership[3+10]);

wait_vbl();
		}while(!(i & JOY_A));

		_vbl_count = 0;
		clear_fix();
		clear_spr();
}

void get_ready()
{
		setpalette(0, 2, (const PPALETTE)&palettes);
		clear_fix();
		clear_spr();
		_vbl_count = 0;
		do
		{
			textoutf(13,12, 0, 0, "Get ready my friend!");
			textoutf(17,13, 0, 0, "%d", 5 - _vbl_count/40);
			wait_vbl();

		}while(_vbl_count < 200);

		_vbl_count = 0;
		clear_fix();
		clear_spr();
}

void game_over()
{
		setpalette(0, 2, (const PPALETTE)&palettes);
		clear_fix();
		clear_spr();
		_vbl_count = 0;
		do
		{
			textoutf(13,12, 0, 0, "Game over");
			textoutf(6,16, 0, 0, "You travelled %d light years", lastscore);
			textoutf(9,17, 0, 0, "before you got owned");
			wait_vbl();

		}while(_vbl_count < 300);

		_vbl_count = 0;
		clear_fix();
		clear_spr();
}

void game()
{
	int playershield = 10;
	int x, y;
	int current_background_frame = 0;
	int current_enemy_frame = 0;
	int playermoving = 0;
	int playerspeed = 1;
	char c = 177;
	int count = 0;
	int stage = 1;

	// Start coordinates for our sprite
	x = 0;
	y = 100;
	// Copy our palette into video hardware, palette no 1
	setpalette(0, 2, (const PPALETTE)&palettes);

	//==============

	set_current_sprite(200);
	write_sprite_data(0, 100, 15, 255, 1, 1, (const PTILEMAP)&playership[2]);
	set_current_sprite(201);
	write_sprite_data(0, 100, 15, 255, 1, 1, (const PTILEMAP)&playership[3]);

	initialize_background();
	initialize_bullets();
	initialize_enemies();

	while(1)
	{

		wait_vbl();
		playermoving = 0;
		i = poll_joystick(PORT1, READ_BIOS);
		joy2 = i;

		if (i & JOY_UP)
		{
			if(y>-8)
				y-=playerspeed;
			playermoving = 1;
		}

		if (i & JOY_DOWN)
		{
			if(y<216)
				y+=playerspeed;
			playermoving = 1;
		}

		if (i & JOY_LEFT)
		{
			if(x>-24)
				x-=playerspeed;
			playermoving = 1;
		}

		if (i & JOY_RIGHT)
		{
			if(x<304)
				x+=playerspeed;
			playermoving = 1;
		}

		if (i & JOY_START)
		{
			x = y = 0;
		}


		i = poll_joystick(PORT1, READ_BIOS_REPEAT);

		if (i & JOY_A)
		{
			fire_new_bullet(x+24, y+2, 5, 0, BULLET_TYPE_PLAYER );
		}

		render_playership(x, y, playermoving);

		// increment background frame
		current_background_frame++;
		if(current_background_frame > maximum_frame_background_sprite()) current_background_frame=0;

		update_background(current_background_frame);
		update_bullets(x, y, &playershield);

		current_enemy_frame+=stage;
		if(current_enemy_frame > maximum_frame_enemy_sprite())
		{
			current_enemy_frame=0;
			stage++;
		}
		update_enemies(current_enemy_frame, x, y, &playershield);

		// output stats
		//textoutf(0,24, 0, 0, "stage: %d", stage);
		//textoutf(0,25, 0, 0, "VBL:%03d bgframe:%03d bgspr:%03d ", _vbl_count, current_background_frame, background_sprites_onscreen());
		//textoutf(0,26, 0, 0, "bullets:%03d enemies:%03d shield:%d", bullet_sprites_onscreen(), enemy_sprites_onscreen(), playershield);
		textoutf(0,27, 0, 0, "SHIELD: ");
		for(count=0;count<playershield;count++)textoutf(8+count,27, 0, 0, "%c", c);
		for(count=playershield;count<10;count++)textoutf(8+count,27, 0, 0, " ");
		lastscore = _vbl_count/300;
		textoutf(20,27, 0, 0, "DISTANCE: %02d", 32 - lastscore);

		// lose?
		if(playershield <= 0)
		{
			return;
		}

		if( lastscore >= 32 )
		{
			// win!
			textoutf(5,3, 0, 0, "Congrats, you beat the game!");
			// keep writing on screen for a bit longer
			if(lastscore == 33)
			{
				return;
			}
		}
	}


	return;
}

int	main(void)
{
	char* author = "kl3mousse, 2018";
	p1ship = 1; //0 or 1 or 2
	while(1)
	{
		menu();
		selectPlayerShip();
		get_ready();
		game();
		if(lastscore < 32)
			game_over();
	}

	return 0;
}
