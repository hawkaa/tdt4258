.PHONY : all driver-gamepad game driver-gamepad-flash

all: driver-gamepad game

driver-gamepad-flash: 
	make driver-gamepad
	make install
	make flash

game-flash:
	make game
	make install
	make flash


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
