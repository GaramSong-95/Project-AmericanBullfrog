#include "device_driver.h"
#include "graphics.h"

#define LCDW			(320)
#define LCDH			(240)
#define X_MIN	 		(0)
#define X_MAX	 		(LCDW - 1)
#define Y_MIN	 		(0)
#define Y_MAX	 		(LCDH - 1)

#define TIMER_PERIOD	(10)
#define RIGHT			(1)
#define LEFT			(-1)
#define HOME			(0)
#define SCHOOL			(1)

#define CAR_STEP		(10)
#define CAR_SIZE_X		(20)
#define CAR_SIZE_Y		(20)
#define FROG_STEP		(50)
#define FROG_SIZE_X		(20)
#define FROG_SIZE_Y		(20)
#define FROG_LOCATION_X (150)
#define FROG_LOCATION_Y	(220)
#define BULLET_STEP     (30)
#define BULLET_SIZE_W   (5)
#define BULLET_SIZE_H   (5)


#define BACK_COLOR		(5)
#define CAR_COLOR		(0)
#define FROG_COLOR		(1)


#define GAME_OVER		(1)


static int car_destroy;


class Object
{
	public:

	int x;
	int y;
	unsigned short color;
	int size_w;
	int size_h;

	void Draw()
	{
		Lcd_Draw_Box(x, y, size_w, size_h, color);
	}

};

class Shooting: public Object
{
	public:
	
	bool state;

	void Move()
	{
		if(y<0) 
		{
			state=false;
		}
		if(state)
		{
			y -= BULLET_STEP;
		}
	}

};

class C_frog : public Object
{
	public:

	int dir;
	int score;
	unsigned short Collision_state;
	int POWER_STEP;
	int Count_Step;
	int Power_state;
	int num;
	

	void Move(int key)
	{
		if(Count_Step==3) 
		{
			Power_state=0;
			Count_Step=0;
			POWER_STEP=0;
		}
		switch (key)
		{
		case 0:
		if((y<FROG_STEP+POWER_STEP)&&(y>Y_MIN)) y = Y_MIN;
		else if(y > Y_MIN) y -= FROG_STEP+POWER_STEP;
		if(Power_state) Count_Step+=1;
		break;
		case 1:
		if((y+size_h<Y_MAX)&&(y+size_h>Y_MAX-(FROG_STEP+POWER_STEP))) y = FROG_LOCATION_Y;
		else if(y + size_h < Y_MAX) y += FROG_STEP+POWER_STEP;
		if(Power_state) Count_Step+=1;
		break;
		case 2:
		if((x>X_MIN)&&(x<FROG_STEP+POWER_STEP)) x = X_MIN;
		else if(x > X_MIN) x -= FROG_STEP+POWER_STEP;
		if(Power_state) Count_Step+=1;
		break;
		case 3:
		if((x+size_w<X_MAX)&&(x+size_w>X_MAX-(FROG_STEP+POWER_STEP))) x = X_MAX - FROG_SIZE_X;
		else if(x + size_w < X_MAX) x += FROG_STEP+POWER_STEP;
		if(Power_state) Count_Step+=1;
		break;
		case 4:
		if(num==3) break;
		Power_state=1;
		POWER_STEP = 10;
		num++;
		break;
		case 5:
		break;

		
		default:
		break;
		}
	}
	int Check_Collision(int Car_x, int Car_y)
	{
		if((Car_x >= x) && ((x + FROG_SIZE_X) >= Car_x)) Collision_state |= 1<<0;
	 	else if((Car_x <= x) && ((Car_x + FROG_SIZE_X) >= x)) Collision_state |= 1<<0;
	
		if((Car_y >= y) && ((y + FROG_SIZE_X) >= Car_y)) Collision_state |= 1<<1;
	 	else if((Car_y <= y) && ((Car_y + FROG_SIZE_X) >= y)) Collision_state |= 1<<1;

		if(Collision_state==3) 
		{
			Collision_state = 0;
			Uart_Printf("SCORE = %d\n", score);
			return GAME_OVER;
		}
		if((dir==SCHOOL)&&(y==Y_MIN)) 
		{
			dir=HOME;
			Uart_Printf("SCHOOL\n");
			
		}
		else if((dir==HOME)&&(y>=FROG_LOCATION_Y)) 
		{
			score++;
			Uart_Printf("HOME %d\n",score);
			dir=SCHOOL;
			
		}

		Collision_state = 0;
		return 0;
	}
};




class C_car : public Object
{
	public:
	
	int dir;
	bool state;
	
	void Move(int speed=0)
	{
		if(x>=LCDW-CAR_SIZE_X) dir=LEFT;
		else if(x<=X_MIN) dir=RIGHT;

		if((x<LCDW-CAR_SIZE_X)&&(dir==RIGHT)) x += CAR_STEP+speed;
		else if((x>X_MIN)&&(dir==LEFT)) x -= CAR_STEP+speed;
	}
	
};

