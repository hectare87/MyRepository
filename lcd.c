#include "lcd.h"
#include <My_ILI9328.h>
#include <My_PIC.h>
#include <My_USB_CDC.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

void LCD_Init(void){
    ILI9328_Init();
    delay_ms(100);
    LCD_TSK_STATE = STATE1;
}

void LCD_Clear(void){
    ILI9328_Clear();
}

void LCD_Task(void){
    
    switch(LCD_TSK_STATE){
        case IDLE:
            return;
        case STATE1:
            ILI9328_Clear();
            LCD_Disp_String();
            ILI9328_DrawTextX("UART START", 20, 20);
            
            LCD_TSK_STATE = CLOCK;
            break;
        case STATE2:
            ILI9328_Clear();
            LCD_Disp_GraphArea();
            LCD_Graph_SinWave();
            
            break;
        case CLOCK:
        {
            static uint16_t disp_time = 999;
            static uint8_t disp_deg = 0;
//            if(disp_time != grobal_time){
//                disp_time = grobal_time;
//                int hour = grobal_time / 3600;
//                int min  = grobal_time / 60 % 60;
//                int sec  = grobal_time % 60;
//                char str[20];
//                sprintf(str, "%02d:%02d:%02d", hour, min, sec);
//                ILI9328_DrawTextX(str, 20, 40);
//            }
            if(disp_deg < grobal_deg - 1
                    || disp_deg > grobal_deg + 1){
//                char str[30];
//                sprintf(str, "deg=%5u,%5u", disp_deg, grobal_deg);
//                ILI9328_DrawTextX(str, 20, 60);
                disp_deg = grobal_deg;
                
                int xpos = disp_deg / 255.0f * 210;
                static int _xpos = 0;
                ILI9328_DrawRectangle(_xpos, 80, 30, 6, COLOR_BLACK);
                ILI9328_DrawRectangle(xpos, 80, 30, 6, COLOR_CYAN);
                _xpos = xpos;
            }
            if(ball_update){
                static uint16_t _ball_x = 0;
                static uint16_t _ball_y = 0;
                
                ball_update = false;
                ILI9328_DrawRectangle(_ball_x, _ball_y, BALL_R, BALL_R, COLOR_BLACK);
                ILI9328_DrawRectangle(ball_x, ball_y, BALL_R, BALL_R, COLOR_YELLOW);
                _ball_x = ball_x;
                _ball_y = ball_y;
            }
            break;
        }
    }
}


void LCD_Disp_String(void){
    const char *str1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *str2 = "debug out";
//    ILI9328_DrawText(str1);
    for(int i=0x20; i<0x20+192; i++){
        ILI9328_DrawChar(i);
    }
    ILI9328_DrawText("\nI'm a Perfect Human.\n");
}

void LCD_Disp_GraphArea(void){
    ILI9328_DrawLine(0,229,10,10,COLOR_YELLOW,1);
    ILI9328_DrawLine(0,229,11,11,COLOR_WHITE,1);
    ILI9328_DrawLine(229,229,10,319,COLOR_YELLOW,1);
    ILI9328_DrawLine(230,230,10,319,COLOR_WHITE,1);
}

void LCD_Graph_SinWave(void){
    for(int b=0; b<10; b++)
    for(int i=12; i<320; i++){
        ILI9328_DrawPixel(sin(i/100.0*2*3.14)*100 + 100 + b,i,COLOR_CYAN);
    }
}

void LCD_Disp_Point(int x, int y){
    ILI9328_DrawPixel(x,y,COLOR_RED);
}