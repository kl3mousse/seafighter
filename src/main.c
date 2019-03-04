/***************************************/
/* Seafighter by Kl3mousse             */
/* for NEO GEO                         */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <input.h>
#include <DATlib.h>
#include "..\gfxout\charInclude.h" // include sprite metadata from DATlib
#include "..\gfxout\fixData.h"     // include fix metadata from DATlib

// ngsdk prototypes
bool NGSDK_SHOWINFO;           //1: will display current mode on FIX layer. 0: normal play.
#define NGSDK_DEMOTIMER_DURATION 1200 // duration of demo mode when no Credit.
#define NGSDK_FLASHTIMER_DURATION 60 // duration for blinking of INSERT COIN, PRESS START, ..

void ngsdk_init(void);
void ngsdk_demomode(int demo_timer, int isMVSorAES, int flash_timer, uchar mvs_demosound);

// NeoHomebrew init

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

BYTE p1,p2,ps,p1e,p2e;

typedef struct global_init
{
	int user_request_0_flag;
	int aes_init_flag;
	int aes_soft_dip_lives, aes_soft_dip_continues, aes_soft_dip_difficulty;
}

global_init;
global_init global_data;


static const char LetterList[30][3] =
{
    " \0","A\0","B\0","C\0","D\0","E\0","F\0","G\0","H\0","I\0",
    "J\0","K\0","L\0","M\0","N\0","O\0","P\0","Q\0","R\0","S\0",
    "T\0","U\0","V\0","W\0","X\0","Y\0","Z\0",":\0","!\0","<\0"
};

// function prototypes
int convertHexToDecimal(int hexadecimal);
void displayRanking(void);
void displaySoftDipsMVS(void);
void displaySoftDipsAES(void);
void playerStartAES(void);


// main loop ////////////////////////////////////////////////////////////////////

