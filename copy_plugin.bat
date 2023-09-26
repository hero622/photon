SET "binary=wormhole-example-plugin.dll"
SET "src=..\bin\%binary%"
SET "dest=D:\SteamLibrary\SteamApps\common"

copy /Y "%src%" "%dest%\Portal 2\wormhole\%binary%"