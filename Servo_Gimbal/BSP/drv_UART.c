#include "drv_uart.h"
#include "string.h"

/* 串口句柄（CubeMX生成，也可手动定义） */
//UART_HandleTypeDef huart1;

/* 环形缓冲区配置（解决中断接收丢包问题） */
#define UART_RX_BUF_SIZE 16  // 缓冲区大小，可根据需求调整
uint8_t uart_rx_buf[UART_RX_BUF_SIZE];  // 接收缓冲区
uint16_t uart_rx_head = 0;              // 缓冲区头指针（写）
uint16_t uart_rx_tail = 0;              // 缓冲区尾指针（读）
uint8_t uart_rx_temp;                   // 中断接收临时变量
uint8_t servo_state[2] = {0, 0};

/**
 * @brief 串口初始化
 */
void UART_Init(void)
{
	// 2. 串口DMA接收初始化（核心步骤）
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);  // 开启空闲中断
  HAL_UART_Receive_DMA(&huart1, uart_rx_buf, UART_RX_BUF_SIZE);  // 启动DMA
}
/**
 * @brief 串口接收完成回调函数（中断触发后执行）
 */
void HAL_UART_RxCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    /*  */
		memcpy(servo_state, uart_rx_buf, 2);
    // 重启DMA接收
    HAL_UART_Receive_DMA(&huart1, uart_rx_buf, UART_RX_BUF_SIZE);
  }
}