void USER(void)
{
	int demo_timer=NGSDK_DEMOTIMER_DURATION;
	int flash_timer=0;
	int P1_continue_timer=0;
	int P2_continue_timer=0;
	int P1_game_over_timer=0;
	int P2_game_over_timer=0;
	int P1_input_delay=0;
	int P2_input_delay=0;
	int AES_user_mode=0;
	int AES_P1_credits=0;
	int AES_P2_credits=0;
	int AES_P1_state=0;
	int AES_P2_state=0;

//ngsdk
  uchar mvs_demosound;
  int isMVSorAES; // 1 = MVS, 2 = AES

	LSPCmode=0x900;

	initGfx();
	clearFixLayer();
	clearSprites(1, 381);
	mvs_demosound=volMEMBYTE(0x10FD8B);

	ngsdk_init();

	SCClose();



	while(1)
	{
		waitVBlank();
		SCClose();
		p1=volMEMBYTE(P1_CURRENT); // read P1 inputs
		p2=volMEMBYTE(P2_CURRENT); // read P2 inputs
		ps=volMEMBYTE(PS_CURRENT); // read START button inputs


		flash_timer+=1;
		if(flash_timer == NGSDK_FLASHTIMER_DURATION) flash_timer=0;

		// INIT MODE /////////////////////////////////////////////////////////////////////////////////////////


		if(volMEMBYTE(0x10FDAE)==0) // user_request=0 is called by BIOS
		{
			// is called by BIOS if given NGH number has not been found into backup RAM (MVS system)
			// only executed once if game is installed the first time into the MVS system
			// is always executed in simulated MVS-Mode on AES systems with Uni-Bios

			// if MVS system has been detected:
			// initial load of default SOFT_DIP data from JPConfig,	USConfig, EUConfig
			// initial setup of the backup RAM save pointer (game data like rankings can be saved in this loaction)

			waitVBlank();
			global_data.user_request_0_flag=1; // indicates that user_request=0 has been executed

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
			bkp_data.save_slot[16]=11132219; // MVS
			bkp_data.save_slot[17]=11201516;
			bkp_data.save_slot[18]=11200514;
			bkp_data.save_slot[19]=11181411;

			break;  // exit loop and call BIOSF_SYSTEM_RETURN (tells the system that the INIT MODE has been finished)
		}

		if(volMEMBYTE(0x10FD82)==0 && global_data.aes_init_flag==0) // AES system detected && only executed once upon system start
		{
			// always loads default SOFT DIP data and default ranking data when AES system starts since there is no backup RAM
			// all data will be lost if AES system is turned off
			waitVBlank();
			global_data.aes_init_flag=1; // set flag to avoid multible inits after BIOSF_SYSTEM_RETURN

			// initial load of default AES soft dip data

			global_data.aes_soft_dip_lives=5;
			global_data.aes_soft_dip_continues=4;
			global_data.aes_soft_dip_difficulty=3;

			break;  // exit loop and call BIOSF_SYSTEM_RETURN (tells the system that the INIT MODE has been finished)
		}

		// DEMO MODE MVS/AES /////////////////////////////////////////////////////////////////////////////////////////

		if(volMEMBYTE(0x10FDAE)==2 && volMEMBYTE(0x10FDAF)==1) // bios_user_request=2, bios_user_mode=1
		{

			if(volMEMBYTE(0x10FD82)>0){isMVSorAES=1;}else{isMVSorAES=2;};

			ngsdk_demomode(demo_timer, isMVSorAES, flash_timer, mvs_demosound);
			// 			fixPrintf( 2, 6,0,0,"%08d",	bkp_data.save_slot[9]);


			// MVS BIOS /////////////////////////////////////////////
			if(isMVSorAES==1)
			{
				if(demo_timer==NGSDK_DEMOTIMER_DURATION)
				{
					// fill AES soft dips with MVS soft dip data (for MVS free play mode)
					global_data.aes_soft_dip_lives		=	volMEMBYTE(0x10FD88);  // MVS SOFT DIP 5 (LIVES)
					global_data.aes_soft_dip_continues	=	volMEMBYTE(0x10FD89);  // MVS SOFT DIP 6 (CONTINUES)
					global_data.aes_soft_dip_difficulty	=	volMEMBYTE(0x10FD8A);  // MVS SOFT DIP 7 (DIFFICULTY)
				}
			}

			// AES BIOS /////////////////////////////////////////////
			if(isMVSorAES==2)
			{
				if(demo_timer==NGSDK_DEMOTIMER_DURATION)
				{
					displayRanking(); 	  // ranking table
					displaySoftDipsAES(); // AES soft dips
					AES_P1_credits=global_data.aes_soft_dip_continues; // add virtual credits
					AES_P2_credits=global_data.aes_soft_dip_continues; // add virtual credits
				}
			}

			// DEMO END /////////////////////////////////////////////

			demo_timer-=1;
			if(demo_timer==0)
			{
				clearFixLayer();
				break; // exit loop and call BIOSF_SYSTEM_RETURN (tells the system that the DEMO MODE has been finished, calls DEMO_END function)
			}
		}

		// TITLE MODE MVS ////////////////////////////////////////////////////////////////////////////////////////

		if(volMEMBYTE(0x10FDAE)==3 && volMEMBYTE(0x10FDAF)==1) // bios_user_request=3, bios_user_mode=1
		{
			volMEMBYTE(0x10FEC5)=0x01; // BIOS_COMPULSION_FLAG prevents that compulsion doesn't restart twice, if still credits available from previous gameplay

			volMEMWORD(0x401ffe)=0x1351; // background color
			volMEMWORD(0x400002)=0x6BDA; // fix layer font color
			volMEMWORD(0x400004)=0x1351; // fix layer background color

			fixPrintf( 2, 4,0,0,"============ TITLE MVS =============");
			fixPrintf(16, 9,0,0,"TIME:%02d", convertHexToDecimal(volMEMBYTE(0x10FDDA))); // BIOS-COMPULSION-TIMER - timer for forced game start

			if(flash_timer>30)	fixPrintf(14,11,0,0,"PRESS START!");
			else				fixPrintf(14,11,0,0,"            ");

			// display credit counter(s) ////////////////////////////////////

			if(volMEMBYTE(0x10FE80)>0) // if developer mode is active, display dev mode credit counters
			{
				fixPrintf( 2,23,0,0,"DevCRED P1: %02d", convertHexToDecimal(volMEMBYTE(0x10FE00))); // dev mode credit counter P1
				// display separete P2 credit counter if BIOS-COUNTRY-CODE is 1=USA
				if(volMEMBYTE(0x10FD83)==1) fixPrintf(24,23,0,0,"DevCRED P2: %02d", convertHexToDecimal(volMEMBYTE(0x10FE01)));  // dev mode credit counter P2

			}else{


				fixPrintf( 2,23,0,0,"CREDITS P1: %02d", convertHexToDecimal(volMEMBYTE(0xD00034)));  // credit counter P1
				// fixPrintf( 2,22,0,0,"DevCRED P1: %02d", convertHexToDecimal(volMEMBYTE(0x10FE00)));  // dev mode credit counter P1
				// display separete P2 credit counter if BIOS-COUNTRY-CODE is 1=USA
				if(volMEMBYTE(0x10FD83)==1) fixPrintf(24,23,0,0,"CREDITS P2: %02d", convertHexToDecimal(volMEMBYTE(0xD00035)));	// credit counter P2
				// if(volMEMBYTE(0x10FD83)==1) fixPrintf(24,22,0,0,"DevCRED P2: %02d", convertHexToDecimal(volMEMBYTE(0x10FE01)));	// dev mode credit counter P2
			}

		}

		// TITLE MODE AES ////////////////////////////////////////////////////////////////////////////////////////
		// the AES title mode is also used by MVS in "free play" mode (hard dip switch 6 active)

		if(volMEMBYTE(0x10FDAE)==2 && volMEMBYTE(0x10FDAF)==2 && AES_user_mode==0) // bios_user_request=2, bios_user_mode=2
		{
			AES_user_mode=1;		// change to title mode
			volMEMBYTE(0x10FDB4)=0;	// reset input flag
			volMEMBYTE(0x10FDB6)=0;	// change BIOS-PLAYER-MOD1 to 0 (P1 never played)
			volMEMBYTE(0x10FDB7)=0;	// change BIOS-PLAYER-MOD2 to 0 (P2 never played)
		}

		if(AES_user_mode==1)
		{
			fixPrintf( 2, 4,0,0,"============ TITLE AES =============");

			if(flash_timer>30)	fixPrintf(14, 9,0,0,"PRESS START!");
			else				fixPrintf(14, 9,0,0,"            ");

			fixPrintf( 2,23,0,0,"CREDITS P1: %02d", AES_P1_credits);
			fixPrintf(24,23,0,0,"CREDITS P2: %02d", AES_P2_credits);

			//  Player 1 has started the game from the title mode
			if(volMEMBYTE(0x10FDB6)==1 && volMEMBYTE(0x10FDB4)==1) // BIOS-PLAYER-MOD1 == 1 (P1 playing) && START_FLAG == 1
			{
				if(volMEMBYTE(0x10FD82)==0) AES_P1_credits-=1; 	// decrement virtual P1 credit if system is AES
				AES_P1_state=1;		// set P1 state to 1 (playing)
				AES_user_mode=2;	// change to game mode
				clearFixLayer();	// clear title screen
			}

			//  Player 2 has started the game from the title mode
			if(volMEMBYTE(0x10FDB7)==1 && volMEMBYTE(0x10FDB4)==2) // BIOS-PLAYER-MOD2 == 1 (P2 playing) && START_FLAG == 2
			{
				if(volMEMBYTE(0x10FD82)==0) AES_P2_credits-=1;	// decrement virtual P2 credit if system is AES
				AES_P2_state=1;		// set P2 state to 1 (playing)
				AES_user_mode=2;	// change to game mode
				clearFixLayer();	// clear title screen
			}
		}

		// GAME MODE AES /////////////////////////////////////////////////////////////////////////////////////////
		//  the AES title mode is also used by MVS in "free play" mode (hard dip switch 6 active)

		if(AES_user_mode==2)
		{
			volMEMWORD(0x401ffe)=0x5872; // background color
			volMEMWORD(0x400002)=0x4ED9; // fix layer font color
			volMEMWORD(0x400004)=0x5872; // fix layer background color

			fixPrintf( 2, 4,0,0,"============ GAME AES  ============= ");
			fixPrintf( 2,23,0,0,"CREDITS P1: %02d", AES_P1_credits);
			fixPrintf(24,23,0,0,"CREDITS P2: %02d", AES_P2_credits);

			// join game ///////////////////////////////////////////////////////////////

			// Player 1 ////////////////////////////////////////////////////////////////
			if(AES_P1_state==0)  // AES_P1_state 0 = P1 never played
			{
				if(AES_P1_credits>0 || volMEMBYTE(0x10FD82)>0)  // join P2 game if P1 credits available or if MVS free play mode is active
				{
					fixPrintf( 4, 6,0,0,"JOIN GAME   ");
					fixPrintf( 3, 9,0,0,"PRESS START ");

					if(ps&P1_START)
					{
						AES_P1_state=1;		// set AES_P1_state to 1 (P1 playing)
						if(volMEMBYTE(0x10FD82)==0) AES_P1_credits-=1; 	// decrement virtual P1 credit if system is AES
					}
				}

				if(AES_P1_credits==0 && volMEMBYTE(0x10FD82)==0) // P1 cant play anymore if no P1 credits available and if system is AES
				{
					fixPrintf( 4, 6,0,0,"GAME OVER   ");
				}
			}

			// Player 2 ///////////////////////////////////////////////////////////////////
			if(AES_P2_state==0)  // AES_P2_state 0 = P2 never played
			{
				if(AES_P2_credits>0 || volMEMBYTE(0x10FD82)>0)  // join P1 game if P2 credits available or if MVS free play mode is active
				{
					fixPrintf(26, 6,0,0,"JOIN GAME   ");
					fixPrintf(25, 9,0,0,"PRESS START ");

					if(ps&P2_START)
					{
						AES_P2_state=1;		// set AES_P2_state to 1 (P2 playing)
						if(volMEMBYTE(0x10FD82)==0) AES_P2_credits-=1; 	// decrement virtual P2 credit if system is AES
					}
				}

				if(AES_P2_credits==0 && volMEMBYTE(0x10FD82)==0) // P2 cant play anymore if no P2 credits available and if system is AES
				{
					fixPrintf(26, 6,0,0,"GAME OVER   ");
				}
			}

			// playing /////////////////////////////////////////////////////////////////

			// Player 1 ////////////////////////////////////////////////////////////////
			if(AES_P1_state==1) // AES_P1_state 1 = P1 playing
			{
				fixPrintf( 4, 6,0,0,"PLAYING  ");
				fixPrintf( 3, 9,0,0,"P1 PRESS A ");

				if(p1&JOY_A)
				{
					if(AES_P1_credits>0 || volMEMBYTE(0x10FD82)>0)  // start continue timer if P1 credits available or if MVS free play mode
					{
						P1_continue_timer=600;	// start P1 continue timer
						AES_P1_state=2;			// set AES_P1_state to 2 (P1 Continue Option)
						clearFixLayer();		// clear message
					}

					if(AES_P1_credits==0 && volMEMBYTE(0x10FD82)==0) // start game over timer if no P1 credits available if system is AES
					{
						P1_game_over_timer=300;	// start P1 game_over_timer
						AES_P1_state=3;			// set AES_P1_state to 3 (P1 Game Over)
						clearFixLayer();		// clear message
					}
				}
			}

			// Player 2 ///////////////////////////////////////////////////////////////////
			if(AES_P2_state==1) // AES_P2_state 1 = P2 playing
			{
				fixPrintf(26, 6,0,0,"PLAYING  ");
				fixPrintf(25, 9,0,0,"P2 PRESS A ");

				if(p2&JOY_A)
				{
					if(AES_P2_credits>0 || volMEMBYTE(0x10FD82)>0)  // start continue timer if P2 credits available or if MVS free play mode
					{
						P2_continue_timer=600;	// start P2 continue timer
						AES_P2_state=2;			// set AES_P2_state to 2 (P2 Continue Option)
						clearFixLayer();		// clear message
					}

					if(AES_P2_credits==0 && volMEMBYTE(0x10FD82)==0) // start game over timer if no P2 credits available if system is AES
					{
						P2_game_over_timer=300;	// start P2 game_over_timer
						AES_P2_state=3;			// set AES_P2_state to 3 (P2 Game Over)
						clearFixLayer();		// clear message
					}
				}
			}

			// continue timer //////////////////////////////////////////////////////////

			// Player 1 ////////////////////////////////////////////////////////////////
			if(P1_continue_timer>0)
			{
				P1_continue_timer-=1;

				if(P1_input_delay==0 && P1_continue_timer>60) // speed up timer if C is pressed
				{
					if(p1&JOY_C)
					{
						P1_continue_timer-=60;
						P1_input_delay=10;
					}
				}

				fixPrintf( 4, 6,0,0,"CONTINUE?"); // BIOS-PLAYER-MOD1 2 = continue
				fixPrintf( 7, 7,0,0,"%02d", P1_continue_timer/60); // show timer
				fixPrintf( 3, 9,0,0,"PRESS START ");

				if(P1_continue_timer==0) // Player 1 has not pressed START, start game_over_timer
				{
					AES_P1_state=3;  		// sets BIOS-PLAYER-MOD1 to 3 (P1 Game Over)
					P1_game_over_timer=300;	// start game_over_timer
					clearFixLayer();		// clear message
				}

				if(ps&P1_START)	// Player 1 has pressed START - continue game
				{
					AES_P1_state=1;			// set AES_P1_state to 1 (P1 playing)
					if(volMEMBYTE(0x10FD82)==0) AES_P1_credits-=1; 	// decrement virtual P1 credit if system is AES
					P1_continue_timer=0;	// reset timer
					clearFixLayer();		// clear message
				}
			}

			// Player 2 ///////////////////////////////////////////////////////////////////
			if(P2_continue_timer>0)
			{
				P2_continue_timer-=1;

				if(P2_input_delay==0 && P2_continue_timer>60) // speed up timer if C is pressed
				{
					if(p2&JOY_C)
					{
						P2_continue_timer-=60;
						P2_input_delay=10;
					}
				}

				fixPrintf(26, 6,0,0,"CONTINUE?");
				fixPrintf(29, 7,0,0,"%02d", P2_continue_timer/60); // show timer
				fixPrintf(25, 9,0,0,"PRESS START ");

				if(P2_continue_timer==0) // Player 2 has not pressed START, start game_over_timer
				{
					AES_P2_state=3;  // sets BIOS-PLAYER-MOD2 to 3 (P2 Game Over)
					P2_game_over_timer=300;	// start game_over_timer
					clearFixLayer();		// clear message
				}

				if(ps&P2_START) // Player 2 has pressed START - continue game
				{
					AES_P2_state=1;			// set AES_P2_state to 1 (P2 playing)
					if(volMEMBYTE(0x10FD82)==0) AES_P2_credits-=1; 		// decrement virtual P2 credit if system is AES
					P2_continue_timer=0;	// reset timer
					clearFixLayer();		// clear message
				}
			}

			// game over timer //////////////////////////////////////////////////////////

			// Player 1 ////////////////////////////////////////////////////////////////
			if(P1_game_over_timer>0)
			{
				P1_game_over_timer-=1;
				fixPrintf( 4, 6,0,0,"GAME OVER  ");
				fixPrintf( 7, 7,0,0,"%02d", P1_game_over_timer/60);

				if(p1&JOY_B)
				{
					bkp_data.save_slot[6]=10123456;  // add score
					bkp_data.save_slot[16]=11140802; // add name
				}

				if(P1_game_over_timer==0)
				{
					AES_P1_state=0;			// set AES_P1_state to 0 (P1 never played)
					clearFixLayer();		// clear message
				}
			}

			// Player 2 ////////////////////////////////////////////////////////////////
			if(P2_game_over_timer>0)
			{
				P2_game_over_timer-=1;
				fixPrintf(26, 6,0,0,"GAME OVER  ");
				fixPrintf(29, 7,0,0,"%02d", P2_game_over_timer/60);

				if(p2&JOY_B)
				{
					bkp_data.save_slot[6]=10123456;  // add score
					bkp_data.save_slot[16]=11140802; // add name
				}

				if(P2_game_over_timer==0)
				{
					AES_P2_state=0;			// set AES_P2_state to 0 (P2 never played)
					clearFixLayer();		// clear message
				}
			}

			if(P1_input_delay>0){P1_input_delay-=1;} // count down input delay
			if(P2_input_delay>0){P2_input_delay-=1;} // count down input delay

			// exit game mode ////////////////////////////////////////////////////////////////

			if(AES_P1_state==0 && AES_P2_state==0) // Player 1 and Player 2 have no credits or do not want to play
			{
				break; // exit loop and call BIOSF_SYSTEM_RETURN (will restart demo mode)
			}
		}

		// GAME MODE MVS /////////////////////////////////////////////////////////////////////////////////////////

		if(volMEMBYTE(0x10FDAE)==3 && volMEMBYTE(0x10FDAF)==2) // bios_user_request=3, bios_user_mode=2
		{
			volMEMWORD(0x401ffe)=0x5872; // background color
			volMEMWORD(0x400002)=0x4ED9; // fix layer font color
			volMEMWORD(0x400004)=0x5872; // fix layer background color

			fixPrintf( 2, 4,0,0,"============ GAME MVS  ============= ");

			// display credit counter(s) ////////////////////////////////////

			if(volMEMBYTE(0x10FE80)>0) // if developer mode is active display dev mode credit counters
			{
				fixPrintf( 2,23,0,0,"DevCRED P1: %02d", convertHexToDecimal(volMEMBYTE(0x10FE00))); // dev mode credit counter P1
				// display separete P2 credit counter if BIOS-COUNTRY-CODE is 1=USA
				if(volMEMBYTE(0x10FD83)==1) fixPrintf(24,23,0,0,"DevCRED P2: %02d", convertHexToDecimal(volMEMBYTE(0x10FE01)));  // dev mode credit counter P2

			}else{


				fixPrintf( 2,23,0,0,"CREDITS P1: %02d", convertHexToDecimal(volMEMBYTE(0xD00034)));  // credit counter P1
				// fixPrintf( 2,22,0,0,"DevCRED P1: %02d", convertHexToDecimal(volMEMBYTE(0x10FE00)));  // dev mode credit counter P1
				// display separete P2 credit counter if BIOS-COUNTRY-CODE is 1=USA
				if(volMEMBYTE(0x10FD83)==1) fixPrintf(24,23,0,0,"CREDITS P2: %02d", convertHexToDecimal(volMEMBYTE(0xD00035)));	// credit counter P2
				// if(volMEMBYTE(0x10FD83)==1) fixPrintf(24,22,0,0,"DevCRED P2: %02d", convertHexToDecimal(volMEMBYTE(0x10FE01)));	// dev mode credit counter P2
			}

			// join game ///////////////////////////////////////////////////////////////

			// Player 1 ////////////////////////////////////////////////////////////////
			if(volMEMBYTE(0x10FDB6)==0)  // BIOS-PLAYER-MOD1 0 = never played
			{
				fixPrintf( 4, 6,0,0,"JOIN GAME   ");
				if(volMEMBYTE(0x10FE00)>0  && volMEMBYTE(0x10FE80)>0)  fixPrintf( 3, 9,0,0,"PRESS START "); // dev P1 credits available && dev-mode active
				if(volMEMBYTE(0x10FE00)==0 && volMEMBYTE(0x10FE80)>0)  fixPrintf( 3, 9,0,0,"INSERT COIN "); // no dev P1 credits available && dev-mode active
				if(volMEMBYTE(0xD00034)>0  && volMEMBYTE(0x10FE80)==0) fixPrintf( 3, 9,0,0,"PRESS START "); // regular P1 credits available && dev-mode inactive
				if(volMEMBYTE(0xD00034)==0 && volMEMBYTE(0x10FE80)==0) fixPrintf( 3, 9,0,0,"INSERT COIN "); // no regular P1 credits available && dev-mode inactive
			}

			// Player 2 ///////////////////////////////////////////////////////////////////
			if(volMEMBYTE(0x10FDB7)==0)  // BIOS-PLAYER-MOD2 0 = never played
			{
				fixPrintf(26, 6,0,0,"JOIN GAME   ");

				if(volMEMBYTE(0x10FD83)==0 || volMEMBYTE(0x10FD83)==2) // BIOS-COUNTRY-CODE 0=JAP or 2=EUR (single credit counter)
				{
					if(volMEMBYTE(0x10FE00)>0  && volMEMBYTE(0x10FE80)>0)  fixPrintf(25, 9,0,0,"PRESS START "); // dev P1 credits available && dev-mode active
					if(volMEMBYTE(0x10FE00)==0 && volMEMBYTE(0x10FE80)>0)  fixPrintf(25, 9,0,0,"INSERT COIN "); // no dev P1 credits available && dev-mode active
					if(volMEMBYTE(0xD00034)>0  && volMEMBYTE(0x10FE80)==0) fixPrintf(25, 9,0,0,"PRESS START "); // regular P1 credits available && dev-mode inactive
					if(volMEMBYTE(0xD00034)==0 && volMEMBYTE(0x10FE80)==0) fixPrintf(25, 9,0,0,"INSERT COIN "); // no regular P1 credits available && dev-mode inactive
				}

				if(volMEMBYTE(0x10FD83)==1) // BIOS-COUNTRY-CODE 1=USA (separete P2 credit counter)
				{
					if(volMEMBYTE(0x10FE01)>0  && volMEMBYTE(0x10FE80)>0)  fixPrintf(25, 9,0,0,"PRESS START "); // dev P2 credits available && dev-mode active
					if(volMEMBYTE(0x10FE01)==0 && volMEMBYTE(0x10FE80)>0)  fixPrintf(25, 9,0,0,"INSERT COIN "); // no dev P2 credits available && dev-mode active
					if(volMEMBYTE(0xD00035)>0  && volMEMBYTE(0x10FE80)==0) fixPrintf(25, 9,0,0,"PRESS START "); // regular P2 credits available && dev-mode inactive
					if(volMEMBYTE(0xD00035)==0 && volMEMBYTE(0x10FE80)==0) fixPrintf(25, 9,0,0,"INSERT COIN "); // no regular P2 credits available && dev-mode inactive
				}
			}

			// playing /////////////////////////////////////////////////////////////////

			// Player 1 ////////////////////////////////////////////////////////////////
			if(volMEMBYTE(0x10FDB6)==1) // BIOS-PLAYER-MOD1 1 = playing
			{
				fixPrintf( 4, 6,0,0,"PLAYING  ");
				fixPrintf( 3, 9,0,0,"P1 PRESS A ");

				if(p1&JOY_A)
				{
					volMEMBYTE(0x10FDB6)=0x02;	// sets BIOS-PLAYER-MOD1 to 2 (P1 Continue Option)
					P1_continue_timer=600;		// start P1 continue timer
				}
			}

			// Player 2 ///////////////////////////////////////////////////////////////////
			if(volMEMBYTE(0x10FDB7)==1) // BIOS-PLAYER-MOD2 1 = playing
			{
				fixPrintf(26, 6,0,0,"PLAYING  ");
				fixPrintf(25, 9,0,0,"P2 PRESS A ");

				if(p2&JOY_A)
				{
					volMEMBYTE(0x10FDB7)=0x02;	// sets BIOS-PLAYER-MOD2 to 2 (P2 Continue Option)
					P2_continue_timer=600;		// start P2 continue timer
				}
			}

			// continue timer //////////////////////////////////////////////////////////

			// Player 1 ////////////////////////////////////////////////////////////////
			if(P1_continue_timer>0)
			{
				P1_continue_timer-=1;

				if(P1_input_delay==0 && P1_continue_timer>60) // speed up timer if C is pressed
				{
					if(p1&JOY_C)
					{
						P1_continue_timer-=60;
						P1_input_delay=10;
					}
				}

				fixPrintf( 4, 6,0,0,"CONTINUE?");
				fixPrintf( 7, 7,0,0,"%02d", P1_continue_timer/60); // show timer

				if(volMEMBYTE(0x10FE00)>0  && volMEMBYTE(0x10FE80)>0)  fixPrintf( 3, 9,0,0,"PRESS START "); // dev P1 credits available && dev-mode active
				if(volMEMBYTE(0x10FE00)==0 && volMEMBYTE(0x10FE80)>0)  fixPrintf( 3, 9,0,0,"INSERT COIN "); // no dev P1 credits available && dev-mode active
				if(volMEMBYTE(0xD00034)>0  && volMEMBYTE(0x10FE80)==0) fixPrintf( 3, 9,0,0,"PRESS START "); // regular P1 credits available && dev-mode inactive
				if(volMEMBYTE(0xD00034)==0 && volMEMBYTE(0x10FE80)==0) fixPrintf( 3, 9,0,0,"INSERT COIN "); // no regular P1 credits available && dev-mode inactive


				if(P1_continue_timer==0) // player 1 has not pressed START, start game_over_timer
				{
					volMEMBYTE(0x10FDB6)=0x03;  // sets BIOS-PLAYER-MOD1 to 3 (P1 Game Over)
					P1_game_over_timer=300;	// start game_over_timer
					clearFixLayer();		// clear message
				}

				if(volMEMBYTE(0x10FDB6)==1) // BIOS-PLAYER-MOD1 = 1 >> player 1 has pressed START - continue game
				{
					P1_continue_timer=0;	// reset timer
					clearFixLayer();	// clear message
				}
			}

			// Player 2 ///////////////////////////////////////////////////////////////////
			if(P2_continue_timer>0)
			{
				P2_continue_timer-=1;

				if(P2_input_delay==0 && P2_continue_timer>60) // speed up timer if C is pressed
				{
					if(p2&JOY_C)
					{
						P2_continue_timer-=60;
						P2_input_delay=10;
					}
				}

				fixPrintf(26, 6,0,0,"CONTINUE?");
				fixPrintf(29, 7,0,0,"%02d", P2_continue_timer/60); // show timer

				if(volMEMBYTE(0x10FD83)==0 || volMEMBYTE(0x10FD83)==2) // BIOS-COUNTRY-CODE 0=JAP or 2=EUR (single credit counter)
				{
					if(volMEMBYTE(0x10FE00)>0  && volMEMBYTE(0x10FE80)>0)  fixPrintf(25, 9,0,0,"PRESS START "); // dev P1 credits available && dev-mode active
					if(volMEMBYTE(0x10FE00)==0 && volMEMBYTE(0x10FE80)>0)  fixPrintf(25, 9,0,0,"INSERT COIN "); // no dev P1 credits available && dev-mode active
					if(volMEMBYTE(0xD00034)>0  && volMEMBYTE(0x10FE80)==0) fixPrintf(25, 9,0,0,"PRESS START "); // regular P1 credits available && dev-mode inactive
					if(volMEMBYTE(0xD00034)==0 && volMEMBYTE(0x10FE80)==0) fixPrintf(25, 9,0,0,"INSERT COIN "); // no regular P1 credits available && dev-mode inactive
				}

				if(volMEMBYTE(0x10FD83)==1) // BIOS-COUNTRY-CODE 1=USA (separete P2 credit counter)
				{
					if(volMEMBYTE(0x10FE01)>0  && volMEMBYTE(0x10FE80)>0)  fixPrintf(25, 9,0,0,"PRESS START "); // dev P2 credits available && dev-mode active
					if(volMEMBYTE(0x10FE01)==0 && volMEMBYTE(0x10FE80)>0)  fixPrintf(25, 9,0,0,"INSERT COIN "); // no dev P2 credits available && dev-mode active
					if(volMEMBYTE(0xD00035)>0  && volMEMBYTE(0x10FE80)==0) fixPrintf(25, 9,0,0,"PRESS START "); // regular P2 credits available && dev-mode inactive
					if(volMEMBYTE(0xD00035)==0 && volMEMBYTE(0x10FE80)==0) fixPrintf(25, 9,0,0,"INSERT COIN "); // no regular P2 credits available && dev-mode inactive
				}

				if(P2_continue_timer==0) // player 2 has not pressed START, start game_over_timer
				{
					volMEMBYTE(0x10FDB7)=0x03;  // sets BIOS-PLAYER-MOD2 to 3 (P2 Game Over)
					P2_game_over_timer=300;	// start game_over_timer
					clearFixLayer();		// clear message
				}

				if(volMEMBYTE(0x10FDB7)==1) // BIOS-PLAYER-MOD2 = 1 >> player 2 has pressed START - continue game
				{
					P2_continue_timer=0;	// reset timer
					clearFixLayer();	// clear message
				}
			}

			// game over timer //////////////////////////////////////////////////////////

			// Player 1 ////////////////////////////////////////////////////////////////
			if(P1_game_over_timer>0)
			{
				P1_game_over_timer-=1;
				fixPrintf( 4, 6,0,0,"GAME OVER");
				fixPrintf( 7, 7,0,0,"%02d", P1_game_over_timer/60);
			//	fixPrintf( 3, 9,0,0,"            ");

				if(p1&JOY_B)
				{
					bkp_data.save_slot[6]=22345678;  // add score
					bkp_data.save_slot[16]=11140802; // add name
				}

				if(P1_game_over_timer==0)
				{
					volMEMBYTE(0x10FDB6)=0x00;	// sets BIOS-PLAYER-MOD1 to 0 (P1 Never Played)
					clearFixLayer();			// clear message
				}
			}

			// Player 2 ////////////////////////////////////////////////////////////////
			if(P2_game_over_timer>0)
			{
				P2_game_over_timer-=1;
				fixPrintf(26, 6,0,0,"GAME OVER");
				fixPrintf(29, 7,0,0,"%02d", P2_game_over_timer/60);
			//	fixPrintf(25, 9,0,0,"            ");

				if(p2&JOY_B)
				{
					bkp_data.save_slot[6]=10123456;  // add score
					bkp_data.save_slot[16]=11140802; // add name
				}

				if(P2_game_over_timer==0)
				{
					volMEMBYTE(0x10FDB7)=0x00;	// sets BIOS-PLAYER-MOD2 to 0 (P2 Never Played)
					clearFixLayer();			// clear message
				}
			}

			if(P1_input_delay>0){P1_input_delay-=1;} // count down input delay
			if(P2_input_delay>0){P2_input_delay-=1;} // count down input delay

			// exit game mode ////////////////////////////////////////////////////////////////

			if(volMEMBYTE(0x10FDB6)==0 && volMEMBYTE(0x10FDB7)==0) // BIOS-PLAYER-MOD1 and BIOS-PLAYER-MOD2 == 0 (never played)
			{
				break; // exit loop and call BIOSF_SYSTEM_RETURN (will restart demo or return to title if credits available)
			}

		}

//		fixPrintf( 26, 5,0,0,"NGH : %06d", volMEMWORD(0x108)); // NGH number
//		fixPrintf( 26, 5,0,0,"DEV-MODE %03d", volMEMBYTE(0x10FE80)); // non-zero = developer mode is active
		fixPrintf( 26, 3,0,0,"FRAMES: %04d",	DAT_frameCounter);

		if(volMEMBYTE(0x10FD83)==0) fixPrintf(11,3,0,0,"JAP"); // BIOS-COUNTRY-CODE - 0=JAP, 1=USA, 2=EUR
		if(volMEMBYTE(0x10FD83)==1) fixPrintf(11,3,0,0,"USA"); // BIOS-COUNTRY-CODE - 0=JAP, 1=USA, 2=EUR
		if(volMEMBYTE(0x10FD83)==2) fixPrintf(11,3,0,0,"EUR"); // BIOS-COUNTRY-CODE - 0=JAP, 1=USA, 2=EUR

		if(volMEMBYTE(0x10FD82)==0) fixPrintf( 2,3,0,0,"BIOS:AES-"); // BIOS-MVS-FLAG - 0=AES, 128(0x80)=MVS
		if(volMEMBYTE(0x10FD82)>0 ) fixPrintf( 2,3,0,0,"BIOS:MVS-"); // BIOS-MVS-FLAG - 0=AES, 128(0x80)=MVS

		fixPrintf( 2,25,0,0,"------------------------------------");
		fixPrintf( 2,26,0,0,"INIT-FLAG   : %02d", global_data.user_request_0_flag); // 0 = init mode was not called, 1 = init mode was called by BIOS
		fixPrintf( 2,27,0,0,"USER-REQUEST: %02d", volMEMBYTE(0x10FDAE)); // 0 = Init, 1 = Boot animation, 2 = Demo, 3 = Game (set by SYSTEM BIOS)
		fixPrintf( 2,28,0,0,"USER-MODE   : %02d", volMEMBYTE(0x10FDAF)); // Used by the game to tell what it's doing: 0:Init/Boot animation, 1:Title/Demo, 2:Game

		fixPrintf(22,26,0,0,"START FLAG  : %02d", volMEMBYTE(0x10FDB4)); // indicates which player has pressed the START_BUTTON (P1=1, P2=2)
		fixPrintf(22,27,0,0,"PLAYER-MOD1 : %02d", volMEMBYTE(0x10FDB6)); // Player 1 status. 0:Never played, 1:Playing, 2:Continue option being displayed, 3:Game over
		fixPrintf(22,28,0,0,"PLAYER-MOD2 : %02d", volMEMBYTE(0x10FDB7)); // Player 1 status. 0:Never played, 1:Playing, 2:Continue option being displayed, 3:Game over
	}

	__asm__ ("jmp 0xc00444 \n"); // BIOSF_SYSTEM_RETURN - return to bios control
}

