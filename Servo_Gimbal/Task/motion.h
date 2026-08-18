#ifndef __MOTION
#define __MOTION

#include "cmsis_os.h"
#include "main.h"

#include "FreeRTOS.h"

/* 限制最大最小值 */
#define CONSTRAINT(x, min, max) (x < min ? min : (x > max ? max : x))

typedef enum
{
    MOTION_SLEEP = 0,
    MOTION_WALKING,
    MOTION_SAD,
    MOTION_HAPPY,
    MOTION_SCARE,
    MOTION_ANGRY,
    MOTION_DISDAIN,
}MotionState_e;


void StartMotionTask(void * argument);

#endif
