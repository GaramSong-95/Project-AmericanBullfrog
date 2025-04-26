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
#define UP				(-1)
#define DOWN			(1)
#define HOME			(0)
#define SCHOOL			(1)

#define CAR_STEP		(10)
#define CAR_STEP_1		(10)
#define CAR_STEP_2		(20)
#define CAR_STEP_3		(30)
#define CAR_SQ_SIZE		(20)
#define CAR_SIZE_X		(20)
#define CAR_SIZE_Y		(20)
#define FROG_STEP		(10)
#define FROG_SQ_SIZE	(20)	
#define FROG_SIZE_X		(20)
#define FROG_SIZE_Y		(20)
#define WALL_1_SIZE_X	(20)
#define WALL_1_SIZE_Y	(20)
#define WALL_2_SIZE_X	(100)
#define WALL_2_SIZE_Y 	WALL_1_SIZE_Y
#define WALL_MOVE_STEP	(5)
#define WALL_STOP_STEP	(0)

#define BACK_COLOR		(5)
#define CAR_COLOR		(0)
#define CAR_COLOR_1		(0)
#define CAR_COLOR_2		(2)
#define CAR_COLOR_3		(3)
#define FROG_COLOR		(1)
#define WALL_COLOR		(4)
#define WALL2_COLOR		(5)

#define MAX_CAR_COUNT	(3)
#define MAX_WALL_COUNT	(5)

#define MURDER		    (0)
#define SUVIVOR			(1)

#define MAX_SCORE			(99)
#define MAX_LEVEL			(10)
#define LEVEL_CHANGE_SCORE	(10)
#define GAME_OVER			(1)

typedef struct
{
	int x,y;
	int w,h;
	int ci;
	int dir_lr;
	int dir_ud;
}QUERY_DRAW;
typedef QUERY_DRAW Entity_Inform;

class entity
{
protected :
	int pos_x, pos_y, dir_LR, dir_UD, speed;
	const int sq_size, u_color;
	
	bool murder, suvivor;
	bool attackable;
public :
	entity(Entity_Inform _inform, int _speed, bool position /** 0 : murder, 1 : suvivor */, bool _ack_able) :\
	 sq_size(_inform.w), u_color(_inform.ci) 
	{
		pos_x = _inform.x; 
		pos_y = _inform.y;
		dir_LR = _inform.dir_lr;
		dir_UD = _inform.dir_ud;
		speed = _speed;

		attackable = _ack_able;

		if(position == MURDER) 				{ murder = true;  suvivor = false; }
		else /*if(position == SUVIVOR)*/	{ murder = false; suvivor = true;  }
	}

	virtual void move(int _x = 0, int _y = 0) = 0;

	inline void draw_clear();
	inline void draw();
	
	inline int get_x() const;
	inline int get_y() const;

	inline int get_speed() const;
	inline void set_speed(int _speed);
};

class wall : public entity
{
private :
	bool is_moving_wall;
public :
	wall(Entity_Inform _inform, int _speed, bool position /** 0 : murder, 1 : suvivor */, bool _ack_able, bool _is_move) \
	 : entity(_inform, _speed, position, _ack_able), is_moving_wall(_is_move) { /* -- */ }

	void auto_move(int _x = 0, int _y = 0);
	virtual void move(int _x = 0, int _y = 0) override;
	
	inline void set_attackable(bool ack_able);
	inline bool is_attackable();

	// inline bool set_movable();
	inline bool is_movable();
	inline bool get_direction_lr() { return dir_LR == LEFT ? true : false; }

	inline void draw_Line();
};

class car : public entity
{
public :
	car(Entity_Inform _inform, int _speed, bool position /** 0 : murder, 1 : suvivor */, bool _ack_able) \
	 : entity(_inform, _speed, position, _ack_able) { /* -- */ }

	void auto_move(int _x = 0, int _y = 0);
	virtual void move(int _x = 0, int _y = 0) override;
	
	inline void set_attackable(bool ack_able);
	inline bool is_attackable();
};

class player : public entity
{
private :
	int mission = SCHOOL;
	int score 	= 0;
public :

	player(Entity_Inform _inform, int _speed, bool position /** 0 : murder, 1 : suvivor */, bool _ack_able) \
	 : entity(_inform, _speed, position, _ack_able) { /* -- */ }

	virtual void move(int _x = 0, int _y = 0) override;
	void manual_move(int _k);

	inline int get_cur_mission() const;
	inline void set_cur_mission(int _mission);