void PLAYER_START(void) // is called by MVS and AES BIOS if user has pressed P1 or P2 START_BUTTON
{
	// PLAYER 1 ///////////////////////////////////////////////////////////////////////////////////////

	// Player 1 is in the GAME OVER state, pressing START is disabled (to avoid unwanted credit decrement)
	if(volMEMBYTE(0x10FDB6)==3 && volMEMBYTE(0x10FDB4)==1) // BIOS-PLAYER-MOD1 == 3 && START_FLAG == 1
	{
		volMEMBYTE(0x10FDB4)=0x00; // deactivate P1 START_BUTTON
	}

	// Player 1 is currently playing, pressing START is disabled (to avoid unwanted credit decrement)
	if(volMEMBYTE(0x10FDB6)==1 && volMEMBYTE(0x10FDB4)==1) // BIOS-PLAYER-MOD1 == 1 && START_FLAG == 1
	{
		volMEMBYTE(0x10FDB4)=0x00; // deactivate P1 START_BUTTON
	}

	// Player 1 Continue option being displayed, pressing START is allowed
	if(volMEMBYTE(0x10FDB6)==2 && volMEMBYTE(0x10FDB4)==1) // BIOS-PLAYER-MOD1 == 2 && START_FLAG == 1
	{
		volMEMBYTE(0x10FDB6)=0x01; // sets BIOS-PLAYER-MOD1 to 1 (P1 Playing) to decrement 1 credit
	}

	//  Player 1 has started the game for the first time from the title mode
	if(volMEMBYTE(0x10FDAF)==1 && volMEMBYTE(0x10FDB6)==0 && volMEMBYTE(0x10FDB4)==1) // BIOS-USER-MODE == 1 && BIOS-PLAYER-MOD1 == 0 && START_FLAG == 1
	{
		volMEMBYTE(0x10FDAF)=0x02;	// set BIOS-USER-MODE to 2 (GAME), it stops the BIOS_COMPULSION_TIMER
		volMEMBYTE(0x10FDB6)=0x01;	// set BIOS-PLAYER-MOD1 to 1 (P1 Playing), it deactivates P1 START_BUTTON
		clearFixLayer(); 			// clear TITLE screen
	}

	// Player 1 joins the game of Player 2
	if(volMEMBYTE(0x10FDAF)==2 && volMEMBYTE(0x10FDB6)==0 && volMEMBYTE(0x10FDB4)==1) // BIOS-USER-MODE == 2 && BIOS-PLAYER-MOD1 == 0  && START_FLAG == 1
	{
		volMEMBYTE(0x10FDB6)=0x01;	// set BIOS-PLAYER-MOD1 to 1 (P1 Playing), it deactivates P1 START_BUTTON
	}

	// PLAYER 2 ///////////////////////////////////////////////////////////////////////////////////////

	// Player 2 is in the GAME OVER state, pressing START is disabled (to avoid unwanted credit decrement)
	if(volMEMBYTE(0x10FDB7)==3 && volMEMBYTE(0x10FDB4)==2) // BIOS-PLAYER-MOD1 == 3 && START_FLAG == 2
	{
		volMEMBYTE(0x10FDB4)=0x00; // deactivate P2 START_BUTTON
	}

	// Player 2 is currently playing, pressing START is disabled (to avoid unwanted credit decrement)
	if(volMEMBYTE(0x10FDB7)==1 && volMEMBYTE(0x10FDB4)==2) // BIOS-PLAYER-MOD2 = 1  && START_FLAG == 2
	{
		volMEMBYTE(0x10FDB4)=0x00; // deactivate P2 START_BUTTON
	}

	// Player 2 Continue option being displayed, pressing START is allowed
	if(volMEMBYTE(0x10FDB7)==2 && volMEMBYTE(0x10FDB4)==2) // BIOS-PLAYER-MOD2 == 2  && START_FLAG == 2
	{
		volMEMBYTE(0x10FDB7)=0x01; // sets BIOS-PLAYER-MOD2 to 1 (P2 Playing) to decrement 1 credit
	}

	//  Player 2 has started the game for the first time from the title mode
	//  JAP or EUR BIOS has start flag 3 for Player 2, it will decrement 2 credits and starts P1 and P2 game simultaneously
	if(volMEMBYTE(0x10FDAF)==1 && volMEMBYTE(0x10FDB7)==0 && volMEMBYTE(0x10FDB4)==3) // BIOS-USER-MODE == 1 && BIOS-PLAYER-MOD2 = 0 && START_FLAG == 3
	{
		volMEMBYTE(0x10FDAF)=0x02;	// set BIOS-USER-MODE to 2 (GAME), it stops the BIOS_COMPULSION_TIMER
		volMEMBYTE(0x10FDB7)=0x01;	// set BIOS-PLAYER-MOD2 to 1 (P2 Playing), it deactivates P2 START_BUTTON
		volMEMBYTE(0x10FDB6)=0x01;	// set BIOS-PLAYER-MOD1 to 1 (P1 Playing), it deactivates P1 START_BUTTON
		clearFixLayer(); 			// clear TITLE screen
	}

	//  Player 2 has started the game for the first time from the title mode
	//  USA BIOS has start flag 2 for Player 2, and will start the game only for Player 2 and decrement 1 credit (from P2 credit counter)
	if(volMEMBYTE(0x10FDAF)==1 && volMEMBYTE(0x10FDB7)==0 && volMEMBYTE(0x10FDB4)==2) // BIOS-USER-MODE == 1 && BIOS-PLAYER-MOD2 = 0 && START_FLAG == 2
	{
		volMEMBYTE(0x10FDAF)=0x02;	// set BIOS-USER-MODE to 2 (GAME), it stops the BIOS_COMPULSION_TIMER
		volMEMBYTE(0x10FDB7)=0x01;	// set BIOS-PLAYER-MOD2 to 2 (P2 Playing), it deactivates P1 START_BUTTON
		clearFixLayer(); 			// clear TITLE screen
	}

	// Player 2 joins the game of Player 1
	if(volMEMBYTE(0x10FDAF)==2 && volMEMBYTE(0x10FDB7)==0 && volMEMBYTE(0x10FDB4)==2) // BIOS-USER-MODE == 2 && BIOS-PLAYER-MOD1 == 0  && START_FLAG == 2
	{
		volMEMBYTE(0x10FDB7)=0x01;	// set BIOS-PLAYER-MOD2 to 1 (P2 Playing), it deactivates P1 START_BUTTON
	}
}

