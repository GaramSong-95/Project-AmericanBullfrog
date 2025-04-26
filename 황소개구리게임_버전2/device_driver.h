#include "stm32f10x.h"
#include "option.h"
#include "macro.h"
#include "malloc.h"
#include "lcd.h"

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stdio.h"

// Led.c

extern void LED_Init(void);
extern void LED_Display(unsigned int num);
extern void LED_All_On(void);
extern void LED_All_Off(void);

// Uart.c

#define Uart_Init			Uart1_Init
#define Uart_Send_Byte		Uart1_Send_Byte
#define Uart_Send_String	Uart1_Send_String
#define Uart_Printf			Uart1_Printf

extern void Uart1_Init(int baud);
extern void Uart1_Send_Byte(char data);
extern void Uart1_Send_String(const char *pt);
extern void Uart1_Printf(const char *fmt,...);
extern char Uart1_Get_Char(void);
extern char Uart1_Get_Pressed(void);
extern void Uart1_Get_String(char *string);
extern int Uart1_Get_Int_Num(void);
extern void Uart1_RX_Interrupt_Enable(int en);

// Clock.c

extern void Clock_Init(void);

// Timer.c

extern void TIM2_Delay(int time);
extern void TIM4_Repeat_Interrupt_Enable(int en, int time);

// Asm_Function.s

extern unsigned int __get_IPSR(void);

// Sys_Tick.c

extern void SysTick_OS_Tick(unsigned int msec);

// Jog_Key.c

extern void Jog_Poll_Init(void);
extern int Jog_Get_Pressed_Calm(void);
extern int Jog_Get_Pressed(void);
extern void Jog_Wait_Key_Released(void);
extern int Jog_Wait_Key_Pressed(void);
extern void Jog_ISR_Enable(int en);

#ifdef __cplusplus
}
#endif