	inline int get_cur_score() const;
	inline void set_cur_score(int _score);
};


#ifndef __cplusplus

void Game_Init(player *_p, car (*_car)[MAX_CAR_COUNT]);
static int Check_Collision(player *_p, car (*_car)[MAX_CAR_COUNT]);
static void score_up_setter(player *_p, car (*_car)[MAX_CAR_COUNT]);
static void score_down_setter(player *_p, car (*_car)[MAX_CAR_COUNT]);
static void level_setter(player *_p, car (*_car)[MAX_CAR_COUNT], int _score);
void level_1(player *_p, car (*_car)[MAX_CAR_COUNT]);
void level_2(player *_p, car (*_car)[MAX_CAR_COUNT]);
void level_3(player *_p, car (*_car)[MAX_CAR_COUNT]);
void level_4(player *_p, car (*_car)[MAX_CAR_COUNT]);
void level_5(player *_p, car (*_car)[MAX_CAR_COUNT]);
void level_6(player *_p, car (*_car)[MAX_CAR_COUNT]);
void level_7(player *_p, car (*_car)[MAX_CAR_COUNT]);
void level_8(player *_p, car (*_car)[MAX_CAR_COUNT]);
void level_9(player *_p, car (*_car)[MAX_CAR_COUNT]);
void level_10(player *_p, car (*_car)[MAX_CAR_COUNT]);

#else

void Game_Init(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);

static int Check_Car_Collision(player &_p, car (&_cars)[MAX_CAR_COUNT]);
static void Check_Wall_Collision(player &_p, wall (&_walls)[MAX_WALL_COUNT]);
// static int Check_Bullet_Collision(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);
static void score_up_setter(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);
static void score_down_setter(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);
static void level_setter(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT], int _score);
void level_1(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);
void level_2(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);
void level_3(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);
void level_4(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);
void level_5(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);
void level_6(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);
void level_7(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);
void level_8(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);
void level_9(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);
void level_10(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]);

#endif

static unsigned short color[] = {RED, YELLOW, GREEN, BLUE, WHITE, BLACK};

// entity table
Entity_Inform _car_1  = { 0,   90,  CAR_SIZE_X,    CAR_SIZE_Y,    CAR_COLOR_1, RIGHT, UP };
Entity_Inform _car_2  = { 0,   150, CAR_SIZE_X,    CAR_SIZE_Y,    CAR_COLOR_2, RIGHT, UP };
Entity_Inform _car_3  = { 0,   30,  CAR_SIZE_X,    CAR_SIZE_Y,    CAR_COLOR_3, RIGHT, UP };
Entity_Inform _frog_1 = { 150, 220, FROG_SIZE_X,   FROG_SIZE_Y,   FROG_COLOR,  LEFT,  UP };
Entity_Inform _wall_1 = { 0,   60 , WALL_2_SIZE_X, WALL_2_SIZE_Y, WALL2_COLOR, RIGHT, UP };
Entity_Inform _wall_2 = { 0,   180, WALL_2_SIZE_X, WALL_2_SIZE_Y, WALL2_COLOR, RIGHT, UP };
Entity_Inform _wall_3 = { 160, 60,  WALL_1_SIZE_X, WALL_1_SIZE_Y, WALL_COLOR,  RIGHT, UP };
Entity_Inform _wall_4 = { 40,  120, WALL_1_SIZE_X, WALL_1_SIZE_Y, WALL_COLOR,  RIGHT, UP };
Entity_Inform _wall_5 = { 180, 180, WALL_1_SIZE_X, WALL_1_SIZE_Y, WALL_COLOR,  RIGHT, UP };

extern volatile int TIM4_expired;
extern volatile int USART1_rx_ready;
extern volatile int USART1_rx_data;
extern volatile int Jog_key_in;
extern volatile int Jog_key;

