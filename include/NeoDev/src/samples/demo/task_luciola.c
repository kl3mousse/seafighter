/* $Id: task_luciola.c,v 1.5 2001/06/15 12:15:19 fma Exp $ */

#include <stdlib.h>
#include <math.h>
#include "task_luciola.h"

#define SMALL_LUCIOLAS	4
#define BIG_LUCIOLAS	3

extern PTILEMAP map_logo;

// Small luciola, anim 1
extern TILEMAP	map_luciola_small1[];

// Small luciola, anim 2
extern TILEMAP	map_luciola_small2[];

// Big luciola, anim 1
extern TILEMAP	map_luciola_big1[];

// Big luciola, anim 2
extern TILEMAP	map_luciola_big2[];

void task_luciola_small(PTASK myself)
{
	int	x[SMALL_LUCIOLAS];
	int	y[SMALL_LUCIOLAS];
	int	w[SMALL_LUCIOLAS];
	int	a[SMALL_LUCIOLAS];
	int	s[SMALL_LUCIOLAS];
	int	i;
	
	// Fill the structures with random values
	for(i=0;i<SMALL_LUCIOLAS;i++)
	{
		w[i] = (rand()%80) + 10;
		x[i] = (rand()%(296-w[i])) + w[i] + 8;
		y[i] = (rand()%160) + 32;
		a[i] = rand()%512;
		s[i] = (rand()%3) + 1;
	}
	
	while(1)
	{
		// Display the luciolas (anim 1)
		for(i=0;i<SMALL_LUCIOLAS;i++)
		{
			write_sprite_data(
				ifmuli(fcos(a[i]), w[i]) + x[i],
		 		y[i], 15, 255, 32, 1,
				(const PTILEMAP)&map_luciola_small1);

			a[i] += 4;
			a[i] &= 511;
			
			y[i] -= s[i];
			if (y[i] < 16)
			{
				y[i] = 192;
				w[i] = (rand()%80) + 10;
				x[i] = (rand()%(296-w[i])) + w[i] + 8;
				a[i] = rand()%512;
				s[i] = (rand()%3) + 1;
			}
		}
	
		_release_timeslice();

		// Display the luciolas (anim 2)
		for(i=0;i<SMALL_LUCIOLAS;i++)
		{
			write_sprite_data(
				ifmuli(fcos(a[i]), w[i]) + x[i],
		 		y[i], 15, 255, 32, 1,
				(const PTILEMAP)&map_luciola_small2);

			a[i] += 4;
			a[i] &= 511;

			y[i] -= s[i];
			if (y[i] < 16)
			{
				y[i] = 192;
				w[i] = (rand()%80) + 10;
				x[i] = (rand()%(296-w[i])) + w[i] + 8;
				a[i] = rand()%512;
				s[i] = (rand()%3) + 1;
			}
		}

		_release_timeslice();
	}	
}

void task_luciola_big(PTASK myself)
{
	int	x[BIG_LUCIOLAS];
	int	y[BIG_LUCIOLAS];
	int	w[BIG_LUCIOLAS];
	int	a[BIG_LUCIOLAS];
	int	s[BIG_LUCIOLAS];
	int	i;
	
	// Fill the structures with random values
	for(i=0;i<BIG_LUCIOLAS;i++)
	{
		w[i] = (rand()%80) + 10;
		x[i] = (rand()%(280-w[i])) + w[i] + 8;
		y[i] = (rand()%160) + 32;
		a[i] = rand()%512;
		s[i] = (rand()%3) + 1;
	}
	
	while(1)
	{
		// Display the luciolas (anim 1)
		for(i=0;i<BIG_LUCIOLAS;i++)
		{
			write_sprite_data(
				ifmuli(fcos(a[i]), w[i]) + x[i],
		 		y[i], 15, 255, 32, 2,
				(const PTILEMAP)&map_luciola_big1);

			a[i] += 4;
			a[i] &= 511;
			
			y[i] -= s[i];
			if (y[i] < 0)
			{
				y[i] = 192;
				w[i] = (rand()%80) + 10;
				x[i] = (rand()%(280-w[i])) + w[i] + 8;
				a[i] = rand()%512;
				s[i] = (rand()%3) + 1;
			}
		}
	
		_release_timeslice();

		// Display the luciolas (anim 2)
		for(i=0;i<BIG_LUCIOLAS;i++)
		{
			write_sprite_data(
				ifmuli(fcos(a[i]), w[i]) + x[i],
		 		y[i], 15, 255, 32, 2,
				(const PTILEMAP)&map_luciola_big2);

			a[i] += 4;
			a[i] &= 511;

			y[i] -= s[i];
			if (y[i] < 0)
			{
				y[i] = 192;
				w[i] = (rand()%80) + 10;
				x[i] = (rand()%(280-w[i])) + w[i] + 8;
				a[i] = rand()%512;
				s[i] = (rand()%3) + 1;
			}
		}

		_release_timeslice();
	}	
}

void task_bgdisp(PTASK myself)
{	
	write_sprite_data(0, -8, 15, 255, 32, 19,
		(const PTILEMAP)&map_logo);

	_release_timeslice();

	while(1)
	{
		inc_current_sprite(19);		
		_release_timeslice();
	}	
}
