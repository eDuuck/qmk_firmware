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
#include QMK_KEYBOARD_H
#include "splitboard.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE = 0,
	_NUMP,
	_GAMING
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
	NUMPAD
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
/* Keymap 0: Basic layer
 * ,------.  ,---------------------------.  .-------------.                .-------------.  ,---------------------------.    /-----\
 * | ESC  |  |  F1  |  F2  |  F3  |  F4  |  |  F5  |  F6  |                |  F7  |  F8  |  |  F9  |  F10 |  F11 |  F12 |   |       |
 * |      |  |      |      |      |      |  |      |      |                |      |      |  |      |      |      |      |   |       |
 * `------'  `---------------------------'  `-------------'	               `-------------'  `---------------------------'    \-----/
 *                     ,--------------------.                                            ,--------------------.
 *       ,-------------|  "   |  #   |  ¤   |------.  ,------.          ,------.  ,------|  / { |  ( ] |  ) ] |--------------------.
 *       | DEL  |  !   |  2 @ |  3 £ |  4 $ |  %   |  | HOME |          | Page |  |  &   |  7   |  8   |  9   |  = } |  ? \ | Num  |
 *       |      |  1   |------+------+------|  5   |  |      |          | Up   |  |  6   |------+------+------|  0   |  +   | Lock |
 *    ,---------+------|   E  |   R  |   T  |------|  |------|          |------|  |------|   U  |   I  |   O  |------+------+------|
 *    |   TAB   |   Q  |      |      |      |   Y  |  | END  |          | Page |  |   Y  |      |      |      |   P  |   Å  |  ^ ~ |
 *    |         |      |------+------+------|      |  |      |          | Down |  |      |------+------+------|      |      |  ¨   |
 * ,------------+------|   S  |   D  |   F  |------|  |------|          |------|  |------|   J  |   K  |   L  |------+------+------|
 * | Caps       |   A  |      |      |      |   G  |  | BK   |          | BK   |  |   H  |      |      |      |   Ö  |   Ä  |  *   |
 * | Lock       |      |------+------+------|      |  |      |          |      |  |      |------+------+------|      |      |  '   |
 * |------------+------|   Z  |   X  |   C  |------|  `------'          `------'  |------|   N  |   M  |   ;  |------+------+------|
 * | Left       |  >   |      |      |      |   V  |                              |   B  |      |      |   ,  |   :  |      |   _  |
 * | Shift      |  < | |--------------------|      |       	                      |      |--------------------|   .  |      |   -  |
 * `------------+------|                    `------'   		                      `------'                    |------+------+------|
 *    |  CTRL   | Sys  |          ,---------.                                            ,---------.          |      |      |      |
 *    |         |      |          |   Alt   |  ,-------------.          ,-------------.  |   Alt   |          |      |      |      |
 *    `----------------'          |         |  |      |      |          |      |      |  |         |          `--------------------'
 *                                `---------'  |      |      |          |      |      |  `---------'
 *                                             |Back  |Delete|          |Enter |Space |
 *                                             |space |      |          |      |      |
 *                                             |      |      |          |      |      |
 *                                             `-------------'          `-------------'
 */
[_BASE] = LAYOUT(  // layer 0 : default   KC_NONUS_BSLASH
// left hand
KC_ESC,		KC_F1,		KC_F2,		KC_F3,		KC_F4,		KC_F5,		KC_F6,
KC_DEL,		KC_1,		KC_2,		KC_3,		KC_4,		KC_5,		KC_HOME,
KC_TAB,		KC_Q,		KC_W,		KC_E,		KC_R,		KC_T,		KC_END,
KC_CAPS,	KC_A,		KC_S,		KC_D,		KC_F,		KC_G,		TG(_GAMING),
KC_LSFT,	KC_NUBS,	KC_Z,		KC_X,		KC_C,		KC_V,
KC_LCTL,	KC_LGUI,				KC_LALT,				KC_BSPACE,	KC_DEL,

// right hand
KC_F7,		KC_F8,		KC_F9,		KC_F10,		KC_F11,		KC_F12,						KC_AUDIO_MUTE,
KC_PGUP,	KC_6,		KC_7,		KC_8,		KC_9,		KC_0,		KC_MINUS,		NUMPAD,
KC_PGDOWN,	KC_Y,		KC_U,		KC_I,		KC_O,		KC_P,		KC_LBRACKET,	KC_RBRACKET,
KC_PSCREEN,	KC_H,		KC_J,		KC_K,		KC_L,		KC_SCOLON,	KC_QUOTE,		KC_NONUS_HASH,
			KC_B,		KC_N,		KC_M,		KC_COMMA,	KC_DOT,		KC_UP,			KC_SLASH,
KC_ENTER,	KC_SPACE,				KC_RALT,				KC_LEFT,	KC_DOWN,		KC_RIGHT
),
[_GAMING] = LAYOUT(
// left hand
_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	KC_1,		KC_2,		KC_3,		KC_4,		KC_5,		KC_HOME,
_______,	KC_T,		KC_Q,		KC_W,		KC_E,		KC_R,		KC_END,
_______,	KC_G,		KC_A,		KC_S,		KC_D,		KC_F,		TG(_GAMING),
_______,	KC_NUBS,	KC_Z,		KC_X,		KC_C,		KC_V,
_______,	_______,				_______,				KC_SPACE,	KC_BSPC,

// right hand
_______,	_______,	_______,	_______,	_______,	_______,				_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,				_______,				_______,	_______,	_______
),
[_NUMP] = LAYOUT(
    // left hand
	_______,	_______,	_______,	_______,	_______,	_______,	_______,
	_______,	_______,	_______,	_______,	_______,	_______,	_______,
	_______,	_______,	_______,	_______,	_______,	_______,	_______,
	_______,	_______,	_______,	_______,	_______,	_______,	_______,
	_______,	_______,	_______,	_______,	_______,	_______,
	_______,	_______,				_______,				_______,	_______,

    // right hand
	_______,	_______,		_______,	_______,	_______,	_______,	_______,		
	_______,	_______,		KC_P7,		KC_P8,		KC_P9,		_______,	_______,	NUMPAD,
	_______,	KC_KP_ASTERISK,	KC_P4,		KC_P5,		KC_P6,		KC_KP_PLUS,	_______,	_______,
	_______,	KC_KP_SLASH,	KC_P1,		KC_P2,		KC_P3,		KC_KP_MINUS,_______,	_______,
				_______,		KC_P0,		KC_P0,		KC_COMMA,	_______,	_______,	_______,
	KC_KP_ENTER,_______,		_______,							_______,	_______,	_______
)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QMKBEST:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("QMK is the best thing ever!");
            } else {
                // when keycode QMKBEST is released
            }
            break;
		case NUMPAD:
			if (record->event.pressed) {
				layer_invert(_NUMP);
				if(layer_state_is(_NUMP)){
					numlock = true;
				}else{
					numlock = false;
				}
				wait_us(50);
				set_backlight_PWM(pwm);
				
			}
			break;
    }
    return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
	if (index == 0) { /* First encoder */
		if (clockwise) {
			if(layer_state_is(_NUMP)){
				if(pwm <= 31){
					pwm += 1;
					set_backlight_PWM(pwm);
				}
			}else{
				tap_code(KC_MS_WH_DOWN);
			}
		}else{
			if(layer_state_is(_NUMP)){
				if(pwm >= 0){
					pwm -= 1;
					set_backlight_PWM(pwm);
				}
			}else{
				tap_code(KC_MS_WH_UP);
			}
		}
	}
	return false;
}