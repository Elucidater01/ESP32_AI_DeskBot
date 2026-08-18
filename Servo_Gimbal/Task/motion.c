#include "motion.h"
#include "Servo.h"
#include "drv_uart.h"

#define LEFT_MID 150
#define RIGHT_MID 30
#define BOTTIOM_MID 90

MotionState_e motion_state = MOTION_SLEEP;

uint16_t left_angle = 150;
uint16_t right_angle = 30;
uint16_t bottom_angle = 90;
uint16_t up_angle = 90;


int task_cnt = 0;
int sad_cnt = 0;
int scare_cnt = 0;
int angry_cnt = 0;
int disdain_cnt = 0;

int16_t left_bias = 0;
int16_t right_bias = 0;
int16_t bottom_bias = 0;
uint8_t direction = 0;

void Motion_Sleep()
{
	left_angle = LEFT_MID;
	right_angle = RIGHT_MID;
	bottom_angle = BOTTIOM_MID;
	
	LeftArm_Angle(left_angle);
	RightArm_Angle(right_angle);
	Bottom_Angle(bottom_angle);
}

void Motion_Walking()
{
	if(task_cnt % 10 == 0)
	{
		if(left_bias >= 50)
		{
			direction = 1;
		}
		else if(left_bias <= -50)
		{
			direction = 0;
		}
		
		if(direction == 0)
		{
			left_bias++;
		}
		else
		{
			left_bias--;
		}
	}
	LeftArm_Angle(left_angle+left_bias);
	RightArm_Angle(right_angle+left_bias);
	Bottom_Angle(bottom_angle+bottom_bias);
}

void Motion_Sad()
{
	sad_cnt++;

	if(sad_cnt < 300)
	{
		left_angle = 0;
		bottom_angle = 80;
	}
	else if(sad_cnt < 500)
	{
		left_angle = 20;
		bottom_angle = 90;
	}
	else if(sad_cnt < 700)
	{
		left_angle = 0;
	}
	else if(sad_cnt < 900)
	{
		left_angle = 20;
	}
	else if(sad_cnt < 1100)
	{
		left_angle = 0;
	}
	else if(sad_cnt < 1300)
	{
		left_angle = 20;
	}
	else if(sad_cnt < 1500)
	{
		left_angle = 0;
	}
	else if(sad_cnt < 1700)
	{
		left_angle = 20;
	}
	else if(sad_cnt < 1900)
	{
		left_angle = 0;
	}
	else if(sad_cnt < 2100)
	{
		left_angle = 20;
	}
	else
	{
		motion_state = MOTION_SLEEP;
	}
}

void Motion_Scare()
{
	scare_cnt++;

	// if(task_cnt % 10 == 0)
	// {
		
		if(scare_cnt < 1000)
		{
			left_angle = 0;
			right_angle = 180;
		}
		else if(scare_cnt >= 1000  && scare_cnt < 1300)
		{
			bottom_angle = 80;
		}
		else if(scare_cnt >= 1300 && scare_cnt < 1600)
		{
			bottom_angle = 110;
		}
		else if(scare_cnt >= 1600 && scare_cnt < 1900)
		{
			bottom_angle = 80;
		}
		else if(scare_cnt >= 1900 && scare_cnt < 2200)
		{
			bottom_angle = 110;
		}
		else if(scare_cnt >= 2200 && scare_cnt < 2500)
		{
			bottom_angle = 80;
		}
		else if(scare_cnt >= 2500 && scare_cnt < 2800)
		{
			bottom_angle = 110;
		}
		else if(scare_cnt >= 2800 && scare_cnt < 3100)
		{
			bottom_angle = 80;
		}
		else if(scare_cnt >= 3400 && scare_cnt < 3700)
		{
			bottom_angle = 110;
		}
		else
		{
			bottom_angle = BOTTIOM_MID;
			motion_state = MOTION_SLEEP;
		}
	// }
		
	LeftArm_Angle(left_angle);
	RightArm_Angle(right_angle);
	Bottom_Angle(bottom_angle);
}