static int current_level = 0;

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
		// init	

		// -----------------------------------------------------------------
		player frog(_frog_1, FROG_STEP, SUVIVOR, false);
		car cars[MAX_CAR_COUNT]    = {  car(_car_1, CAR_STEP_1, MURDER, true),  \
									    car(_car_2, CAR_STEP_2, MURDER, false), \
									    car(_car_3, CAR_STEP_3, MURDER, false) };
		wall walls[MAX_WALL_COUNT] = {  wall(_wall_1, WALL_MOVE_STEP, MURDER, false, true), \
										wall(_wall_2, WALL_MOVE_STEP, MURDER, false, true), \
										wall(_wall_3, WALL_STOP_STEP, MURDER, false, false), \
										wall(_wall_4, WALL_STOP_STEP, MURDER, false, false), \
										wall(_wall_5, WALL_STOP_STEP, MURDER, false, false), };
		Game_Init(frog, cars, walls);

		// -----------------------------------------------------------------

		TIM4_Repeat_Interrupt_Enable(1, TIMER_PERIOD * 10);

		for(;;)
		{
			int game_over = 0;

			if(Jog_key_in) 
			{
				if(Jog_key == 4) { score_up_setter(frog, cars, walls); }
				if(Jog_key == 5) { score_down_setter(frog, cars, walls); }

				frog.manual_move(Jog_key);
				game_over = Check_Car_Collision(frog, cars);
				Jog_key_in = 0;				
			}

			if(TIM4_expired) 
			{
				for(int i = 0; i < MAX_CAR_COUNT; i++)
				{
					if(!cars[i].is_attackable()) { continue; }
					cars[i].auto_move(cars[i].get_speed(), 0);
				}
				game_over = Check_Car_Collision(frog, cars);
				for(int i = 0; i < MAX_WALL_COUNT; i++)
				{
					if(!walls[i].is_attackable()) { continue; }
					if(!walls[i].is_movable())    { continue; }
					frog.draw();
					walls[i].auto_move(walls[i].get_speed(), 0);
				}
				if(walls[0].is_attackable()) { Check_Wall_Collision(frog, walls); }
				TIM4_expired = 0;
			}

			if(game_over)
			{
				TIM4_Repeat_Interrupt_Enable(0, 0);
				Uart_Printf("Game Over, Please press any key to continue.\n");
				Jog_Wait_Key_Pressed();
				Jog_Wait_Key_Released();
				Uart_Printf("Game Start\n");
				break;
			}
			else
			{
				level_setter(frog, cars, walls, frog.get_cur_score());
			}
		}
	}
}

// Move 

// ---------------------------------------
void car::move(int _x, int _y)
{
	draw_clear();
	pos_x += _x; pos_y += _y;
	draw();
}

void player::move(int _x, int _y)
{
	draw_clear();
	pos_x += _x; pos_y += _y;
	draw();
}

void wall::move(int _x, int _y)
{
	pos_x += _x; pos_y += _y;

	bool dir = (0 > _x);
	if(dir) // dir_LR == LEFT
	{ 
		Lcd_Draw_Box(pos_x, pos_y, -(_x), WALL_2_SIZE_Y, color[u_color]);
		if(pos_x + WALL_2_SIZE_X + WALL_1_SIZE_X < X_MAX) { Lcd_Draw_Box(pos_x + WALL_2_SIZE_X, pos_y, WALL_1_SIZE_X , WALL_2_SIZE_Y, color[4]); }
	}
	else /* if(dir_LR == RIGHT) */ 
	{ 
		Lcd_Draw_Box(pos_x - _x, pos_y, _x, WALL_2_SIZE_Y, color[u_color]);
		if(pos_x - WALL_1_SIZE_X > WALL_2_SIZE_X) { Lcd_Draw_Box(pos_x - WALL_2_SIZE_X , pos_y, WALL_1_SIZE_X, WALL_2_SIZE_Y, color[4]); }
	}
}

void wall::auto_move(int _x, int _y)
{	
	if(dir_LR == RIGHT)  	 
	{ 
		if(pos_x + _x >= X_MAX) 
		{
			_x = X_MAX - pos_x;
			dir_LR = LEFT;
		}
		_x *= RIGHT /* 1 */;
	}
	else if(dir_LR == LEFT) 
	{ 
		if(pos_x <= _x) 
		{
			_x = pos_x;
			dir_LR = RIGHT;
		} 
		_x *= LEFT /* -1 */;
	}

	if(dir_UD == UP)
	{
		if(pos_y + _y >= Y_MAX) 
		{
			_y = Y_MAX - pos_y;
			dir_UD = DOWN;
		} 
		_y *= UP /* 1 */;
	}
	else if(dir_UD == DOWN)
	{
		if(pos_y <= _y) 
		{
			_y = pos_y;
			dir_UD = UP;
		} 
		_y *= DOWN /* -1 */;
	}
	move(_x, _y);
}

