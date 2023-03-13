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

void pwm_out(float duty)//������·pwm��������෴������pid�㷨����ռ�ձ�
{
   TA0CCR4 = (int)(duty*T);
   TA0CCR3 = (int)(duty*T);
}

void TIMER0_A0_Init()//8.4���pwm
{
    TA0CTL = TASSEL_2+ MC_3 +TACLR;//SMCLK,��������ģʽ,���TAR
    TA0CCR0 = T;
    TA0CCTL0|=CCIE;
    TA0CCTL4=OUTMOD_2;//P8.4���PWM��
    TA0CCTL3=OUTMOD_6;//P8.4���PWM��
    P8DIR|=BIT4;
    P8SEL|=BIT4;
    P8DIR|=BIT3;
    P8SEL|=BIT3;
}
void TIMER1_A1_Init()//8.6���pwm
{
    TA1CTL = TASSEL_2+ MC_3 +TACLR;
    TA1CCR0 = 12000; //����Ƶ��Ϊ50HZ
    TA1CCTL1 = OUTMOD_2;//ȡ��/��λģʽ
    TA1CCTL1=CCIE;
}