void Motion_Angry()
{
	angry_cnt++;

	if(angry_cnt < 300)
	{
		left_angle = 0;
		right_angle = 180;
	}
	else if(angry_cnt < 600)
	{
		left_angle = LEFT_MID;
		right_angle = RIGHT_MID;
	}
	else if(angry_cnt < 900)//2
	{
		left_angle = 0;
		right_angle = 180;
		bottom_angle = 100;
	}
	else if(angry_cnt < 1200)
	{
		left_angle = LEFT_MID;
		right_angle = RIGHT_MID;
		bottom_angle = 80;
	}
	else if(angry_cnt < 1500)//3
	{
		left_angle = 0;
		right_angle = 180;
		bottom_angle = 100;
	}
	else if(angry_cnt < 1800)
	{
		left_angle = LEFT_MID;
		right_angle = RIGHT_MID;
		bottom_angle = 80;
	}
	else if(angry_cnt < 2100)//4
	{
		left_angle = 0;
		right_angle = 180;
		bottom_angle = 100;
	}
	else if(angry_cnt < 2400)
	{
		left_angle = LEFT_MID;
		right_angle = RIGHT_MID;
		bottom_angle = 80;
	}
	else if(angry_cnt < 2700)//5
	{
		left_angle = 0;
		right_angle = 180;
		bottom_angle = 100;
	}
	else if(angry_cnt < 3000)
	{
		left_angle = LEFT_MID;
		right_angle = RIGHT_MID;
		bottom_angle = 80;
	}
	else
	{
		motion_state = MOTION_SLEEP;
	}

	LeftArm_Angle(left_angle);
	RightArm_Angle(right_angle);
	Bottom_Angle(bottom_angle);
}

void Motion_Disdain()
{
	disdain_cnt++;

	if(disdain_cnt < 300)
	{
		bottom_angle = 100;
	}
	else if(disdain_cnt >= 900 && disdain_cnt < 1200)
	{
		bottom_angle = BOTTIOM_MID;
		right_angle = 130;
	}
	else if(disdain_cnt >= 1200 && disdain_cnt < 1500)
	{
		right_angle = 100;
	}
	else if(disdain_cnt >= 1500 && disdain_cnt < 1800)
	{
		right_angle = 130;
	}
	else if(disdain_cnt >= 1800 && disdain_cnt < 2100)
	{
		right_angle = 100;
	}
	else if(disdain_cnt >= 2100 && disdain_cnt < 2400)
	{
		right_angle = 130;
	}
	else if(disdain_cnt >= 2400 && disdain_cnt < 2700)
	{
		right_angle = 90;
	}
	else if(disdain_cnt >= 2700 && disdain_cnt < 3000)
	{
		right_angle = RIGHT_MID;
		bottom_angle = 80;
	}
	else
	{
		bottom_angle = BOTTIOM_MID;
		motion_state = MOTION_SLEEP;
	}
	LeftArm_Angle(left_angle);
	RightArm_Angle(right_angle);
	Bottom_Angle(bottom_angle);
}

void StartMotionTask(void * argument)
{
	/* Infinite loop */
	
  for(;;)
  {
		// if(task_cnt < 1000)
		// {
		// 	motion_state = MOTION_SLEEP;
		// }
		// else if(task_cnt < 2000)
		// {
		// 	motion_state = MOTION_ANGRY;
		// }

		if (servo_state[0] == 0xB0)
		{
			motion_state = MOTION_SLEEP;
		}
		else if (servo_state[0] == 0xB1)
		{
			motion_state = MOTION_SAD;
		}
		else if (servo_state[0] == 0xB2)
		{
			motion_state = MOTION_HAPPY;
		}
		else if (servo_state[0] == 0xB3)
		{
			motion_state = MOTION_ANGRY;
		}
		else if (servo_state[0] == 0xB4)
		{
			motion_state = MOTION_SCARE;
		}
		else if (servo_state[0] == 0xB5)
		{
			motion_state = MOTION_DISDAIN;
		}

		switch(motion_state)
		{
			case MOTION_SLEEP:
				scare_cnt = 0;
				angry_cnt = 0;
				disdain_cnt = 0;
				Motion_Sleep();
				break;
			case MOTION_WALKING:
				Motion_Walking();
				break;	
			case MOTION_SAD:
				Motion_Sad();
				break;
			case MOTION_HAPPY:
				Motion_Scare();
				break;
			case MOTION_SCARE:
				Motion_Scare();
				break;
			case MOTION_ANGRY:
				Motion_Angry();
				break;
			case MOTION_DISDAIN:
				Motion_Disdain();
				break;
			default:
				break;
		}

		// Up_Angle(up_angle);
		
		if(++task_cnt > 10000)
		{
			task_cnt = 0;
		}
		
    osDelay(1);
  }
}