// demo end (MVS only)  //////////////////////////////////////////////////////////////

void DEMO_END(void) // is called by MVS BIOS if user has inserted a coin and exits the DEMO
{
	// end demo (clear display / update status for sram save / whatever needed)
	// fixPrintf(15, 4,0,0,"DEMO END");
	clearFixLayer();
}

// coin sound (MVS only)  ////////////////////////////////////////////////////////////

void COIN_SOUND(void) // is called by MVS BIOS if user has inserted a coin
{
	(*((PBYTE)0x320000)) = (0x14); //play coin sound
}









// conversion ////////////////////////////////////////////////////////////////////////

int convertHexToDecimal(int hexadecimal)
{
	int decimal=0;

	decimal=hexadecimal-(hexadecimal/16)*6;

return decimal;
}

void convertDecimaltoHex(int decimal)
{
	// int decimal=1600,
	int hexadecimal=0, decimal_calculated=0;

	// char zeichen;
	// zeichen = 'A';
	// zeichen = 66;
	//	fixPrintf(2, 17,0,0, "Zeichen: %c\0", zeichen);

	hexadecimal=decimal+(decimal/10)*6;
	decimal_calculated=hexadecimal-(hexadecimal/16)*6;

	fixPrintf(2, 14,0,0,"Convert dec to hex");
//	fixPrintf(2, 16,0,0,"0x0 hex input: %06d",0x5A);
	fixPrintf(2, 17,0,0,"decimal input: %06d", decimal);
	fixPrintf(2, 18,0,0,"dec hex output: %06d", hexadecimal);
	fixPrintf(2, 19,0,0,"decimal output: %06d", decimal_calculated);
}

