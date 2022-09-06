#include<msp430.h>
#include"key.h"
#define CPU_F ((double)24000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
void key_Init()//2.7
{
    P2DIR&=~(BIT0+BIT4+BIT7);
    P2REN|=BIT0+BIT4+BIT7;
    P2OUT|=BIT0+BIT4+BIT7;
    delay_ms(15);
    P2IES&=~BIT0;
    P2IES&=~BIT7;
    P2IES&=~BIT4;
    P2IFG&=~BIT0;
    P2IFG&=~BIT7;
    P2IFG&=~BIT4;
    P2IE|=BIT0;
    P2IE|=BIT7;
    P2IE|=BIT4;
    P6DIR|=BIT4+BIT5;
    P6OUT&=~(BIT4+BIT5);
}
