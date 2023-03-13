#include<msp430.h>
#include"pwm.h"
#define F 50000
#define T  600 //  (1/2)*(24000000/F)=240
void pwm_Init()//
{
    TA0CTL=TASSEL_2+MC_3+TACLR;
    TA0CCR0=T;
    TA0CCTL4=OUTMOD_2;
    TA0CCTL0=OUTMOD_4;
    P8DIR|=BIT0;
    P8SEL|=BIT0;
    P8DIR|=BIT4;
    P8SEL|=BIT4;
}

void pwm_out(float duty)//配置两路pwm波，输出相反，运用pid算法调整占空比
{
   TA0CCR4 = (int)(duty*T);
   TA0CCR3 = (int)(duty*T);
}

void TIMER0_A0_Init()//8.4输出pwm
{
    TA0CTL = TASSEL_2+ MC_3 +TACLR;//SMCLK,增减计数模式,清楚TAR
    TA0CCR0 = T;
    TA0CCTL0|=CCIE;
    TA0CCTL4=OUTMOD_2;//P8.4输出PWM波
    TA0CCTL3=OUTMOD_6;//P8.4输出PWM波
    P8DIR|=BIT4;
    P8SEL|=BIT4;
    P8DIR|=BIT3;
    P8SEL|=BIT3;
}
void TIMER1_A1_Init()//8.6输出pwm
{
    TA1CTL = TASSEL_2+ MC_3 +TACLR;
    TA1CCR0 = 12000; //开关频率为50HZ
    TA1CCTL1 = OUTMOD_2;//取反/复位模式
    TA1CCTL1=CCIE;
}

