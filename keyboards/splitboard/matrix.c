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
#include "expander.h"

#include "bootloader.c"

/*
 * This constant define not debouncing time in msecs, but amount of matrix
 * scan loops which should be made to get stable debounced results.
 *
 * On Ergodox matrix scan rate is relatively low, because of slow I2C.
 * Now it's only 317 scans/second, or about 3.15 msec/scan.
 * According to Cherry specs, debouncing time is 5 msec.
 *
 * And so, there is no sense to have DEBOUNCE higher than 2.
 */

#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif

static uint8_t expander_reset_loop;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];

// Debouncing: store for each key the number of scans until it's eligible to
// change.  When scanning the matrix, ignore any changes in keys that have
// already changed in the last DEBOUNCE scans.
static uint8_t debounce_matrix[MATRIX_ROWS * MATRIX_COLS];

static matrix_row_t read_cols(uint8_t row);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);
static uint8_t sumBits(uint8_t byte);
static void unselect_row(uint8_t row);

__attribute__ ((weak))
void matrix_init_user(void) {}

__attribute__ ((weak))
void matrix_scan_user(void) {}

__attribute__ ((weak))
void matrix_init_kb(void) {
	matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
	matrix_scan_user();
}

inline
uint8_t matrix_rows(void){
	return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void){
	return MATRIX_COLS;
}

void matrix_init(void)
{
	expander_init();
	
	unselect_rows();
	init_cols();

	// initialize matrix state: all keys off
	for (uint8_t i=0; i < MATRIX_ROWS; i++) {
		matrix[i] = 0;
		for (uint8_t j=0; j < MATRIX_COLS; ++j) {
			debounce_matrix[i * MATRIX_COLS + j] = 0;
		}
	}
	matrix_init_quantum();
}

void matrix_power_up(void) {
	expander_init();
	
	unselect_rows();
	init_cols();

	// initialize matrix state: all keys off
	for (uint8_t i=0; i < MATRIX_ROWS; i++) {
		matrix[i] = 0;
	}
}

// Returns a matrix_row_t whose bits are set if the corresponding key should be
// eligible to change in this scan.
matrix_row_t debounce_mask(uint8_t row) {
  matrix_row_t result = 0;
  for (uint8_t j=0; j < MATRIX_COLS; ++j) {
    if (debounce_matrix[row * MATRIX_COLS + j]) {
      --debounce_matrix[row * MATRIX_COLS + j];
    } else {
      result |= (1 << j);
    }
  }
  return result;
}

// Report changed keys in the given row.  Resets the debounce countdowns
// corresponding to each set bit in 'change' to DEBOUNCE.
void debounce_report(matrix_row_t change, uint8_t row) {
  for (uint8_t i = 0; i < MATRIX_COLS; ++i) {
    if (change & (1 << i)) {
      debounce_matrix[row * MATRIX_COLS + i] = DEBOUNCE;
    }
  }
}

uint8_t matrix_scan(void)
{
	if(!expander_status){	//If not expander connected, start cycling.
		if(++expander_reset_loop == 0){
			exp_scan();
		}
	}

	for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
		select_row(i);
		//wait_us(30);  // without this wait read unstable value. (Not really since it takes ~100 us to select a register and write over i2c (Added this delay in select_row instead) )
		matrix_row_t mask = debounce_mask(i);
		matrix_row_t cols = (read_cols(i) & mask) | (matrix[i] & ~mask);
		debounce_report(cols ^ matrix[i], i);
		matrix[i] = cols;

		unselect_row(i);
	}

  matrix_scan_quantum();

  return 1;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
  return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
  return matrix[row];
}

void matrix_print(void)
{
  print("\nr/c 0123456789ABCDEF\n");
  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    print_hex8(row); print(": ");
    print_bin_reverse16(matrix_get_row(row));
    print("\n");
  }
}

uint8_t matrix_key_count(void)
{
  uint8_t count = 0;
  for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    count += bitpop16(matrix[i]);
  }
  return count;
}

/* Column pin configuration
 *
 * ATMEGA32U4:	7	 6    5    4    3    2    1    0
 *				PD2	 PD3  PB0  PB1  PB2  PB3  PB4  PF4
 *
 * TCA9555:		15	 14	  13   12   11   10   9    8
 *				P01  P02  P03  P04  P05  P06  P07  P00
 */
