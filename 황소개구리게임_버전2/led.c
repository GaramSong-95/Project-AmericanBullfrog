#include "device_driver.h"

void LED_Init(void)
{
	Macro_Set_Bit(RCC->APB2ENR, 3);
	Macro_Write_Block(GPIOB->CRH, 0xff, 0x66, 0);
	Macro_Set_Area(GPIOB->ODR, 0x3, 8);
}

void LED_Display(unsigned int num)
{
	Macro_Write_Block(GPIOB->ODR, 0x3, (~num & 3), 8);
}

void LED_All_On(void)
{
	Macro_Clear_Area(GPIOB->ODR, 0x3, 8);
}

void LED_All_Off(void)
{
	Macro_Set_Area(GPIOB->ODR, 0x3, 8);
}



