#include "device_driver.h"

#define TIM2_TICK			(20) 					// usec
#define TIM2_FREQ			(1000000/TIM2_TICK) 	// Hz
#define TIME2_PLS_OF_1ms	(1000/TIM2_TICK)
#define TIM2_MAX			(0xffffu)

void TIM2_Delay(int time)
{
	int i;
	unsigned int t = TIME2_PLS_OF_1ms * time;

	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->PSC = (unsigned int)(TIMXCLK/(double)TIM2_FREQ + 0.5)-1;
	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->ARR = 0xffff;
	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Set_Bit(TIM2->DIER, 0);

	for(i=0; i<(t/0xffff); i++)
	{
		Macro_Set_Bit(TIM2->EGR,0);
		Macro_Clear_Bit(TIM2->SR, 0);
		Macro_Set_Bit(TIM2->CR1, 0);
		while(Macro_Check_Bit_Clear(TIM2->SR, 0));
	}

	TIM2->ARR = t % 0xffff;
	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Clear_Bit(TIM2->SR, 0);
	Macro_Set_Bit(TIM2->CR1, 0);
	while (Macro_Check_Bit_Clear(TIM2->SR, 0));

	Macro_Clear_Bit(TIM2->CR1, 0);
	Macro_Clear_Bit(TIM2->DIER, 0);
	Macro_Clear_Bit(RCC->APB1ENR, 0);
}

#define TIM4_TICK	  (20) 							// usec
#define TIM4_FREQ 	  (1000000/TIM4_TICK) 			// Hz
#define TIME4_PLS_OF_1ms  (1000/TIM4_TICK)
#define TIM4_MAX	  (0xffffu)

void TIM4_Repeat_Interrupt_Enable(int en, int time)
{
  if(en)
  {
    Macro_Set_Bit(RCC->APB1ENR, 2);

    TIM4->CR1 = (1<<4)+(0<<3)+(0<<0);
    TIM4->PSC = (unsigned int)(TIMXCLK/(double)TIM4_FREQ + 0.5)-1;
    TIM4->ARR = TIME4_PLS_OF_1ms * time;

    Macro_Set_Bit(TIM4->EGR,0);
    Macro_Set_Bit(TIM4->SR, 0);
    NVIC_ClearPendingIRQ((IRQn_Type)30);
    Macro_Set_Bit(TIM4->DIER, 0);
    NVIC_EnableIRQ((IRQn_Type)30);
    Macro_Set_Bit(TIM4->CR1, 0);
  }

  else
  {
    NVIC_DisableIRQ((IRQn_Type)30);
    Macro_Clear_Bit(TIM4->CR1, 0);
    Macro_Clear_Bit(TIM4->DIER, 0);
    Macro_Clear_Bit(RCC->APB1ENR, 2);
  }
}