static void  init_cols(void)
{
  // ATMEGA32U4
  DDRB  &= ~(1<<PB0 | 1<<PB1 | 1<<PB2 | 1<<PB3 | 1<<PB4); //Setting register DDxn to 0 sets the pin as an input.
  PORTB |=  (1<<PB0 | 1<<PB1 | 1<<PB2 | 1<<PB3 | 1<<PB4); //Setting register PORTxn to 1 activates pull-up on pin.
  DDRD  &= ~(1<<PD2 | 1<<PD3);
  PORTD |=  (1<<PD2 | 1<<PD3);
  DDRF  &= ~(1<<PF4);
  PORTF |=  (1<<PF4);

}

/* Column pin configuration
 *
 * ATMEGA32U4:	8	 9    10   11   12   13   14   15
 *				PD2	 PD3  PB0  PB1  PB2  PB3  PB4  PF4
 *
 * TCA9555:		0	 1	  2	   3    4    5    6    7
 *				P01  P02  P03  P04  P05  P06  P07  P00
 */
static matrix_row_t read_cols(uint8_t row)
{
	uint8_t left_hand_row = expander_read_row(row);
	while(sumBits(left_hand_row) > 5){
		left_hand_row = expander_read_row(row);
	}
	return left_hand_row|
		(PIND&(1<<PD2) ? 0 : (1UL<<8)) |
		(PIND&(1<<PD3) ? 0 : (1UL<<9)) |
		(PINB&(1<<PB0) ? 0 : (1UL<<10)) |
		(PINB&(1<<PB1) ? 0 : (1UL<<11)) |
		(PINB&(1<<PB2) ? 0 : (1UL<<12)) |
		(PINB&(1<<PB3) ? 0 : (1UL<<13)) |
		(PINB&(1<<PB4) ? 0 : (1UL<<14)) |
		(PINF&(1<<PF4) ? 0 : (1UL<<15)) ;
}

/* Row pin configuration
 *
 * ATMEGA32U4: 0   1   2   3   4   5
 *             E6  C6  C7  F7  F6  F5
 *
 */
static void unselect_rows(void)	//Just sets all output pins as inputs.
{
  // ATMEGA32U4
  
  DDRC  &= ~(1<<PC6 | 1<<PC7);
  PORTC &= ~(1<<PC6 | 1<<PC7);
  DDRE  &= ~(1<<PE6);
  PORTE &= ~(1<<PE6);
  DDRF  &= ~(1<<PF5 | 1<<PF6 | 1<<PF7);
  PORTF &= ~(1<<PF5 | 1<<PF6 | 1<<PF7);

}

static void select_row(uint8_t row){
	// ATMEGA32U4
	switch (row) {
	case 0:
		DDRE  |=  (1<<PE6);
		PORTE &= ~(1<<PE6);
		break;
	case 1:
		DDRC  |=  (1<<PC6);
		PORTC &= ~(1<<PC6);
		break;
	case 2:
		DDRC  |=  (1<<PC7);
		PORTC &= ~(1<<PC7);
		break;
	case 3:
		DDRF  |=  (1<<PF7);
		PORTF &= ~(1<<PF7);
		break;
	case 4:
		DDRF  |=  (1<<PF6);
		PORTF &= ~(1<<PF6);
		break;
	case 5:
		DDRF  |=  (1<<PF5);
		PORTF &= ~(1<<PF5);
		break;
	}
	//TCA9555
	expander_select_row(row); //This also includes the delay.
}

static uint8_t sumBits(uint8_t byte){
	uint8_t i = 0;
	uint8_t sum = 0;
	while(byte >> i != 0){
		sum += (byte>>i) & 0b1;
		i++;
	}
	return sum;
}

static void unselect_row(uint8_t row)
{
	switch (row) {
		case 0:
		DDRE  &= ~(1<<PE6);
		PORTE |=  (1<<PE6);
		break;
		case 1:
		DDRC  &= ~(1<<PC6);
		PORTC |=  (1<<PC6);
		break;
		case 2:
		DDRC  &= ~(1<<PC7);
		PORTC |=  (1<<PC7);
		break;
		case 3:
		DDRF  &= ~(1<<PF7);
		PORTF |=  (1<<PF7);
		break;
		case 4:
		DDRF  &= ~(1<<PF6);
		PORTF |=  (1<<PF6);
		break;
		case 5:
		DDRF  &= ~(1<<PF5);
		PORTF |=  (1<<PF5);
		break;
	}
}