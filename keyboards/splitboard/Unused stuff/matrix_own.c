#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "splitboard.h"
#include "uart_hal.h"
#include "wait.h"
#include "action_layer.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "debounce.h"


extern matrix_row_t matrix[MATRIX_ROWS];      // debounced values
extern matrix_row_t raw_matrix[MATRIX_ROWS];  // raw values

static matrix_row_t read_cols(uint8_t row);
static void         init_cols(void);
static void         unselect_rows(void);
//static void			unselect_row(uint8_t row);
static void         select_row(uint8_t row);

void matrix_init_custom(void){
	init_cols();
	unselect_rows();
}

// Reads and stores a row, returning
// whether a change occurred.
static inline bool store_raw_matrix_row(uint8_t index) {
	matrix_row_t temp = read_cols();
	if (raw_matrix[index] != temp) {
		uart_send_byte(temp);
		raw_matrix[index] = temp;
		return true;
	}
	return false;
}


bool matrix_scan_custom(matrix_row_t current_matrix[]){
	bool matrix_has_changed = false;
	for(uint8_t i = 0; i < MATRIX_ROWS; i++){
		select_row(i);
		wait_us(30);
		matrix_has_changed |= store_raw_matrix_row(i);
		unselect_rows();
	}
	return matrix_has_changed;
}
 
/* Column pin configuration
 * ATMEGA32U4:	7	 6    5    4    3    2    1    0
 *				PD2	 PD3  PB0  PB1  PB2  PB3  PB4  PF4
 */
static void init_cols(void)
{
  // ATMEGA32U4
	DDRB  &= ~(1<<PB0 | 1<<PB1 | 1<<PB2 | 1<<PB3 | 1<<PB4); //Set pin as input.
	PORTB |=  (1<<PB0 | 1<<PB1 | 1<<PB2 | 1<<PB3 | 1<<PB4); //Activate pull-up.
	DDRD  &= ~(1<<PD2 | 1<<PD3);
	PORTD |=  (1<<PD2 | 1<<PD3);
	DDRF  &= ~(1<<PF4);
	PORTF |=  (1<<PF4);
}


static matrix_row_t read_cols()
{
	matrix_row_t cols = 
	(PIND&(1<<PD2) ? 0 : (1<<7)) | //If PD2 = 1, assign bit 0, otherwise assign 1.
	(PIND&(1<<PD3) ? 0 : (1<<6)) | //True since we pull down on pushing button.
	(PINB&(1<<PB0) ? 0 : (1<<5)) |
	(PINB&(1<<PB1) ? 0 : (1<<4)) |
	(PINB&(1<<PB2) ? 0 : (1<<3)) |
	(PINB&(1<<PB3) ? 0 : (1<<2)) |
	(PINB&(1<<PB4) ? 0 : (1<<1)) |
	(PINF&(1<<PF4) ? 0 : (1<<0)) ;
	return cols;
}

// Atmega32u4:
// Row	:	0	1	2	3	4	5
// Pin	:	E6	C6	C7	F7	F6	F5
static void unselect_rows(void) {
	DDRC &= ~(1<<PC6 | 1<<PC7);
	PORTC |= (1<<PC6 | 1<<PC7);
	DDRE &= ~(1<<PE6);
	PORTE |= (1<<PE6);
	DDRF &= ~(1<<PF5 | 1<<PF6 | 1<<PF7);
	PORTF |= (1<<PF5 | 1<<PF6 | 1<<PF7);
}


// static void unselect_row(uint8_t row) {
// 	switch (row)
// 	{
// 		case 0:
// 			DDRE &= ~(1<<PE6); 
// 			PORTE |= (1<<PE6); 
// 			break;
// 		case 1:
// 			DDRC &= ~(1<<PC6); 
// 			PORTC |= (1<<PC6); 
// 			break;
// 		case 2:
// 			DDRC &= ~(1<<PC7); 
// 			PORTC |= (1<<PC7); 
// 			break;
// 		case 3:
// 			DDRF &= ~(1<<PF7);
// 			PORTF |= (1<<PF7); 
// 			break;
// 		case 4:
// 			DDRF &= ~(1<<PF6); 
// 			PORTF |= (1<<PF6);
// 			break;
// 		case 5:
// 			DDRF &= ~(1<<PF5); 
// 			PORTF |= (1<<PF5);
// 			break;
// 	}
// }

// Atmega32u4:
// Row	:	0	1	2	3	4	5
// Pin	:	E6	C6	C7	F7	F6	F5
static void select_row(uint8_t row){
	switch (row)
	{
		case 0:
			DDRE |= (1<<PE6); //Set pin as output
			PORTE &= ~(1<<PE6); //Set output as low
			break;
		case 1:
			DDRC |= (1<<PC6); //Set pin as output
			PORTC &= ~(1<<PC6); //Set output as low
			break;
		case 2:
			DDRC |= (1<<PC7); //Set pin as output
			PORTC &= ~(1<<PC7); //Set output as low
			break;
		case 3:
			DDRF |= (1<<PF7); //Set pin as output
			PORTF &= ~(1<<PF7); //Set output as low
			break;		
		case 4:
			DDRF |= (1<<PF6); //Set pin as output
			PORTF &= ~(1<<PF6); //Set output as low
			break;
		case 5:
			DDRF |= (1<<PF5); //Set pin as output
			PORTF &= ~(1<<PF5); //Set output as low
			break;
	}
}

void matrix_power_up(void) {
	unselect_rows();
	init_cols();

	// initialize matrix state: all keys off
	for (uint8_t i=0; i < MATRIX_ROWS; i++) {
		matrix[i] = 0;
	}

}