static unsigned short color[] = {RED, YELLOW, GREEN, BLUE, WHITE, BLACK};
static unsigned int Stage;
static int Car_Speed[3];
static C_frog frog1;
static C_car Car[3];
static int Collision_State_Bullet;
static Shooting Bullet[5];
static int game_state;
static int game_over;

static int k;
static int num;
static long long int timetick;





extern volatile int TIM4_expired;
extern volatile int USART1_rx_ready;
extern volatile int USART1_rx_data;
extern volatile int Jog_key_in;
extern volatile int Jog_key;



static void Game_Init()
{
	Lcd_Clr_Screen();
	
    Car_Speed[0] = 0;
	Car_Speed[1] = 3;
	Car_Speed[2] = 5;

	frog1.x=FROG_LOCATION_X;
	frog1.y=FROG_LOCATION_Y;
	frog1.size_h=FROG_SIZE_Y;
	frog1.size_w=FROG_SIZE_X;
	frog1.color=color[3];
	frog1.dir=SCHOOL;
	frog1.score=0;
	frog1.Collision_state=0;
	frog1.Power_state=0;
	frog1.POWER_STEP=0;
	frog1.Count_Step=0;
	frog1.num=0;
	Car[0].x=0;
	Car[0].y=Y_MAX/2;
	Car[0].color=color[0];
	Car[0].size_h=CAR_SIZE_X;
	Car[0].size_w=CAR_SIZE_Y;
	Car[0].dir = RIGHT;
	Car[0].state = true;
	game_over=0;
	game_state=0;
	Stage = 0;
	k=0;
	num=0;


}
static void Car_Init(int i,int y=0,int x=0)
{
	Car[i].x=x;
	Car[i].y=y;
	Car[i].color=color[i];
	Car[i].size_h=CAR_SIZE_X;
	Car[i].size_w=CAR_SIZE_Y;
	Car[i].dir = RIGHT;
	Car[i].state=true;
}
static void Shooting_Init(int i)
{
	
	Bullet[i].size_w=BULLET_SIZE_W;
	Bullet[i].size_h=BULLET_SIZE_H;
	Bullet[i].color=color[1];
	Bullet[i].state=false;

}

static bool Check_Collision_Shoot(C_car &car,Shooting &bullet)
{
	if((car.x >= bullet.x) && ((bullet.x + BULLET_SIZE_W) >= car.x)) Collision_State_Bullet |= 1<<0;
	 	else if((car.x <= bullet.x) && ((car.x + 10) >= bullet.x)) Collision_State_Bullet |= 1<<0;
	
		if((car.y >= bullet.y) && ((bullet.y + BULLET_SIZE_W) >= car.y)) Collision_State_Bullet |= 1<<1;
	 	else if((car.y <= bullet.y) && ((car.y + 10) >= bullet.y)) Collision_State_Bullet |= 1<<1;

		if(Collision_State_Bullet==3) 
		{
			car.color=color[5];
			bullet.color=color[5];
			bullet.Draw();
			car.Draw();
			car.state=false;
			car.x=0;
			car.y=0;
			Collision_State_Bullet = 0;
			bullet.state=false;
			bullet.x=0;
			bullet.y=0;
			Uart_Printf("destoryed\n");
			num++;
			
			return true;
		}

		return false;

}

static void Key_In_Frog()
{
		Uart_Printf("KEY %d\n",Jog_key);
		
		frog1.color=color[5];
		frog1.Draw();
		frog1.Move(Jog_key);
		for(int i=0;i<game_state+1;i++)
		{
			game_over = frog1.Check_Collision(Car[i].x,Car[i].y);
			if(game_over) break;
		}
		frog1.color=color[3];
		frog1.Draw();
		Jog_key_in = 0;

}
static void Auto_Car()
{
	for(int i=0;i<game_state+1;i++)
		{
			if(Car[i].state)
			{
				Car[i].color=color[5];
				Car[i].Draw();
				Car[i].Move(Car_Speed[i]);
				game_over = frog1.Check_Collision(Car[i].x,Car[i].y);
				Car[i].color = color[i];
				Car[i].Draw();
			}
			if(game_over) break;
		}

}

static void Shooting_Run()
{
	for(int i=0;i<k;i++)
	{
		if(Bullet[i].state)
		{
		Bullet[i].color=color[5];
		Bullet[i].Draw();
		Bullet[i].Move();
		car_destroy=Check_Collision_Shoot(Car[i],Bullet[i]);
		if(!car_destroy||Bullet[i].state!=0)
		{
			Bullet[i].color=color[1];
			Bullet[i].Draw();
			car_destroy=0;
		}
		}
	}
	
	
}
static void Basic_run()
{

	if(Jog_key_in) 
	{
		Uart_Printf("KEY %d\n",Jog_key);
		frog1.color=color[5];
		frog1.Draw();
		frog1.Move(Jog_key);
		for(int i=0;i<game_state+1;i++)
		{
			game_over = frog1.Check_Collision(Car[i].x,Car[i].y);
			if(game_over) break;
		}
		frog1.color=color[3];
		frog1.Draw();
		Jog_key_in = 0;				
	}

	if(TIM4_expired) 
	{
		
		for(int i=0;i<game_state+1;i++)
		{
				Car[i].color=color[5];
				Car[i].Draw();
				Car[i].Move(Car_Speed[i]);
				game_over = frog1.Check_Collision(Car[i].x,Car[i].y);
				Car[i].color = color[i];
				Car[i].Draw();
			if(game_over) break;
		}
		TIM4_expired = 0;
	}

}


