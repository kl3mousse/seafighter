# seafighter
one more neogeo homebrew... an **horizontal shooter** you can play on MAME neogeo emulator, or even on arcade MVS board with a NeoSD cadridge able to load roms.

![https://img.shields.io/badge/buildpack-gcc%20for%20win10-blue.svg]()

I'm glad you have found this repo :) note that this is work in progress.
For now I'm trying to get basic things sorted out, starting with a good dev kit, that can be easily reused for another project. Most of the code /game logic is from Sebastian Mihai (Neo Thunder game). I'm mostly focusing on reverse engineering the build/compilation process and eventually tune it a bit (add sound? add makefile? dev tools?).

Contributions and enhancements are welcome. Please create a pull request.

*Thanks to the neogeo scene (Sebastian Mihai, HPman, Furrtek, dciabrin, freem/AJ, Blastar, Vasiliy Familiya, Smkdan, Chris Covell...) for the assets & examples :)*

# key programs
- run_me_first.bat: will setup Windows PATH to be able to access compilers and libraries for your C/ASM code into ROMs
- build_Cartridge.bat will compile your code and build roms
- testwithMAME.bat will launch your ROM with MAME

# latest updates (latest updates first)
- added NGFX tool for sprites/graphic/fix editing
- dedicated MAME hash setup in order to test as a homebrew Rom, not as a puzzledp clone. Unfortunately this now makes the Sound driver crash for some reason (to be fixed later)
- now also compile in NeoSD format also as part of the make process
- added 7zip and a zip version of MAME64 so that MAME is included. Now possible to test just by cloning the Git repo (dont forget to excecute 'run_me_first').
- included own Sound roms (M1/V1) made with Neo Sound Builder: soon I can get rid off proprietary code from Puzzle De Pon
- included Neo Sound Builder tool (from NeoBitz)
- ability for player 1 to choose fighter when game starts
- added XML content for MAME (thanks to Vasiliy Familiya)
- added SRC folder for programs
- removed all files related to NeoGeo CD. Focusing on AES/MVS only.
- added "gfx" folder to store all bitmaps separately
- MAME with NeoGeo Unibios added to be able to test
- moved NeoDev as a library

# build cartridge process

![Build process](./doc/build_process.PNG?raw=true "Build process")
- Black circles: source files
- Blue/Green circles: temp files created during BUILD/LINKING build_process
- Purple circles: Rom files for MAME
(need to add sound building process, NeoSD & ZIP output)

# includes and other dependancies
- NeoDev / NeoBitz
  - gcc version 2.95.2
  - version has been modified to map the new project structure (moving the game out of "src" folder, and also trying to get rid off all Win10 PATH and other environment variables
- Neo Sound Builder v008 by NeoBitz
- NGFX by Blastar (https://blastar.citavia.de)
- DATlib 0.3 by HPman (included in subdirectories, not yet in game)
- MAME v0.197 (http://mamedev.org/)
- Unibios 3.2 (http://unibios.free.fr)
- NEO THUNDER modified game code (http://sebastianmihai.com/main.php?t=22)
- Neo Builder v1.06 (https://www.terraonion.com/en/products)
- 7zip (to Zipbin files in a ZIP rom, then convert into NeoSD format)

"NEOGEO" is a registered trademark of SNK Playmore Corp. All other company and product names used are trademarks or registered trademarks of their respective owners.
The contents of this page are not authorized or licensed by SNK Playmore.
