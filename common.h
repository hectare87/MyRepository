/* 
 * File:   common.h
 * Author: kazuki
 *
 * Created on 2016/08/08, 1:01
 */

#ifndef COMMON_H
#define	COMMON_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define AD_POTENTIO     0

int16_t grobal_time = 10;
uint8_t grobal_deg = 0;

typedef enum {PRINT_FAST = 0, PRINT_LOW = 1, PRINT_NONE = 2} print_state_t;

print_state_t p_state = PRINT_NONE;
bool p_state_changed = true;

bool enable_tsk_adcn = false;

bool ball_update = false;
bool ball_visible = true;

#define BALL_R      4
int16_t ball_x = 0;
int16_t ball_y = 0;
int8_t ball_movex = 1;
int8_t ball_movey = 1;

#endif	/* COMMON_H */

