
setx neodev %cd%\include\NeoDev
.\tools\pathed\pathed /user /add "%cd%\include\NeoDev\m68k\bin"
.\tools\7zip\7za e .\tools\mame\mame64.exe.zip -o.\tools\mame\ 
pause
