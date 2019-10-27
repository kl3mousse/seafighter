# seafighter
![](https://img.shields.io/badge/devkit%20platform-windows-ff69b4.svg)
![](https://img.shields.io/badge/game%20platform-MAME-blue.svg)
![](https://img.shields.io/badge/game%20platform-NeoSD%20(real%20MVS%20or%20AES)-blue.svg)

This project started first as a fork of Neo Thunder game (see includes below), then finally integrating DatLib SDK more than building a game engine.
Latest target it to provide a NeoGeo mode API that would simplify the various modes the NeoGeo is providing (Demo mode, Title mode, Game mode with all the possible variations a developer has to handle to have the game working on both Console and Arcade modes). Part (most) of the code comes from NeoHomeBrew and HPman.

The game is not working anymore, need to work on it.

![https://img.shields.io/badge/buildpack-gcc%20for%20win10-blue.svg]()

*Thanks to the neogeo scene (Sebastian Mihai, HPman, Furrtek, dciabrin, freem/AJ, Blastar, Vasiliy Familiya, Smkdan, Chris Covelln NeoHomebrew...) for the assets & examples :)*

# key programs
- make (rom)				# compile the whole
- make Install			# install for Windows
- make gfx					# rebuild Graphics
- make fix_gfx			# rebuild FIX rom
- make clean				# remove temp files
- make play         # launch MAME

# latest updates (latest updates first)
- Demo/Title modes APIs ok (that was the easy part)
- initiated a simple makefile (not expert of make automation, but at least newbies can understand)
- started to break the whole thing to include DATlib from HPman. This requires to change the make/build process (wip). Fix & C1/C2 files build process OK.
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


# includes and other dependancies
- NeoDev / NeoBitz
  - gcc version 2.95.2
  - version has been modified to map the new project structure (moving the game out of "src" folder, and also trying to get rid off all Win10 PATH and other environment variables
- Neo Sound Builder v008 by NeoBitz
- NGFX by Blastar (https://blastar.citavia.de)
- DATlib 0.3 by HPman
- MAME v0.197 (http://mamedev.org/)
- Unibios 3.2 (http://unibios.free.fr)
- NEO THUNDER modified game code (http://sebastianmihai.com/main.php?t=22)
- Neo Builder v1.06 (https://www.terraonion.com/en/products)
- 7zip (to Zipbin files in a ZIP rom, then convert into NeoSD format)

# Questions
- it this a game? I dont think SeaFighter repository will become a game. I will have to rename it to something like NeoGeo GameModes API.
- is this a NeoGeo SDK ? no, just a library to create an abstraction layer between NeoGeo and your Game code for the Game modes. Hopefully it can work with the SDKs you know. I will work on it once (if) it works

"NEOGEO" is a registered trademark of SNK Playmore Corp. All other company and product names used are trademarks or registered trademarks of their respective owners.
The contents of this page are not authorized or licensed by SNK Playmore.
