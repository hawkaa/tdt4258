#ifndef DRIVER_GAMEPAD_H_
#define DRIVER_GAMEPAD_H_


#define PLATFORM_MEM_INDEX_GPIO 0
#define PLATFORM_IRQ_INDEX_GPIO_EVEN 0
#define PLATFORM_IRQ_INDEX_GPIO_ODD 1

#define GPIO_PC_BASE 0x40006048

#define GPIO_CTRL 0x00
#define GPIO_MODEL 0x04
#define GPIO_MODEH 0x08
#define GPIO_DOUT 0x0c
#define GPIO_DOUTSET 0x10
#define GPIO_DOUTCLR 0x14
#define GPIO_DOUTTGL 0x18
#define GPIO_DIN 0x1c
#define GPIO_PINLOCKN 0x20

struct port_range {
	int start;
	int end;
};


#endif
