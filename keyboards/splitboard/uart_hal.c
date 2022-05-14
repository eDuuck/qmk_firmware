#include "uart_hal.h"

volatile static uint8_t uart_tx_busy = 1;
const char* numb = "0123456789";

ISR(USART1_RX_vect){
	
}

ISR(USART1_TX_vect){
	uart_tx_busy = 1;
}

void uart_init(uint32_t baud, uint8_t high_speed){
	uint8_t speed = 16;
	if(high_speed != 0){
		speed = 8;
		UCSR1A |= 1 << U2X1;
	}
	baud = (F_CPU/(speed*baud)) - 1;
	UBRR1H = (baud & 0x0F00) >> 8;
	UBRR1L = (baud & 0x00FF);
	UCSR1B |= (1 << TXEN1)| (1 << TXCIE1) | (1 << RXEN1) | (1 << RXCIE1);
}

void uart_send_byte(uint8_t c){
	while(uart_tx_busy == 0);
	uart_tx_busy = 0;
	UDR1 = c;
}

void uart_send_number(int c){
	char string[20];
	itoa(c,string,10);
	uart_send_string((uint8_t*)string);
}

void uart_send_array(uint8_t *c, uint16_t len){
	for(uint16_t i = 0; i < len; i++){
		uart_send_byte(c[i]);
	}
}

void uart_send_string(uint8_t *c){
	uint16_t i = 0;
	
	do{
		uart_send_byte(c[i]);
		i++;
	}while(c[i] != '\0');
	uart_send_byte('\n');
	uart_send_byte('\r');
	uart_send_byte(c[i]);
}