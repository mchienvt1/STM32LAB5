/*
 * command.c
 *
 *  Created on: Dec 21, 2023
 *      Author: lmchi
 */

#include "command.h"
#include "stdio.h"

uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

uint32_t ADC_value;
uint8_t str[10];

int com_state = WAIT_COM;
int uart_state = WAIT_UART;

char* get_command(){
	return (char*) buffer;
}


void clear_command(){
	for (int i = 0; i < MAX_BUFFER_SIZE; i++){
		buffer[i] = '\0';
	}
	index_buffer = 0;
	buffer_flag = 0;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART2){
		buffer_flag = 1;
		HAL_UART_Transmit(&huart2 , &temp, 1, 50);
		HAL_UART_Receive_IT(&huart2, &temp, 1);
	}
}

void command_parser_fsm(){
	switch (com_state) {
		case WAIT_COM:
			if (temp == '!') com_state = GET;
			break;
		case GET:
			if (temp == '#'){
				com_state = WAIT_COM;
				uart_state = RESPONE;
			}
			else {
				buffer[index_buffer++] = temp;
				if (index_buffer >= MAX_BUFFER_SIZE){
					com_state = WAIT_COM;
					index_buffer = 0;
					clear_command();
				}
			}
			break;
		default:
			break;
	}
}

void uart_communication_fsm(){
	switch (uart_state) {
		case WAIT_UART:
			break;
		case RESPONE:
			if (strcmp(get_command(), "RST") == 0){
				ADC_value = HAL_ADC_GetValue(&hadc1);
				HAL_UART_Transmit(&huart2, (void *)str,  sprintf(str, "\r\n!ADC=%d#", ADC_value), 1000);
				uart_state = LOOP;
				clear_command();
				setTimer0(300);
			}
			else {
				uart_state = WAIT_UART;
			}
			clear_command();
			break;
		case LOOP:
			if (timer0_flag == 1){
				ADC_value = HAL_ADC_GetValue(&hadc1);
				HAL_UART_Transmit(&huart2, (void*) str,  sprintf(str, "\r\n!ADC=%d#", ADC_value), 1000);
				setTimer0(300);
			}
			if (strcmp(get_command(), "OK") == 0){
				uart_state = WAIT_UART;
			}
			break;
		default:
			break;
	}
}
