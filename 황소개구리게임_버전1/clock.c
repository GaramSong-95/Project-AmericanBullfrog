#include "device_driver.h"

void Clock_Init(void)
{
	RCC->CR = 0x1;
	Macro_Set_Bit(RCC->CR, 16);
	RCC->CFGR = ((9-2)<<18)|(0<<17)|(1<<16)|(0<<11)|(4<<8)|(0<<4)|(0<<0);
	Macro_Set_Bit(RCC->CR, 24);
	while(!Macro_Check_Bit_Set(RCC->CR, 25));
    Macro_Write_Block(FLASH->ACR, 0x3, 0x2, 0);
	Macro_Write_Block(RCC->CFGR, 0x3, 0x2, 0);
}
