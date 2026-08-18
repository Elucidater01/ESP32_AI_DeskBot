#ifndef __DRV_UART_H__
#define __DRV_UART_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "usart.h"

void UART_Init(void);
void HAL_UART_RxCallback(UART_HandleTypeDef *huart);

extern uint8_t servo_state[2];

#endif