extern "C" void abort(void)
{
  while (1);
}

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);

	SCB->VTOR = 0x08003000;
	SCB->SHCSR = 7<<16;	
}

extern "C" void Main()
{
	Sys_Init();
	Uart_Printf("Game Example\n");

	Lcd_Init();
	Jog_Poll_Init();
	Jog_ISR_Enable(1);
	Uart1_RX_Interrupt_Enable(1);

	for(;;)
	{
		Game_Init();
		TIM4_Repeat_Interrupt_Enable(1, TIMER_PERIOD*10);
		
		Collision_State_Bullet=0;
		Lcd_Printf(100,100,GREEN,BLACK,1,1,"American Bullfrog");
		TIM2_Delay(3000);
		Lcd_Clr_Screen();
		Lcd_Printf(100,100,BLUE,BLACK,1,1,"Game Start!");
		TIM2_Delay(3000);
		Lcd_Init();

		frog1.Draw();
	    Car[0].Draw();
		Lcd_Printf(260,10,RED,WHITE,1,1,"SCORE:%d",frog1.score);

		

		for(;;)
		{
			
			
			

			switch(Stage)
			{
				case 0:
				game_state=0;
				if(frog1.score==1) 
				{
					Car_Init(1,Y_MAX/2-30);
					Lcd_Printf(260,10,RED,WHITE,1,1,"SCORE:%d",frog1.score);
					Stage = 1;
					
				}
				Basic_run();
				break;
				case 1:
				game_state=1;
				if(frog1.score==2)
				{
					Car_Init(2,Y_MAX/2-60);
					Lcd_Printf(260,10,RED,WHITE,1,1,"SCORE:%d",frog1.score);
					Stage = 2;
					
				}
				Basic_run();
				break;
				case 2:
				game_state=2;
				if(frog1.score==3)
				{
					Lcd_Printf(260,10,RED,WHITE,1,1,"SCORE:%d",frog1.score);
					Stage = 3;
					Car_Speed[0]++;
					Car_Speed[1]++;
					Car_Speed[2]++;
				}
				Basic_run();
				break;
				case 3:
				game_state=2;
				if(frog1.score==4)
				{
					Lcd_Printf(260,10,RED,WHITE,1,1,"SCORE:%d",frog1.score);
					Stage = 4;
					Car_Speed[0]++;
					Car_Speed[1]++;
					Car_Speed[2]++;
				}
				Basic_run();
				break;
				case 4:
				game_state=2;
				if(frog1.score==5)
				{
					Lcd_Printf(260,10,RED,WHITE,1,1,"SCORE:%d",frog1.score);
					Stage = 5;
					Car_Speed[0]+=2;
					Car_Speed[1]+=2;
					Car_Speed[2]+=2;
				}
				Basic_run();
				break;
				case 5:
				game_state=2;
				if(frog1.score==6)
				{
					Lcd_Printf(100,100,BLUE,WHITE,1,1,"BOSS STAGE");
					TIM2_Delay(3000);
					Stage=6;
				}
				Basic_run();
				break;
				case 6:
				Lcd_Clr_Screen();
				frog1.Draw();
				for(int i=0;i<5;i++)
				{
					Shooting_Init(i);
				}
				for(;;)
				{
					
					if(Jog_key_in)
					{
						if(Jog_key==5)
						{
							
							if(k<5) 
							{
								Bullet[k].x=frog1.x+FROG_SIZE_X/2-2.5;
	        					Bullet[k].y=frog1.y-5;
								Bullet[k].state=true;
							}
							k++;
						}
						Key_In_Frog();
					}

					if(TIM4_expired)
					{
						Auto_Car();
						Shooting_Run();
						timetick++;
						TIM4_expired=0;
					}
					if(game_over) break;
					if(num==3) 
					{
						game_over=1;
						break;
					}
					if(timetick==500) 
					{
						game_over=1;
						break;
					}
				}
				break;
			}

			
			

			if(game_over)
			{
				TIM4_Repeat_Interrupt_Enable(0, 0);
				Uart_Printf("Game Over, Please press any key to continue.\n");
				if(num==3)
				{
					Lcd_Printf(100,100,RED,BLACK,1,1,"You Win!");
					TIM2_Delay(3000);
				}
				else 
				{
					Lcd_Printf(100,100,RED,BLACK,1,1,"Game Over!");
				}
				TIM2_Delay(3000);
				Lcd_Printf(30,120,GREEN,BLACK,1,1,"Please press any key to continue.");
				Jog_Wait_Key_Pressed();
				Jog_Wait_Key_Released();
				Uart_Printf("Game Start\n");
				Stage = 0;

				break;
			}


		}
	}
}