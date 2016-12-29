/* 
 * File:   lcd.h
 * Author: kazuki
 *
 * Created on 2016/08/07, 12:57
 */

#ifndef LCD_H
#define	LCD_H

#include <xc.h>

static enum {IDLE,STATE1,STATE2,CLOCK} LCD_TSK_STATE = IDLE;

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Task(void);

void LCD_Disp_String(void);
void LCD_Disp_GraphArea(void);
void LCD_Graph_SinWave(void);

void LCD_Disp_Point(int x, int y);
#endif	/* LCD_H */

