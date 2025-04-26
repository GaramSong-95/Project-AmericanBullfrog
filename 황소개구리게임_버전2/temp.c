#if 0

static int Collision_State_Bullet;  // 총알과 차의 충돌여부를 확인하는 변수
static Shooting Bullet[5];  //총알 배열 선언
static int game_state;  // 차의 갯수를 제한하기 위한 변수
static int game_over;  // 게임이 끝나는 것을 알려주는 변수
static int car_destroy;  //차가 파괴되었을 때, 총알을 그리지 않기 위한 변수
static int k;      //총알 객체를 세기위한 변수
static int num;  //충돌 횟수를 세기 위한 변수
static long long int timetick;  //시간제한을 두기위한 변수

// 총알 객체 생성을 위한 class 선언
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
		if(y<0) state=false;  // 화면을 벗어나면 활성화 OFF
		if(state)  // 총알 활성화 상태에서만 움직인다.
		{
			y -= BULLET_STEP;
		}
	}

};

// 총알 객체 배열로 5개 생성
static Shooting Bullet[5];
// 총알 객체 초기화 함수
static void Shooting_Init(int i)
{
	
	Bullet[i].size_w=BULLET_SIZE_W;
	Bullet[i].size_h=BULLET_SIZE_H;
	Bullet[i].color=color[1];
	Bullet[i].state=false;

}

//총알과 차량의 충돌체크 함수
static bool Check_Collision_Shoot(C_car &car,Shooting &bullet)
{
	if((car.x >= bullet.x) && ((bullet.x + BULLET_SIZE_W /* 5 */) >= car.x)) Collision_State_Bullet |= 1<<0;  //충돌 사이즈는 여러번의 시험 결과 최적의 값
	 	else if((car.x <= bullet.x) && ((car.x + 10) >= bullet.x)) Collision_State_Bullet |= 1<<0;
	
		if((car.y >= bullet.y) && ((bullet.y + BULLET_SIZE_W) >= car.y)) Collision_State_Bullet |= 1<<1;
	 	else if((car.y <= bullet.y) && ((car.y + 10) >= bullet.y)) Collision_State_Bullet |= 1<<1;

		if(Collision_State_Bullet==3)    //우선적으로 충돌시 검은색으로 덮고 차량과 총알 모두 비활성화
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
			//Uart_Printf("destoryed\n");
			num++;
			
			return true;
		}

		return false;

}

// key가 입력되었을 때 작동되는 메커니즘을 함수화 기존과 다를 것은 없음.
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

// TIM4가 카운트 될 때 작동되는 메커니즘을 함수화 차량의 활성화에 따라 작동여부를 결정.
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

// TIM4 카운트 시 작동되는 메커니즘을 함수화 총알이 날라가는 것과 충돌을 체크  차량과 개구리의 충돌을 구분하기 위함.
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
		if(!car_destroy)
		{
			Bullet[i].color=color[1];
			Bullet[i].Draw();
			car_destroy=0;
		}
		}
	}
	
	
}


// 스테이지 5과 스테이지 6에 대한 매커니즘
case 5:
				game_state=2;
				if(frog1.score==6)
				{
					Lcd_Printf(100,100,BLUE,WHITE,1,1,"BOSS STAGE");   //스테이지 6으로 넘어갈때 문구가 생성되고 딜레이 3초
					TIM2_Delay(3000);
					Stage=6;
				}
				Basic_run();      // 기본적인 작동매커니즘.
				break;
				case 6:
				Lcd_Clr_Screen();    //화면 클리어
				frog1.Draw();
				for(int i=0;i<5;i++)   // 총알 객체 모두 초기화
				{
					Shooting_Init(i);
				}
				for(;;)     //스테이지 6동안 루프에 갇혀있다.
				{
					
					if(Jog_key_in)
					{
						if(Jog_key==5)    // 키 5번이 눌리면 총알의 위치가 설정되고 총알이 활성화됨 k는 총알 갯수 체크를 위한 변수 총알을 5개가 최대이므로 제한을 둠.
						{
							Bullet[k].x=frog1.x+FROG_SIZE_X/2-2.5;
	        				        Bullet[k].y=frog1.y-5;
							if(k<5) Bullet[k].state=true;
							k++;
						}
						Key_In_Frog();
					}

					if(TIM4_expired)
					{
						Auto_Car();      //메커니즘 함수화한 것.
						Shooting_Run();
						timetick++;    // 시간 제한을 두기위한 변수
						TIM4_expired=0;
					}
					if(game_over) break;
					if(num==3)           // 차량과 총알의 충돌시 num가 증가 3개 다 맞추면 num는 3으로 게임이 끝난다.
					{
						game_over=1;
						break;
					}
					if(timetick==100)   // 시간제한을 지나면 게임이 끝난다.
					{
						game_over=1;
						break;
					}
				}
				break;


#endif