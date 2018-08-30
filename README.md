# seafighter
one more neogeo homebrew... an **horizontal shooter** you can play on MAME neogeo emulator, or even on arcade MVS board with a NeoSD cadridge able to load roms.

I'm glad you have found this repo :) note that this is work in progress.
For now I'm trying to get basic things sorted out, starting with a good dev kit, that can be easily reused for another project.

*Thanks to the neogeo scene (Sebastian Mihai, HPman, Furrtek, dciabrin, freem/AJ, Vasiliy Familiya...) for the assets & examples :)*

# key programs
- run_me_first.bat: will setup Windows PATH to be able to access compilers and libraries for your C/ASM code into ROMs
- build_Cartridge.bat will compile your code and build roms
- testwithMAME.bat will launch your ROM with MAME
- compileForNeoSD.bat will create a rom in NEO format in order to load it into a NeoSD card

# latest updates
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

# includes and other dependancies
- NeoDev / NeoBitz
  - gcc version 2.95.2
  - version has been modified to map the new project structure (moving the game out of "src" folder, and also trying to get rid off all Win10 PATH and other environment variables
- DATlib 0.3 by HPman (included in subdirectories, not yet in game)
- MAME v0.197 (http://mamedev.org/) ****soon
- Unibios 3.2 (http://unibios.free.fr)
- NEO THUNDER modified game code (http://sebastianmihai.com/main.php?t=22)
- Neo Builder v1.06 (https://www.terraonion.com/en/products)

"NEOGEO" is a registered trademark of SNK Playmore Corp. All other company and product names used are trademarks or registered trademarks of their respective owners.
The contents of this page are not authorized or licensed by SNK Playmore.