void car::auto_move(int _x, int _y)
{	
	if(dir_LR == RIGHT)  	 
	{ 
		if(pos_x + _x >= X_MAX) 
		{
			_x = X_MAX - pos_x;
			dir_LR = LEFT;
		}
		_x *= RIGHT /* 1 */;
	}
	else if(dir_LR == LEFT) 
	{ 
		if(pos_x <= _x) 
		{
			_x = pos_x;
			dir_LR = RIGHT;
		} 
		_x *= LEFT /* -1 */;
	}

	if(dir_UD == UP)
	{
		if(pos_y + _y >= Y_MAX) 
		{
			_y = Y_MAX - pos_y;
			dir_UD = DOWN;
		} 
		_y *= UP /* 1 */;
	}
	else if(dir_UD == DOWN)
	{
		if(pos_y <= _y) 
		{
			_y = pos_y;
			dir_UD = UP;
		} 
		_y *= DOWN /* -1 */;
	}
	move(_x, _y);
}

void player::manual_move(int _k)
{
	int _step;
	// UP(0), DOWN(1), LEFT(2), RIGHT(3)
	switch(_k)
	{
		case 0 :
			if(pos_y - FROG_STEP <= Y_MIN) { _step = pos_y; }
			else 				   		   { _step = FROG_STEP; }
			move(0, -(_step));
			break;
		case 1 :
			if(pos_y + FROG_STEP >= (Y_MAX - ((FROG_SIZE_Y - 1)))) { _step = (Y_MAX - ((FROG_SIZE_Y - 1))) - pos_y; }
			else { _step = FROG_STEP; }
			move(0, _step);
			break;
		case 2 :
			if(pos_x - FROG_STEP <= X_MIN) { _step = pos_x; }
			else 				  		   { _step = FROG_STEP; }
			move(-(_step), 0);
			break;
		case 3 :
			if(pos_x + FROG_STEP >= (X_MAX - (FROG_SIZE_X - 1))) { _step = (X_MAX - (FROG_SIZE_X - 1)) - pos_x; }
			else { _step = FROG_STEP; }
			move(_step, 0);
			break;
	}
}
// ---------------------------------------

// Function

void Game_Init(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT])
{
	Lcd_Clr_Screen();

	_p.draw();
	_cars[0].draw();
}

static int Check_Car_Collision(player &_p, car (&_cars)[MAX_CAR_COUNT])
{
	for(int i = 0 ; i < MAX_CAR_COUNT; i++)
	{
		if(!_cars[i].is_attackable()) { continue; }

		int col = 0;
		int _temp_score = _p.get_cur_score();

		if((_cars[i].get_x() >= _p.get_x()) && ((_p.get_x() + FROG_STEP) >= _cars[i].get_x())) col |= 1<<0;
		else if((_cars[i].get_x() < _p.get_x()) && ((_cars[i].get_x() + CAR_STEP) >= _p.get_x())) col |= 1<<0;
		
		if((_cars[i].get_y() >= _p.get_y()) && ((_p.get_y() + FROG_STEP) >= _cars[i].get_y())) col |= 1<<1;
		else if((_cars[i].get_y() < _p.get_y()) && ((_cars[i].get_y() + CAR_STEP) >= _p.get_y())) col |= 1<<1;

		if(col == 3)
		{
			Uart_Printf("SCORE = %d\n", _temp_score);	
			return GAME_OVER;
		}

		if((_p.get_cur_mission() == SCHOOL) && (_p.get_y() == Y_MIN)) 
		{
			Uart_Printf("SCHOOL\n");		
			_p.set_cur_mission(HOME);
		}

		if((_p.get_cur_mission() == HOME) && (_p.get_y() == LCDH - FROG_SIZE_Y))
		{
			_temp_score++; // + 1 point
			_p.set_cur_score(_temp_score);
			Uart_Printf("HOME, %d\n", _temp_score);
			_p.set_cur_mission(SCHOOL);
		}
	}
	return 0;
}

static void Check_Wall_Collision(player &_p, wall (&_walls)[MAX_WALL_COUNT])
{
	int _temp;
	int _collisioned = 0;
	for(int i = 0; i < MAX_WALL_COUNT; i++)
	{
		if(!_walls[i].is_attackable()) { continue; }

		// check x
		_temp = _walls[i].get_x();
		if(!((_temp + WALL_2_SIZE_X  <= _p.get_x() + FROG_STEP) || (_temp >= _p.get_x() + FROG_STEP))) { _collisioned |= 1<<0; } 
		if(!((_temp - WALL_2_SIZE_X  >= _p.get_x() + FROG_STEP) || (_temp <= _p.get_x() + FROG_STEP))) { _collisioned |= 1<<1; } 

		// check y
		if(_collisioned)
		{
			_temp = _walls[i].get_y();
			if((_temp >= _p.get_y()) && ((_p.get_y() + FROG_STEP) >= _temp)) 					 { _collisioned |= 1<<2; }
			else if((_temp < _p.get_y()) && ((_temp + WALL_2_SIZE_Y >= _p.get_y() + FROG_STEP))) { _collisioned |= 1<<3; }
		}

		if(_collisioned > (1 << 2)) 
		{
			if(_collisioned & 0x01) 
			{ _p.move(-WALL_MOVE_STEP, 0); }
			if(_collisioned & 0x02) 
			{ _p.move( WALL_MOVE_STEP, 0); }
		}
		_collisioned = 0;
	}
}

