#include "device_driver.h"

void Invalid_ISR(void)
{
	Uart1_Printf("Invalid_Exception: %d!\n", Macro_Extract_Area(SCB->ICSR, 0x1ff, 0));
	Uart1_Printf("Invalid_ISR: %d!\n", Macro_Extract_Area(SCB->ICSR, 0x1ff, 0) - 16);
	for(;;);
}

static char * const Stack_reg[] = {"R0","R1","R2","R3","R12","LR","RA","xPSR"};

static void Stack_Dump(const char * stack, unsigned int * sp)
{
	int i;

	for(i=0; i<(sizeof(Stack_reg)/sizeof(Stack_reg[0])); i++)
	{
		Uart1_Printf("%s[%d],%s=0x%.8X\n", stack, i, Stack_reg[i], sp[i]);
	}
}

static void Fault_Report(unsigned int lr, unsigned int * msp, unsigned int * psp, unsigned int psr)
{
	Uart1_Printf("LR(EXC_RETURN)=0x%.8X\n", lr);
	Uart1_Printf("MSP=0x%.8X\n", msp);
	Uart1_Printf("PSP=0x%.8X\n", psp);
	Uart1_Printf("PSR=0x%.8X\n", psr);

	Uart1_Printf("SHCSR: 0x%.8X\n", SCB->SHCSR);
	Uart1_Printf("ICSR: 0x%.8X\n", SCB->ICSR);
	Uart1_Printf("CFSR: 0x%.8X\n", SCB->CFSR);

	switch((lr & (0xF<<28))|(lr & 0xF))
	{
		case 0xF0000001: Uart1_Printf("Handler(MSP)\n"); Stack_Dump("MSP", msp); break;
		case 0xF0000009: Uart1_Printf("Thread(MSP)\n"); Stack_Dump("MSP", msp); break;
		case 0xF000000d: Uart1_Printf("Thread(PSP)\n"); Stack_Dump("PSP", psp); break;
		default: Uart1_Printf("Invalid exception return value => %#.8X\n", lr & 0xf); break;
	}
}

void HardFault_Handler(unsigned int lr, unsigned int * msp, unsigned int * psp, unsigned int psr)
{
	Uart1_Printf("Hard Fault!\n");
	Fault_Report(lr, msp, psp, psr);
	Uart1_Printf("HFSR: 0x%.8X\n", SCB->HFSR);
	Uart1_Printf("BFAR Valid => %d\n", Macro_Check_Bit_Set(SCB->CFSR, 15));
	Uart1_Printf("BFAR: 0x%.8X\n", SCB->BFAR);
	Uart1_Printf("MMFAR Valid => %d\n", Macro_Check_Bit_Set(SCB->CFSR, 7));
	Uart1_Printf("MMFAR: 0x%.8X\n", SCB->MMFAR);
	for(;;);
}

void MemManage_Handler(unsigned int lr, unsigned int * msp, unsigned int * psp, unsigned int psr)
{
	Uart1_Printf("Memory Management Fault!\n");
	Fault_Report(lr, msp, psp, psr);
	Uart1_Printf("MMFAR Valid => %d\n", Macro_Check_Bit_Set(SCB->CFSR, 7));
	Uart1_Printf("MMFAR: 0x%.8X\n", SCB->MMFAR);
	for(;;);
}

void BusFault_Handler(unsigned int lr, unsigned int * msp, unsigned int * psp, unsigned int psr)
{
	Uart1_Printf("Bus Fault!\n");
	Fault_Report(lr, msp, psp, psr);
	Uart1_Printf("BFAR Valid => %d\n", Macro_Check_Bit_Set(SCB->CFSR, 15));
	Uart1_Printf("BFAR: 0x%.8X\n", SCB->BFAR);
	for(;;);
}

void UsageFault_Handler(unsigned int lr, unsigned int * msp, unsigned int * psp, unsigned int psr)
{
	Uart1_Printf("Usage Fault!\n");
	Fault_Report(lr, msp, psp, psr);
	for(;;);
}

void NMI_Handler(void)
{
	Invalid_ISR();
}

void PendSV_Handler(void)
{
	Invalid_ISR();
}

void SysTick_Handler(void)
{
	Invalid_ISR();
}

void SVC_Handler(void)
{
	Invalid_ISR();
}

/* Interrupt ISR Functions */

volatile int TIM4_expired = 0;

void TIM4_IRQHandler(void)
{
	TIM4_expired = 1;
	Macro_Clear_Bit(TIM4->SR, 0);
}

volatile int USART1_rx_ready = 0;
volatile int USART1_rx_data;

void USART1_IRQHandler(void)
{
	USART1_rx_ready = 1;
	USART1_rx_data = Uart1_Get_Pressed();
}

volatile int Jog_key_in = 0;
volatile int Jog_key = 0;

void EXTI3_IRQHandler(void)
{
	// UP
	Jog_key_in = 1;
	Jog_key = 0;
	EXTI->PR = 0x1<<3;
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
}

void EXTI9_5_IRQHandler(void)
{
	// RLD
	static int EXTI9_5_LUT[8] = {0,1,2,0,3,0,0,0};
	Jog_key = EXTI9_5_LUT[Macro_Extract_Area(EXTI->PR,0x7,5)];
	Jog_key_in = 1;
	EXTI->PR = 0x7<<5;
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
}

void EXTI15_10_IRQHandler(void)
{
	// 10
	static int EXTI15_10_LUT[4] = {0,4,5,0};
	Jog_key = EXTI15_10_LUT[Macro_Extract_Area(EXTI->PR, 0x3, 13)];
	Jog_key_in = 1;	
	EXTI->PR = 0x3<<13;
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
}
