/* Copyright 2021 eDuuck
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <avr/io.h>
#include "splitboard.h"
#include "config.h"
#include "expander.h"
#include <avr/interrupt.h>

bool numlock = false;
uint8_t pwm = 0;
void set_backlight_PWM(uint8_t value);
void PWM_init(void);

void matrix_init_kb(void) {
	sei();
	PWM_init();
	
	//Sets unused pins to inputs.
	DDRB  &= ~(1<<PB7);
	PORTB |=  (1<<PB7);
	DDRD  &= ~(1<<PD5 | 1<<PD6 | 1<<PD7);
	PORTD |=  (1<<PD5 | 1<<PD6 | 1<<PD7);
	
	DDRD &= ~(1<<PD4); //Sets the aux status pin to input.
	PORTD |= (1<<PD4);
	

	
	numlock = false;
	set_backlight_PWM(31);
	wait_ms(500);
	set_backlight_PWM(0);
	wait_ms(500);
	set_backlight_PWM(31);
	wait_ms(500);
	set_backlight_PWM(0);
}

void set_backlight_PWM(uint8_t value){
	if(numlock){
		OCR1A = 0xFF;
	}else{
		OCR1A = value;//pwm_values[value];
	}
	OCR1B = value;//spwm_values[value];
	expander_brightness_set(value,value);
}
	
void PWM_init(void){
	TCCR1A = (1 << COM1A1 | 0 << COM1A0 | 1 << COM1B1 | 0 << COM1B0 | 0 << WGM11 | 1 << WGM10); //Set output for fast PWM operation and compare 
	TCCR1B = (0 << CS11 | 1 << CS10  | 0 << WGM13 | 1 << WGM12); //Set prescaler to 64
	//ICR1 = 0xFFFF;
	wait_ms(1);
}