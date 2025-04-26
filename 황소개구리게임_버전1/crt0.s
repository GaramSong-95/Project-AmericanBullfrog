	.syntax unified
	.thumb

	.text

    .extern   SVC_Handler
	.extern   NMI_Handler
	.extern   DebugMon_Handler
	.extern   PendSV_Handler
	.extern   SysTick_Handler
	.extern	  Invalid_ISR

    .extern   _HardFault_Handler
    .extern   _MemManage_Handler
    .extern   _BusFault_Handler
    .extern   _UsageFault_Handler

	.extern   EXTI3_IRQHandler
	.extern   EXTI9_5_IRQHandler
	.extern   USART1_IRQHandler
	.extern   TIM4_IRQHandler
	.extern   EXTI15_10_IRQHandler

	.word	  0x20005000
	.word	  __start

	.word     NMI_Handler               @ NMI Handler
	.word     _HardFault_Handler       	@ Hard Fault Handler
	.word     _MemManage_Handler       	@ MPU Fault Handler
	.word     _BusFault_Handler       	@ Bus Fault Handler
	.word     _UsageFault_Handler      	@ Usage Fault Handler
	.word     Invalid_ISR               @ Reserved
	.word     Invalid_ISR               @ Reserved
	.word     Invalid_ISR               @ Reserved
	.word     Invalid_ISR               @ Reserved
	.word     SVC_Handler              	@ SVCall Handler
	.word     Invalid_ISR               @ Reserved
	.word     Invalid_ISR               @ Reserved
	.word     PendSV_Handler            @ PendSV Handler
	.word     SysTick_Handler           @ SysTick Handler

	.word     Invalid_ISR           	@ Window Watchdog
	.word     Invalid_ISR            	@ PVD through EXTI Line detect
	.word     Invalid_ISR         		@ Tamper
	.word     Invalid_ISR         		@ RTC
	.word     Invalid_ISR         		@ Flash
	.word     Invalid_ISR         		@ RCC
	.word     Invalid_ISR         		@ EXTI Line 0
	.word     Invalid_ISR         		@ EXTI Line 1
	.word     Invalid_ISR         		@ EXTI Line 2
	.word     EXTI3_IRQHandler     		@ EXTI Line 3
	.word     Invalid_ISR         		@ EXTI Line 4
	.word     Invalid_ISR         		@ DMA1 Channel 1
	.word     Invalid_ISR         		@ DMA1 Channel 2
	.word     Invalid_ISR         		@ DMA1 Channel 3
	.word     Invalid_ISR         		@ DMA1 Channel 4
	.word     Invalid_ISR         		@ DMA1 Channel 5
	.word     Invalid_ISR         		@ DMA1 Channel 6
	.word     Invalid_ISR         		@ DMA1 Channel 7
	.word     Invalid_ISR         		@ ADC1 & ADC2
	.word     Invalid_ISR         		@ USB High Priority or CAN1 TX
	.word     Invalid_ISR         		@ USB Low  Priority or CAN1 RX0
	.word     Invalid_ISR         		@ CAN1 RX1
	.word     Invalid_ISR         		@ CAN1 SCE
	.word     EXTI9_5_IRQHandler        @ EXTI Line 9..5
	.word     Invalid_ISR         		@ TIM1 Break
	.word     Invalid_ISR         		@ TIM1 Update
	.word     Invalid_ISR         		@ TIM1 Trigger and Commutation
	.word     Invalid_ISR         		@ TIM1 Capture Compare
	.word     Invalid_ISR         		@ TIM2
	.word     Invalid_ISR         		@ TIM3
	.word     TIM4_IRQHandler           @ TIM4
	.word     Invalid_ISR         		@ I2C1 Event
	.word     Invalid_ISR         		@ I2C1 Error
	.word     Invalid_ISR         		@ I2C2 Event
	.word     Invalid_ISR         		@ I2C2 Error
	.word     Invalid_ISR         		@ SPI1
	.word     Invalid_ISR         		@ SPI2
	.word     USART1_IRQHandler       	@ USART1
	.word     Invalid_ISR         		@ USART2
	.word     Invalid_ISR         		@ USART3
	.word     EXTI15_10_IRQHandler 		@ EXTI Line 15..10
	.word     Invalid_ISR         		@ RTC Alarm through EXTI Line
	.word     Invalid_ISR         		@ USB Wakeup from suspend

	.global	__start
  	.type 	__start, %function
__start:

	.extern __RO_LIMIT__
	.extern __RW_BASE__
	.extern __ZI_BASE__
	.extern __ZI_LIMIT__

	ldr		r0, =__RO_LIMIT__
	ldr		r1, =__RW_BASE__
	ldr		r3, =__ZI_BASE__

	cmp		r0, r1
	beq		2f

1:
	cmp		r1, r3
	ittt	lo
	ldrlo	r2, [r0], #4
	strlo	r2, [r1], #4
	blo		1b

2:
	ldr		r1, =__ZI_LIMIT__
	mov		r2, #0x0
3:
	cmp		r3, r1
	itt		lo
	strlo	r2, [r3], #4
	blo		3b

	.extern Main

	.equ RAM_END,	(0x20004FFF)
	.equ PSP_BASE,	(MSP_LIMIT)
	.equ MSP_BASE,	(RAM_END + 1)
	.equ MSP_SIZE,	(1*1024)
	.equ MSP_LIMIT,	(MSP_BASE - MSP_SIZE)

	ldr		r0, =PSP_BASE
	msr		psp, r0
	ldr		r0, =(0x1<<1)|(0x0<<0)
	msr		control, r0

	bl		Main

	b		.
			
	.end