// diplay ranking and soft dips ////////////////////////////////////////////////////

void displayRanking(void)
{
	int i;
	int letter1, letter2, letter3;
//	waitVBlank();

//	fixPrintf(24,13,0,0,"BKP-0 %08d", bkp_data.save_slot[0]);
//	fixPrintf(24,14,0,0,"BKP-1 %08d", bkp_data.save_slot[1]);
//	fixPrintf(24,15,0,0,"BKP-2 %08d", bkp_data.save_slot[2]);
//	fixPrintf(24,16,0,0,"BKP-3 %08d", bkp_data.save_slot[3]);
//	fixPrintf(24,17,0,0,"BKP-4 %08d", bkp_data.save_slot[4]);
//	fixPrintf(24,18,0,0,"BKP-5 %08d", bkp_data.save_slot[5]);
//	fixPrintf(24,19,0,0,"BKP-6 %08d", bkp_data.save_slot[6]);
//	fixPrintf(24,20,0,0,"BKP-7 %08d", bkp_data.save_slot[7]);
//	fixPrintf(24,21,0,0,"BKP-8 %08d", bkp_data.save_slot[8]);
//	fixPrintf(24,22,0,0,"BKP-9 %08d", bkp_data.save_slot[9]);

	fixPrintf(2,12,0,0,"TOP 10 RANKING");

	// print highscores
	for(i=0; i<10; i++)
	{
		fixPrintf( 1,14+i,0,0,"%2d %8d", i+1, bkp_data.save_slot[i]-10000000);
	}

	// print names
	for(i=10; i<20; i++)
	{
		letter1=bkp_data.save_slot[i];
		letter2=bkp_data.save_slot[i];
		letter3=bkp_data.save_slot[i];

		letter1=(letter1-11000000)/10000;
		letter2=((letter2-11000000)-(letter1*10000))/100;
		letter3=(letter3-11000000)-(letter1*10000)-(letter2*100);

		fixPrintf(13,4+i,0,0,"%s%s%s", LetterList[letter1],LetterList[letter2], LetterList[letter3]);
	}
}

