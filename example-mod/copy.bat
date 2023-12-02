SET "binary=photon-example-mod.dll"
SET "src=..\bin\%binary%"
SET "dest=D:\SteamLibrary\SteamApps\common"

copy /Y "%src%" "%dest%\Portal 2\photon\%binary%"
