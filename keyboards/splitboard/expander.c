#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "action.h"
#include "expander.h"
#include "i2c_master.h"
#include "debug.h"
#include "wait.h"

uint8_t expander_status = 0;
static uint8_t expander_input = 0;

void exp_config(void);
uint8_t expander_write(uint8_t addr,uint8_t reg, uint8_t data);
uint8_t expander_read(uint8_t addr,uint8_t reg, uint8_t *data);
uint8_t expander_brightness_set(uint8_t led_pwm, uint8_t caps_pwm);

bool    i2c_initialized = 0;
bool	exp_configed = 0;


i2c_status_t expander_init(void){
	wait_us(30);
	if (i2c_initialized == 0) {
		i2c_init();  // on pins D(1,0)
		i2c_initialized = true;
		_delay_ms(100);
	}
	return exp_scan();
}

i2c_status_t exp_scan(void){
	if((PIND&(1<<PD4))){ //When aux cable is connected, R_CONN is pulled high.
		dprintf("expander status: %d ... ", expander_status);
		i2c_status_t ret = i2c_start(IO_EXPANDER_ADDR | I2C_WRITE, I2C_TIMEOUT);
		if (ret == I2C_STATUS_SUCCESS) {
			i2c_stop();
			if (expander_status == 0) {
				dprintf("attached\n");
				expander_status = 1;
				if(exp_configed == 0){
					exp_config();
					exp_configed = 1;
				}
				clear_keyboard();
			}
		} else {
			if (expander_status == 1) {
				dprintf("detached\n");
				expander_status = 0;
				exp_configed = 0;
				clear_keyboard();
			}
		}
		dprintf("%d\n", expander_status);
		return ret;
	}else{
		dprintf("Aux cable not connected... ");
		expander_status = 0;
		exp_configed = 0;
		return I2C_STATUS_ERROR;
	}
}

void expander_read_cols(void){
	if(expander_read(IO_EXPANDER_ADDR,IO_EXP_INPA, &expander_input)){
		expander_input = 0;
	}; //Read register A on expander
}

uint8_t expander_get_col(uint8_t col){
	if(col == 7){		//The pins on the I/O expander are setup as 7,0,1,2,3,4,5,6 
		col = 0;
	}else{
		col++;		//This code just remaps the variable col to the right pin.
	}
	return expander_input & ( 1<<col ) ? 1 : 0;
}

matrix_row_t expander_read_row(uint8_t row){
	if(row > 4){
		return 0;
	}
	expander_read_cols(); //This fetches the data from the expander.

	/* make cols */
	matrix_row_t cols = 0;
	for (uint8_t col = 0; col < MATRIX_COLS_LEFT; col++) {
		if (expander_get_col(col)) {
			cols |= (1UL << col);
		}
	}
	return cols;
}

matrix_row_t expander_rep_read(uint8_t row){
	if(row > 4){
		return 0;
	}
	uint8_t ret = i2c_start(IO_EXPANDER_ADDR | I2C_READ, I2C_TIMEOUT); if (ret) goto stop;
	expander_input = (uint8_t)i2c_read_nack(I2C_TIMEOUT);

	/* make cols */
	matrix_row_t cols = 0;
	for (uint8_t col = 0; col < MATRIX_COLS_LEFT; col++) {
		if (expander_get_col(col)) {
			cols |= (1UL << col);
		}
	}
	return cols;
	
	stop:
	return 0;
}

void expander_unselect_rows(void)
{
  expander_write(IO_EXPANDER_ADDR,IO_EXP_OUTB, 0xFF); //Preferably not used
}

void expander_select_row(uint8_t row)
{
	if (expander_status == 0 || row == 5) {
		wait_us(30);
	}else{
		expander_write(IO_EXPANDER_ADDR, IO_EXP_OUTB, ~(1<<(6-row))); //Write 0 to selected row on expander (PORT B)
	}
}

void led_exp_config(void)
{
	expander_write(LED_EXPANDER_ADDR, LED_EXP_MODE1, 0); //This turns on the internal oscillator which is needed for PWM.
	expander_write(LED_EXPANDER_ADDR, LED_EXP_MODE2, 1<<4); //This inverts the LED_Outputs which is needed since the pins are connected to N-MOS.
	expander_write(LED_EXPANDER_ADDR, LED_EXP_LEDOUT0, 0xF);
}

void exp_config(void)
{
  expander_write(IO_EXPANDER_ADDR, IO_EXP_CONFA, 0xFF); //Sets pins 0-7 to inputs
  expander_write(IO_EXPANDER_ADDR, IO_EXP_POLA, 0xFF); //Sets polarity inversion for the reading pins.
  expander_write(IO_EXPANDER_ADDR, IO_EXP_CONFB, 0); //Sets pin 10-17 of the TCA9555 to outputs.
  expander_write(IO_EXPANDER_ADDR, IO_EXP_OUTB, 0xFF); //Sets all outputs as high.
  
  led_exp_config();
}

uint8_t expander_write(uint8_t addr, uint8_t reg, uint8_t data)
{
	if (expander_status == 0) {
		return 0;
	}
	uint8_t ret;
	ret = i2c_start(addr | I2C_WRITE, I2C_TIMEOUT);	if (ret) goto stop;
	ret = i2c_write(reg, I2C_TIMEOUT); if (ret) goto stop;
	ret = i2c_write(data, I2C_TIMEOUT); if (ret) goto stop;

	i2c_stop();
	return ret;
	
	stop:
	i2c_stop();
	exp_scan();
	return ret;
}

uint8_t expander_read(uint8_t addr,uint8_t reg, uint8_t *data){
	if (expander_status == 0) {
		return 0;
	}
	uint8_t ret;
	ret = i2c_start(addr | I2C_WRITE, I2C_TIMEOUT); if (ret) goto stop;
	ret = i2c_write(reg, I2C_TIMEOUT); if (ret) goto stop;
	ret = i2c_start(addr | I2C_READ, I2C_TIMEOUT); if (ret) goto stop;
	*data = (uint8_t)i2c_read_nack(I2C_TIMEOUT);

	i2c_stop();
	return ret;
	
	stop:
	i2c_stop();
	exp_scan();
	return ret;
}

uint8_t expander_brightness_set(uint8_t led_pwm, uint8_t caps_pwm){
	if (expander_status == 0) {
		return 0;
	}
	uint8_t ret;
	ret = i2c_start(LED_EXPANDER_ADDR | I2C_WRITE, I2C_TIMEOUT); if (ret) goto stop;
	ret = i2c_write(LED_EXP_PWM0 | (1<<7) | (1 << 5), I2C_TIMEOUT); if (ret) goto stop;
	ret = i2c_write(led_pwm, I2C_TIMEOUT); if (ret) goto stop;
	ret = i2c_write(caps_pwm, I2C_TIMEOUT); if (ret) goto stop;
	
	i2c_stop();
	return ret;
	
	stop:
	i2c_stop();
	exp_scan();
	return ret;
}

