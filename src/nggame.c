#include "nggame.h"


#define FRONT_START_X 157
#define FRONT_START_Y 24
#define FRONT_MIN_X 8
#define FRONT_MAX_X 307
#define FRONT_MIN_Y 16
#define FRONT_MAX_Y 24

#define BACK_MIN_X 8
#define BACK_MAX_X 149
#define BACK_MIN_Y 5
#define BACK_MAX_Y 8

int x=94+48;
int y=54;
picture demomode_title;
picture seafighter_ship;

/*******************************
  MVS GAME INSTALL ONCE
*******************************/
void ngMVSGameInstallOnce(){
  typedef struct bkp_ram_info
  {
  	WORD debug_dips;
  	//	256 bytes backup block
  	//	BYTE save_slot[254];
  	//	WORD save_slot[126];
  		DWORD save_slot[62];
  }
  bkp_ram_info;
  bkp_ram_info bkp_data;

  // initial load of default ranking data

  // high scores
  bkp_data.save_slot[0]=18000000;
  bkp_data.save_slot[1]=14000000;
  bkp_data.save_slot[2]=12000000;
  bkp_data.save_slot[3]=10800000;
  bkp_data.save_slot[4]=10400000;
  bkp_data.save_slot[5]=10200000;
  bkp_data.save_slot[6]=10080000;
  bkp_data.save_slot[7]=10040000;
  bkp_data.save_slot[8]=10020000;
  bkp_data.save_slot[9]=10008000;

  // three letter names
  bkp_data.save_slot[10]=11200824;
  bkp_data.save_slot[11]=11081600;
  bkp_data.save_slot[12]=11130114;
  bkp_data.save_slot[13]=11282828;
  bkp_data.save_slot[14]=11140515;
  bkp_data.save_slot[15]=11070515;
  bkp_data.save_slot[16]=11132219; // MVS hex= A9 DD 3B?
  bkp_data.save_slot[17]=11201516;
  bkp_data.save_slot[18]=11200514;
  bkp_data.save_slot[19]=11181411;

};


/*******************************
	USER MODE INITIALIZATION
*******************************/
void ngUserModeInit(){
  pictureInit(&demomode_title, &demomode_seafighter_title, 1, 1, 1, 1,FLIP_NONE);
	palJobPut(1,demomode_seafighter_title.palInfo->count,demomode_seafighter_title.palInfo->data);

	pictureInit(&seafighter_ship, &seafighterh02, 21, 21, x, y,FLIP_NONE);
	palJobPut(21,seafighterh02.palInfo->count,seafighterh02.palInfo->data);

	volMEMWORD(0x401ffe)=0x7022; // background color
	volMEMWORD(0x400002)=0x79BB; // fix layer font color
	volMEMWORD(0x400004)=0x7022; // fix layer background color

};

/***************************************************************************
	DEMO MODE LOOP

  This loop is called while waiting for the player to insert coins (MVS)
  or to Press Start (AES). Once done, it moves to Title mode.
***************************************************************************/
void ngDemoModeLoop(int demo_timer, int isMVSorAES, int flash_timer, uchar mvs_demosound){
  p1=volMEMBYTE(P1_CURRENT);
  if(p1&JOY_UP)		  y--;
  if(p1&JOY_DOWN)		y++;
  if(p1&JOY_LEFT)		x--;
  if(p1&JOY_RIGHT)	x++;

  pictureSetPos(&seafighter_ship, x, y);
  pictureSetPos(&demomode_title, 1, 1);

};

/*******************************
	TITLE MODE LOOP
*******************************/
void ngTitleModeLoop(int isMVSorAES, int flash_timer, uchar title_timer, uchar dev_mode, uchar creditsP1, uchar USmode, uchar creditsP2){
  fixPrintf(16, 9,0,0,"TIME:%02d", title_timer);
  if(flash_timer>30)	fixPrintf(14,11,0,0,"PRESS START!");
  else				fixPrintf(14,11,0,0,"            ");

  //p1=volMEMBYTE(P1_CURRENT);
	if(p1&JOY_UP)		  y-=2;
	if(p1&JOY_DOWN)		y+=2;
	if(p1&JOY_LEFT)		x-=2;
	if(p1&JOY_RIGHT)	x+=2;

  pictureSetPos(&seafighter_ship, x, y);
	//pictureSetPos(&demomode_title, x, y);
}
