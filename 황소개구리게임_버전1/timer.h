#include "device_driver.h"

#ifdef __cplusplus
 extern "C" {
#endif 

#define TIM2_TICK         	(20) 				// usec
#define TIM2_FREQ 	  		(1000000/TIM2_TICK)	// Hz
#define TIME2_PLS_OF_1ms  	(1000/TIM2_TICK)
#define TIM2_MAX	  		(0xffffu)

#define TIM4_TICK	  		(20) 				// usec
#define TIM4_FREQ 	  		(1000000/TIM4_TICK) // Hz
#define TIME4_PLS_OF_1ms  	(1000/TIM4_TICK)
#define TIM4_MAX	  		(0xffffu)

class TIMER
{
public:
	virtual void Stopwatch_Start() = 0;
	virtual unsigned int Stopwatch_Stop() = 0;
	virtual void Delay(int time) = 0;
	// virtual void Repeat(int time) = 0;
	// virtual int Check_Timeout(void) = 0;
	// virtual void Stop(void) = 0;
	// virtual void Change_Value(int time) = 0;
};

class TIMER2 : public TIMER
{
public:	
	TIMER2()
	{
		Macro_Set_Bit(RCC->APB1ENR, 0);	
		TIM2->PSC = (unsigned int)(TIMXCLK/(double)TIM2_FREQ + 0.5)-1;
	}

	// ~TIMER2()
	// {
	// 	Macro_Clear_Bit(RCC->APB1ENR, 0);	
	// }

	void Stopwatch_Start();
	unsigned int Stopwatch_Stop();
	void Delay(int time);

};

#ifdef __cplusplus
}
#endif