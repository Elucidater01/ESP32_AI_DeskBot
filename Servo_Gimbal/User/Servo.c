#include "servo.h"

#define LEFT_ARM TIM_CHANNEL_1
#define RIGHT_ARM TIM_CHANNEL_2
#define BOTTIOM TIM_CHANNEL_3

/**
  * @brief  设置舵机角度
  * @param  htim: 指向已配置为PWM的定时器句柄 (如 &htim3)
  * @param  Channel: 定时器通道 (如 TIM_CHANNEL_1)
  * @param  angle: 目标角度，范围 0 ~ 180
  * @retval None
  */
void Servo_SetAngle(uint32_t Channel, uint16_t angle)
{
  uint16_t pulse;
  
  // 1. 安全限制，确保角度在0-180度范围内
  if (angle < 0) angle = 0;
  else if (angle > 180) angle = 180;
  
  // 2. 核心计算公式：将角度线性映射到脉冲宽度
  //    角度0度   -> SERVO_MIN_PULSE
  //    角度180度 -> SERVO_MAX_PULSE
  pulse = SERVO_MIN_PULSE + (angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE)) / 180;
  
  // 3. 通过设置比较寄存器(CCR)来改变PWM脉宽，控制舵机
  __HAL_TIM_SET_COMPARE(&htim1, Channel, pulse);
}

void LeftArm_Angle(uint16_t angle)
{
	Servo_SetAngle(TIM_CHANNEL_1, angle);
}

void RightArm_Angle(uint16_t angle)
{
	Servo_SetAngle(TIM_CHANNEL_2, angle);
}

void Bottom_Angle(uint16_t angle)
{
	Servo_SetAngle(TIM_CHANNEL_3, angle);
}

void Up_Angle(uint16_t angle)
{
	Servo_SetAngle(TIM_CHANNEL_4, angle);
}