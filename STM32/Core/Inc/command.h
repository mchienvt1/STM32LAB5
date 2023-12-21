/*
 * command.h
 *
 *  Created on: Dec 21, 2023
 *      Author: lmchi
 */

#ifndef INC_COMMAND_H_
#define INC_COMMAND_H_

#include "main.h"
#include "string.h"
#include "software_timer.h"

#define MAX_BUFFER_SIZE  30

#define WAIT_COM	1
#define GET	2
#define WAIT_UART	10
#define RESPONE		11
#define LOOP		12

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

uint8_t temp;
uint8_t buffer_flag;
uint32_t ADC_value;
uint8_t str[10];
uint8_t buffer[MAX_BUFFER_SIZE];

int com_state;
int uart_state;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void command_parser_fsm();
void uart_communication_fsm();

#endif /* INC_COMMAND_H_ */
