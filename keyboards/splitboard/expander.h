#ifndef EXPANDER_H
#define EXPANDER_H

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "splitboard.h"
#include "config.h"

extern uint8_t expander_status;

#define TCA9555
#define TCA9555_A0 0
#define TCA9555_A1 0
#define TCA9555_A2 0

#define PCA9634
#define PCA9634_A0 1
#define PCA9634_A1 1
#define PCA9634_A2 1
#define PCA9634_A3 1
#define PCA9634_A4 1
#define PCA9634_A5 0
#define PCA9634_A6 0

#ifdef TCA9555
#define IO_EXPANDER_ADDR ((0x20|(TCA9555_A0<<0)|(TCA9555_A1<<1)|(TCA9555_A2<<2)) << 1)
enum IO_EXPANDER_REG_BANK0 {
  IO_EXP_INPA = 0,
  IO_EXP_INPB,
  IO_EXP_OUTA,
  IO_EXP_OUTB,
  IO_EXP_POLA,
  IO_EXP_POLB,
  IO_EXP_CONFA,
  IO_EXP_CONFB
};
#endif

//#ifdef PCA9634
#define LED_EXPANDER_ADDR (((PCA9634_A0<<0)|(PCA9634_A1<<1)|(PCA9634_A2<<2)|(PCA9634_A3<<3)|(PCA9634_A4<<4)|(PCA9634_A5<<5)|(PCA9634_A6<<6)) << 1)
enum LED_EXPANDER_REG_BANK0 {
	LED_EXP_MODE1 = 0,
	LED_EXP_MODE2,
	LED_EXP_PWM0,
	LED_EXP_PWM1,
	LED_EXP_PWM2,
	LED_EXP_PWM3,
	LED_EXP_PWM4,
	LED_EXP_PWM5,
	LED_EXP_PWM6,
	LED_EXP_PWM7,
	LED_EXP_GRPPWM,
	LED_EXP_GRPFREQ,
	LED_EXP_LEDOUT0,
	LED_EXP_LEDOUT1,
	LED_EXP_SUBADR1,
	LED_EXP_SUBADR2,
	LED_EXP_SUBADR3,
	LED_EXP_ALLCALLADR
};
//#endif

i2c_status_t expander_init(void);
void expander_read_cols(void);
uint8_t expander_get_col(uint8_t col);
uint8_t expander_brightness_set(uint8_t led_pwm, uint8_t caps_pwm);
matrix_row_t expander_read_row(uint8_t row);
void expander_unselect_rows(void);
void expander_select_row(uint8_t row);
i2c_status_t exp_scan(void);
matrix_row_t expander_rep_read(uint8_t row);

#endif
