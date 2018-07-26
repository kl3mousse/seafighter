NeoBuilderUI, NeoBuilder & NeoValidator

****IMPORTANT NOTICE. IN ORDER TO RUN NEOBUILDER OR NEOBUILDER UI, YOU'LL NEED VISUAL STUDIO 2015 C++ UPDATE 3 32-BIT REDISTRIBUTABLE.  THEY REQUIRE THE 32-BIT RUNTIME EVEN ON 64-BIT MACHINES AND OS*****

NeoBuilder is the main application used to convert romsets from .zip files to NeoSD .neo format. Neo format contains the same romset data but uncompressed and in a linear way, avoiding rom data interleaving or swap around. Also data is decrypted to simplify the rom loading task in NeoSD.

You can convert your romsets with 2 methods:

NeoBuilderUI

It's a simple UI over the NeoBuilder command line application. Just select your romsets directory and hit start. It will process all files and report which romsets are right, wrong or missing from your list. NeoBuilderUI will skip converting files if a .neo file is already present with the same name, so in order to rebuild a romset, you'll need to delete the .neo file.

NeoBuilder

It's the core application that performs the actual conversion.

Usage:
	Converting all MAME zip files in a directory: NeoBuilder -d Directory_absolute_path
	Converting a MAME zip file: NeoBuilder zipfile_absolute_path
	Converting a generic zip file: NeoBuilder -n GameName -m developer -y year -g genre -s screenshotnum zipfile_absolute_path

When using the -d option, NeoBuilder will skip converting files if a .neo file is already present with the same name, so in order to rebuild a romset, you'll need to delete the .neo file. All other options will process the romset and overwrite any .neo files with the same name.

The generic zip conversion will take a NeoGeo romset in a zip file and will try to guess the loading order and function or each file based on its name, so it must follow the usual romset names. It can recognize several naming patterns but the recommended is:
  xxxx-p1,2.bin for program data (max 1+8MB)
  xxxx-s1.bin for FIX (tiles) data (max 512KB, CMC42 style banking is enabled by NeoSD by default for unknown games)
  xxxx-m1.bin for Z80 program (max 256KB)
  xxxx-v1,2,3,4.bin for ADPCM samples (max 16MB). Combined ADPCM-A and ADPCM-B regions as if NEO-PCM chip was present in the cart.
  xxxx-c1,2,34,5,6,7,8.bin for sprites (max 64MB)

NeoValidator

It's a small tool that will analyze your .neo files and will compare them against a list of known .neo files. It will report errors for unknown .neo files (that's normal) or known .neo files that don't match the known checksum (you'll need to rebuild/redump them)


Building your own neo tools

Neo files are just a header, and then all the uncompressed, decrypted rom data, so you can build your own tools to create or manage them. The header length is exactly 4KB, although it's mostly unused, reserved for future expansion:
    struct NeoFile
    {
        uint8_t header1, header2, header3, version;
        uint32_t PSize, SSize, MSize, V1Size, V2Size, CSize;
        uint32_t Year;
        uint32_t Genre;
        uint32_t Screenshot;
	uint32_t NGH;
        uint8_t Name[33];
        uint8_t Manu[17];
        uint8_t Filler[128 + 290];	//fill to 512
	uint8_t Filler2[4096 - 512];	//fill to 4096
    }

Then the rom data follows as is, uncompressed and decrypted in the order they appear in the header: P,S,M,V1,V2,C, according to the size specified in the header.

If your game contains merged ADPCM-A and B areas (neo-pcm), then V2Size must be 0. Separate ADPCM areas is not supported for custom/homebrew games yet.
As you can see, any area with a size of 0 will actually be skipped by neosd when loading, so you can quickly test, for example, program changes without rewritting the other areas, by creating a neo with only P data, filled PSize and 0 for the rest of the areas.

The screenshot to game mapping can be checked in order.txt
Genre ids are the following (starting in 0, ascending value):
enum Genre { Other=0, Action, BeatEmUp, Sports, Driving, Platformer, Mahjong, Shooter, Quiz, Fighting, Puzzle};


FAQs
Q: When converting romsets, NeoBuilderUI shows some with the following error: "Internal Error, skipped"
A: Check that you have the proper redistributable installed. It must be the 32-bit version although your machine is 64-bit. If it still shows the error, try redumping/obtainig the romset from a different place.

Q: Why is kf2003pcb listed as unknown?
A: kf2003pcb requires a special bios to run. This bios contains code beyond the normal bios address space, so it will cause resets/address error/invalid opcode exceptions when run in normal boards. It's not possible to install a larger bios (27c4096 for example) in a standard MVS boards because the extra address lines are not connected (tied to 1 actually). In order to make it run, the game code will need to be patched, the bios code injected in the main game program and relocations fixed. We may study adding it at a later time.

Q: Why is kof10th listed as unknown?
A: kof10th is a bootleg that uses RAM based code and RAM based tiles and is not currently supported.










