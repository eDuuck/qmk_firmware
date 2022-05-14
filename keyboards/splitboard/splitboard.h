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

#pragma once

#include "quantum.h"
#include <stdint.h>
#include <stdbool.h>
#include "i2c_master.h"



extern bool numlock;
extern uint8_t pwm;

#define I2C_TIMEOUT 10

void set_backlight_PWM(uint8_t value);

#define ___ KC_NO
/* This is a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
/*
#define LAYOUT(									\
    R00,R01,R02,R03,R04,R05,	R07,			\
    R10,R11,R12,R13,R14,R15,R16,R17,			\
	R20,R21,R22,R23,R24,R25,R26,R27,			\
	R30,R31,R32,R33,R34,R35,R36,R37,			\
		R41,R42,R43,R44,R45,R46,R47,			\
	R50,R51,	R53,	R55,R56,R57)			\
{												\
    { R00, R01, R02, R03, R04, R05, ___, R07 }, \
    { R10, R11, R12, R13, R14, R15, R16, R17 }, \
	{ R20, R21, R22, R23, R24, R25, R26, R27 }, \
	{ R30, R31, R32, R33, R34, R35, R36, R37 }, \
	{ ___, R41, R42, R43, R44, R45, R46, R47 }, \
	{ R50, R51, R53, ___, ___, R55, R56, R57 }  \
}*/


#define LAYOUT(																									\
L00,L01,L02,L03,L04,L05,L06,																					\
L10,L11,L12,L13,L14,L15,L16,																					\
L20,L21,L22,L23,L24,L25,L26,																					\
L30,L31,L32,L33,L34,L35,L36,																					\
L40,L41,L42,L43,L44,L45,																						\
L50,L51,	L53,	L55,L56,																					\
																												\
								R00,R01,R02,R03,R04,R05,	R07,												\
								R10,R11,R12,R13,R14,R15,R16,R17,												\
								R20,R21,R22,R23,R24,R25,R26,R27,												\
								R30,R31,R32,R33,R34,R35,R36,R37,												\
									R41,R42,R43,R44,R45,R46,R47,												\
								R50,R51,	R53,	R55,R56,R57 )												\
																											 \
{																											 \
	{ L00,  L01,  L02,  L03,  L04,  L05,  L06,  ___,		R00,  R01,  R02,  R03,  R04,  R05,  ___,  R07 }, \
	{ L10,  L11,  L12,  L13,  L14,  L15,  L16,  L51,		R10,  R11,  R12,  R13,  R14,  R15,  R16,  R17 }, \
	{ L20,  L21,  L22,  L23,  L24,  L25,  L26,  L50,		R20,  R21,  R22,  R23,  R24,  R25,  R26,  R27 }, \
	{ L30,  L31,  L32,  L33,  L34,  L35,  L36,  L53,		R30,  R31,  R32,  R33,  R34,  R35,  R36,  R37 }, \
	{ L40,  L41,  L42,  L43,  L44,  L45,  L55,  L56,		___,  R41,  R42,  R43,  R44,  R45,  R46,  R47 }, \
	{ ___,	___,  ___,  ___,  ___,  ___,  ___,  ___,		R50,  R51,  R53,  ___,  ___,  R55,  R56,  R57 }  \
}

//static const uint8_t pwm_values[32] = {0,11,11,11,12,12,12,13,14,14,15,17,18,20,21,24,27,30,34,39,44,51,59,69,80,94,110,130,153,181,215,255};