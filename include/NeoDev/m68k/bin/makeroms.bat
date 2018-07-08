romwak /f dev_p1.rom 202-p1.bin
romwak /p 202-p1.bin 202-p1.bin 512 255

romwak /w test.spr 202-c1.bin 202-c2.bin
romwak /f 202-c1.bin
romwak /f 202-c2.bin

romwak /p 202-c1.bin 202-c1.bin 512 255
romwak /p 202-c2.bin 202-c2.bin 512 255

copy test.fix 202-s1.bin
romwak /p 202-s1.bin 202-s1.bin 128 255

copy sounds\roms\202-v1.bin
romwak /p 202-v1.bin 202-v1.bin 512 255

copy sounds\roms\202-m1.bin

copy 202-*.bin c:\mame\roms\puzzledp



