#include<msp430.h>
#ifndef __pwm_h_
#define __pwm_h_

 extern void pwm_Init();
 extern void pwm_out(float errorD);
 extern void TIMER0_A0_Init();
 extern void TIMER1_A1_Init();

#endif
