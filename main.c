/*
 * File:   main.c
 * Author: student-n6
 *
 * Created on 2016/07/22, 14:16
 */

// CONFIG1L
#pragma config WDTEN = OFF, PLLDIV = 2, CFGPLLEN = ON, STVREN = OFF, XINST = OFF
// CONFIG1H
#pragma config CPUDIV = OSC1, CP0 = OFF
// CONFIG2L
#pragma config OSC = INTOSCPLL, SOSCSEL = LOW, CLKOEC = OFF, FCMEN = OFF, IESO = OFF
// CONFIG2H
#pragma config WDTPS = 1024
// CONFIG3L
#pragma config DSWDTOSC = T1OSCREF, RTCOSC = T1OSCREF, DSBOREN = OFF, DSWDTEN = OFF, DSWDTPS = G2
// CONFIG3H
#pragma config IOL1WAY = OFF, ADCSEL = BIT12, MSSP7B_EN = MSK7
// CONFIG4L
#pragma config WPFP = PAGE_127, WPCFG = OFF
// CONFIG4H
#pragma config WPDIS = OFF, WPEND = PAGE_WPFP, LS48MHZ = SYS48X8

#include <xc.h>
#include <My_PIC.h>
#include <My_Debug.h>
#include <My_Switch.h>
#include <My_USB_CDC.h>
#include <My_ILI9328.h>

#include "common.h"
#include "lcd.h"



void interrupt ISR(){
    USB_CDC_ISR();

    //*** Timer0 ISR (every 700ms)***
    if (INTCONbits.T0IF && INTCONbits.T0IE) {
        INTCONbits.T0IF = 0;
        static uint8_t t1_cnt = 0;
        LATAbits.LA0 = !PORTAbits.RA0;
        ringbuf_put_str(&usb_tx, "Hello=0\n");
    }
    //*** Timer1 ISR (every 100ms)***
    if (PIR1bits.TMR1IF && PIE1bits.TMR1IE) {
        PIR1bits.TMR1IF = 0;
        static uint8_t cnt = 0;
        TMR1 = T1_COUNTUP_100MS;
        if(++cnt == 40){
            cnt = 0;
            grobal_time++;
        }
        enable_tsk_adcn = true;
        if(ball_visible){
            ball_update = true;
            
            ball_x += ball_movex;
            ball_y += ball_movey;
            if(ball_x + BALL_R > ILI9328_MAX_WIDTH || ball_x < 0){
                ball_movex *= -1;
            }
            if(ball_y + BALL_R > ILI9328_MAX_HEIGHT || ball_y < 0){
                ball_movey *= -1;
            }
        }
    }
    //*** Switch ISR ***
    My_Switch_ISR();
}

void main(void) {
    OSC_init();
    TRISA  = 0b00010010; // x,x,x,Vcap,x,x,x,x
    TRISB  = 0b00100001; // x,x,SDI,SCK,x,x,x,x
    TRISC  = 0b00111000; // SDO1,TX,D+,D-,Vusb,LED,T1OSI,T1OSO
    ANCON0 = 0b11111101; // xxx,xxx,xxx,RA5,RA3,RA2,RA1,RA0
    ANCON1 = 0b00011111; // VBG,xxx,xxx,RB0,RC2,RB1,RB3,RB2
    INTCON2bits.RBPU = 0; // Pull-up enable
    
    timer0_init(T0_PS_1_128);     //timer0 16bit 1:128 (700ms countup)
    timer1_init(T1_PS_1_8, T1_FOSC_4);
    
    My_Switch_Init();
    USB_CDC_init();
    LCD_Init();
    ADC_init();
    
    while(1){
        INTCONbits.GIE = 0;
        USB_CDC_task();             //usb buffer check
        INTCONbits.GIE = 1;
        
        LCD_Task();
        
        if(enable_tsk_adcn){
            enable_tsk_adcn = false;
            grobal_deg = ADC8(1);
        }
    }
}

void Sw0_Pushed(void){
    debug("sw0 pushed\r\n");
//    switch(p_state){
//        case PRINT_FAST:
//            p_state = PRINT_LOW;
//            break;
//        case PRINT_LOW:
//            p_state = PRINT_FAST;
//            break;
//    }
//    p_state_changed = true;
//    for(int i=0; i<20; i++){
//        printf("sw0 push[%d]\r\n",i);
//    }
}
void Sw0_LongPushed(void){
    debug("sw0 long pushed\r\n");
//    static print_state_t before_p_state = PRINT_LOW;
//    if(p_state != PRINT_NONE){
//        before_p_state = p_state;
//        p_state = PRINT_NONE;
//    }else{
//        p_state = before_p_state;
//    }
//    p_state_changed = true;
}