void displaySoftDipsMVS(void)
{
}

void displaySoftDipsAES(void)
{
	fixPrintf( 21,12,0,0,"AES SOFT DIP DATA");

	fixPrintf( 21,14,0,0,"LIVES     : %02d", global_data.aes_soft_dip_lives);  // DIP 5
	fixPrintf( 21,15,0,0,"CONTINUES : %02d", global_data.aes_soft_dip_continues);  // DIP 6
	fixPrintf( 21,16,0,0,"DIFFICULTY: %02d", global_data.aes_soft_dip_difficulty);  // DIP 7
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
// seaFighter ////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

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
ushort flipMode=0;
picture demomode_title;
picture seafighter_ship;


void ngsdk_init(void)
{
	pictureInit(&demomode_title, &demomode_seafighter_title, 1, 1, 1, 1,FLIP_NONE);
	palJobPut(1,demomode_seafighter_title.palInfo->count,demomode_seafighter_title.palInfo->data);

	pictureInit(&seafighter_ship, &seafighterh02, 21, 21, x, y,FLIP_NONE);
	palJobPut(21,seafighterh02.palInfo->count,seafighterh02.palInfo->data);

	volMEMWORD(0x401ffe)=0x7022; // background color
	volMEMWORD(0x400002)=0x79BB; // fix layer font color
	volMEMWORD(0x400004)=0x7022; // fix layer background color

	NGSDK_SHOWINFO = false;

}

// demo mode: this routine is called once per VBlank during demo mode
void ngsdk_demomode(int demo_timer, int isMVSorAES, int flash_timer, uchar mvs_demosound)
{

	if (NGSDK_SHOWINFO)
	{
		fixPrintf( 2, 4,0,0,"============ DEMO MODE ============%02d ", mvs_demosound);
		if (isMVSorAES==1) fixPrintf(15, 5,0,0,"** MVS **"); //MVS detected
		if (isMVSorAES==2) fixPrintf(15, 5,0,0,"** AES **"); //AES detected
		fixPrintf( 2, 5,0,0,"TIMER: %02d",	demo_timer/60);


	if(demo_timer==NGSDK_DEMOTIMER_DURATION) // first time in DEMO mode ?
	{
		displayRanking(); 	  // ranking table

		if (isMVSorAES==1) // MVS : display MVS Soft dips
		{
			fixPrintf( 21,12,0,0,"MVS SOFT DIP DATA");
			fixPrintf( 21,14,0,0,"PLAY TIME : %02d:%02d", convertHexToDecimal(volMEMBYTE(0x10FD84)),convertHexToDecimal(volMEMBYTE(0x10FD85))); // DIP 1 and 2
			fixPrintf( 21,15,0,0,"CONT.TIME : %02d:%02d", convertHexToDecimal(volMEMBYTE(0x10FD86)),convertHexToDecimal(volMEMBYTE(0x10FD87))); // DIP 3 and 4
			fixPrintf( 21,16,0,0,"LIVES     : %02d", volMEMBYTE(0x10FD88));  // DIP 5
			fixPrintf( 21,17,0,0,"CONTINUES : %02d", volMEMBYTE(0x10FD89));  // DIP 6
			fixPrintf( 21,18,0,0,"DIFFICULTY: %02d", volMEMBYTE(0x10FD8A));  // DIP 7
			fixPrintf( 21,19,0,0,"DEMO SOUND: %02d", volMEMBYTE(0x10FD8B));  // DIP 8
			fixPrintf( 21,20,0,0,"DEMO SOUND: %02d", mvs_demosound);  // DIP 8
		}
		if (isMVSorAES==2) // AES : display AES Soft dips
		{
			displaySoftDipsAES(); // MVS soft dips
		}

	}
}
	if (isMVSorAES==1) // MVS : display INSERT COIN
	{
		if(flash_timer>(NGSDK_FLASHTIMER_DURATION/2))	fixPrintf(14, 8,0,0,"INSERT COIN");
		else				fixPrintf(14, 8,0,0,"           ");
	};

	if (isMVSorAES==2) // AES : display PRESS START
	{
		if(flash_timer>(NGSDK_FLASHTIMER_DURATION/2))	fixPrintf(14, 8,0,0,"PRESS START");
		else				fixPrintf(14, 8,0,0,"           ");
	}





		p1=volMEMBYTE(P1_CURRENT);
		if(p1&JOY_UP)		  y--;
		if(p1&JOY_DOWN)		y++;
	  if(p1&JOY_LEFT)		x--;
		if(p1&JOY_RIGHT)	x++;

		pictureSetPos(&seafighter_ship, x, y);
		pictureSetPos(&demomode_title, 1, 1);
}
