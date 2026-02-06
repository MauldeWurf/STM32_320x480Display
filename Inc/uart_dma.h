#ifndef __UART_DMA_H__
#define __UART_DMA_H__
#include <stdint.h>
#include "stm32f4xx.h"

#define UART_DATA_BUFF_SIZE		5

void uart2_rx_tx_init(void);
void dma1_init(void);
void dma1_stream5_uart_rx_config(void);
void dma1_stream6_uart_tx_config(uint32_t msg_to_snd, uint32_t msg_len);
void DMA1_Stream6_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);
void USART2_IRQHandler(void);

#endif