static void score_up_setter(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT])
{
	int _temp_score = _p.get_cur_score();
	if(_temp_score < MAX_SCORE) 
	{
		_p.set_cur_mission(HOME);
		_p.move(0, (LCDH - FROG_SIZE_Y) - (_p.get_y()));

		Check_Car_Collision(_p, _cars);
	}	
}

static void score_down_setter(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT])
{
	volatile int _temp_score = _p.get_cur_score();
	if(_temp_score > 1) 
	{ 
		_p.set_cur_score(_temp_score - 2); 
		_p.set_cur_mission(HOME);
		_p.move(0, (LCDH - FROG_SIZE_Y) - (_p.get_y()));

		Check_Car_Collision(_p, _cars);
	}	
	else 
	{ 
		_p.set_cur_score(0); 
	}
}

static void level_setter(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT], int _score)
{
	int _temp = (_score / LEVEL_CHANGE_SCORE) % (MAX_LEVEL);
	if(current_level == _temp) return;

	static void (*level_func[MAX_LEVEL])(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT]) \
		= { level_1, level_2, level_3, level_4, level_5, level_6, level_7, level_8, level_9, level_10 };

	
	if(current_level != _temp) 
	{ 
		current_level = _temp;
		Uart_Printf("level %d : \n", current_level); 
		level_func[current_level](_p, _cars, _walls);	
	}
	else { current_level = _temp; }
}

void level_1(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT])
{
	// visiable
	_cars[1].draw_clear();
	_cars[1].set_attackable(false);

	// speed
	_cars[0].set_speed(10);
}

void level_2(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT])
{
	_cars[1].set_attackable(true);
	_cars[2].draw_clear();
	_cars[2].set_attackable(false);
}

void level_3(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT])
{
	_cars[0].set_speed(10);
	_cars[2].set_attackable(true);
	_walls[0].set_attackable(false);
}

void level_4(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT])
{
	_cars[0].set_speed(40);
	_cars[1].set_speed(20);

	_walls[0].set_attackable(true);
}

void level_5(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT])
{
	_cars[1].set_speed(40);
	_walls[1].set_attackable(false);
}

void level_6(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT])
{
	_walls[1].set_attackable(true);
}

void level_7(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT])
{

}

void level_8(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT])
{

}

void level_9(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT])
{

}

void level_10(player &_p, car (&_cars)[MAX_CAR_COUNT], wall (&_walls)[MAX_WALL_COUNT])
{

}


#endif

inline void entity::draw()
{
	Lcd_Draw_Box(pos_x, pos_y, sq_size, sq_size, color[u_color]);
}

inline void entity::draw_clear()
{
	Lcd_Draw_Box(pos_x, pos_y, sq_size, sq_size, color[5]);
}

inline int entity::get_speed() const
{
	return speed;
}

inline void entity::set_speed(int _speed)
{
	speed = _speed;
}

inline int entity::get_x() const
{
	return pos_x;
}

inline int entity::get_y() const
{
	return pos_y;
}
// ---------------------------------------

// Entity Setting Option

// ---------------------------------------
inline bool wall::is_attackable()
{
	return attackable;
}

inline void wall::set_attackable(bool ack_able)
{
	attackable = ack_able;
}


inline bool wall::is_movable()
{
	return is_moving_wall;
}

inline void wall::draw_Line()
{
	Lcd_Draw_Box(0, pos_y, X_MAX, WALL_2_SIZE_Y, color[4]);
}

inline bool car::is_attackable()
{
	return attackable;
}

inline void car::set_attackable(bool ack_able)
{
	attackable = ack_able;
}

inline int player::get_cur_mission() const
{
	return mission;
}

inline void player::set_cur_mission(int _mission)
{
	mission = _mission;
}

inline int player::get_cur_score() const
{
	return score;
}

inline void player::set_cur_score(int _score)
{
	score = _score;
}
// ---------------------------------------