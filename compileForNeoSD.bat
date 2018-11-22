@rem this script needs to be changed to source roms files from the right folder. For now, you need to zip your BIN files and store the ZIP as "seafighter.zip"

cd output\cartridge
del ..\rom\seafighter.zip
..\..\tools\7zip\7za a ..\rom\seafighter.zip *.bin
cd ..\..\tools\neosd-neoBuilder
NeoBuilder -n seafighter -m klemousse -y 2018 -g Shooter -s 444 ..\..\output\rom\seafighter.zip
move seafighter.neo ..\..\output\neoSD
cd ..\..
