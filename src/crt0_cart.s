********************* Vector Definitions *********************
_IRQ1	=	DAT_TIfunc
_IRQ2	= 	DAT_vblankTI
_IRQ3	=	_irq3_handler
_ENTRY_USER	= _entry_user
_ENTRY_PLAYER_START	= _entry_player_start
_ENTRY_DEMO_END	= _entry_demo_end |; MVS game switch
_ENTRY_COIN_SOUND	= _entry_coin_sound |; coin sound

************************ Definitions *************************
_NGH = 0x5555
_PROGRAM_SIZE	=	0x00100000
_WRK_BCKP_AREA = bkp_data
_WRK_BCKP_AREA_SIZE = 0x0100	|;256bytes
_EYE_CATCHER = 0x02	/*;eye catcher (0-common 1-custom 2-off)*/
_EYE_CATCHER_TILES = 0x01 /*;eye catcher start tiles (upper bits, 0x01 => 0x0100)*/

.include "./src/common_crt0_cart.s"

* Names MUST be 16 characters long
*           <---------------->
JPConfig:
 	.ascii	"GAME-TITLE-JP---"

  	.byte	0x10 /* PLAY TIME, default 10 min */
  	.byte	0x00 /* PLAY TIME, default  0 sec */
  	.byte	0x00 /* CONT.TIME, default  0 min */
  	.byte	0x10 /* CONT.TIME, default 10 sec */
 	.byte	0x05 /* LIVES 99 max, default 5 */
 	.byte	0x64 /* CONTINUES 01-63, infinite=64, without=00, default 64 */

 	.byte	0x38 /* DIFFICULTY, 8 options, 3 default*/
 	.byte	0x02 /* DEMO SOUND, 2 options, 0 default*/

 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
  	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */

 	.ascii	"PLAY TIME   "
 	.ascii	"CONT.TIME   "
 	.ascii	"LIVES       "
 	.ascii	"CONTINUES   "

 	.ascii	"DIFFICULTY  "
 	.ascii	"LEVEL 1     "
 	.ascii	"LEVEL 2     "
 	.ascii	"LEVEL 3     "
 	.ascii	"LEVEL 4     "
 	.ascii	"LEVEL 5     "
 	.ascii	"LEVEL 6     "
 	.ascii	"LEVEL 7     "
 	.ascii	"LEVEL 8     "

 	.ascii	"DEMO SOUND  "
 	.ascii	"WITH        "
 	.ascii	"WITHOUT     "

USConfig:
	.ascii	"GAME-TITLE-US---"

  	.byte	0x10 /* PLAY TIME, default 10 min */
  	.byte	0x00 /* PLAY TIME, default  0 sec */
  	.byte	0x00 /* CONT.TIME, default  0 min */
  	.byte	0x10 /* CONT.TIME, default 10 sec */
 	.byte	0x05 /* LIVES 99 max, default 5 */
 	.byte	0x64 /* CONTINUES 01-63, infinite=64, without=00, default 64 */

 	.byte	0x38 /* DIFFICULTY, 8 options, 3 default*/
 	.byte	0x02 /* DEMO SOUND, 2 options, 0 default*/

 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
  	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */

 	.ascii	"PLAY TIME   "
 	.ascii	"CONT.TIME   "
 	.ascii	"LIVES       "
 	.ascii	"CONTINUES   "

 	.ascii	"DIFFICULTY  "
 	.ascii	"LEVEL 1     "
 	.ascii	"LEVEL 2     "
 	.ascii	"LEVEL 3     "
 	.ascii	"LEVEL 4     "
 	.ascii	"LEVEL 5     "
 	.ascii	"LEVEL 6     "
 	.ascii	"LEVEL 7     "
 	.ascii	"LEVEL 8     "

 	.ascii	"DEMO SOUND  "
 	.ascii	"WITH        "
 	.ascii	"WITHOUT     "

EUConfig:
 	.ascii	"GAME-TITLE-EU---"

  	.byte	0x10 /* PLAY TIME, default 10 min */
  	.byte	0x00 /* PLAY TIME, default  0 sec */
  	.byte	0x00 /* CONT.TIME, default  0 min */
  	.byte	0x10 /* CONT.TIME, default 10 sec */
 	.byte	0x05 /* LIVES 99 max, default 5 */
 	.byte	0x64 /* CONTINUES 01-63, infinite=64, without=00, default 64 */

 	.byte	0x38 /* DIFFICULTY, 8 options, 3 default*/
 	.byte	0x02 /* DEMO SOUND, 2 options, 0 default*/

 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
  	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */
 	.byte	0x00 /* UNUSED OPTION CATEGORY */

 	.ascii	"PLAY TIME   "
 	.ascii	"CONT.TIME   "
 	.ascii	"LIVES       "
 	.ascii	"CONTINUES   "

 	.ascii	"DIFFICULTY  "
 	.ascii	"LEVEL 1     "
 	.ascii	"LEVEL 2     "
 	.ascii	"LEVEL 3     "
 	.ascii	"LEVEL 4     "
 	.ascii	"LEVEL 5     "
 	.ascii	"LEVEL 6     "
 	.ascii	"LEVEL 7     "
 	.ascii	"LEVEL 8     "

 	.ascii	"DEMO SOUND  "
 	.ascii	"WITH        "
 	.ascii	"WITHOUT     "

 	.end
