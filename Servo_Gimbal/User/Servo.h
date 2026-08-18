#ifndef __SERVO_H__
#define __SERVO_H__
#include "TIM.h"

// 舵机参数宏定义，方便修改和阅读
#define SERVO_MIN_PULSE  500   // 0.5ms 脉冲宽度对应的计时器计数值 (对应0度)
#define SERVO_MAX_PULSE  2500  // 2.5ms 脉冲宽度对应的计时器计数值 (对应180度)
#define SERVO_PERIOD     20000 // 20ms 周期对应的计时器计数值 (ARR的值)

void Servo_SetAngle(uint32_t Channel, uint16_t angle);
void LeftArm_Angle(uint16_t angle);
void RightArm_Angle(uint16_t angle);
void Bottom_Angle(uint16_t angle);
void Up_Angle(uint16_t angle);

#endif /*__ SERVO_H__ */
