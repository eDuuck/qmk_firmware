
#include "splitboard.h"
#include "config.h"

void uart_init(uint32_t baud,uint8_t high_speed);
void uart_send_byte(uint8_t c);
void uart_send_array(uint8_t *c, uint16_t len);
void uart_send_string(uint8_t *c);
void uart_send_number(int c);