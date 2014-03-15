.PHONY : all driver-gamepad game

all: driver-gamepad game


driver-gamepad :
	
	ptxdist clean driver-gamepad
	ptxdist compile driver-gamepad

game :
	ptxdist clean game
	ptxdist compile game

install :
	ptxdist go


flash : 
	ptxdist image root.romfs
	ptxdist test flash-rootfs
