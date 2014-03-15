.PHONY : all driver-gamepad game

all: driver-gamepad game


driver-gamepad :
	
	ptxdist clean driver-gamepad
	ptxdist compile driver-gamepad
	ptxdist install driver-gamepad
	ptxdist targetinstall driver-gamepad

game :
	ptxdist clean game
	ptxdist compile game
	ptxdist install game
	ptxdist targetinstall game

flash : 
	ptxdist image root.romfs
	ptxdist test flash-rootfs
