
@set MAMEDIR=%cd%\tools\MAME
@set PROJECTDIR=%cd%

cd %MAMEDIR%

@rem mame64 NEOGEO SEAFIGHTER -bios unibios32
mame64 neogeo -cart1 seafight -bios unibios33

cd %PROJECTDIR%
