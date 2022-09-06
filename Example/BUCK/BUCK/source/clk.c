/*void clk_Init()//8Mhz鍐呴儴refoclk
{
    UCSCTL3=SELREF_2;
    UCSCTL4=SELA_3+SELM_3+SELS_3;//杩欓噷鍙互杩涜浼樺寲锛屽叧闂笉闇�瑕佺殑鏃堕挓
    UCSCTL0=0x0000;
    __bis_SR_register(SCG0);
    UCSCTL1=DCORSEL_5;
    UCSCTL2=243;
    __bic_SR_register(SCG0);
    __delay_cycles(250000);
}*/
#include<msp430.h>
#include"clk.h"
void clk_Init()//24Mhz SMCLK and MCLK锛�32768Hz ACLK
{
  P5SEL|=BIT2+BIT3;//Port select XT2
  UCSCTL6&=~XT2OFF;
  __bis_SR_register(SCG0); 
   __delay_cycles(250000);   
  UCSCTL3=SELREF_5+FLLREFDIV_1;
  UCSCTL6&=~XT2DRIVE0;
  UCSCTL1=DCORSEL_6;
  UCSCTL2=2+FLLD_1;
  UCSCTL0=0;
  UCSCTL4=SELA_2+SELS_3+SELM_3; 
  
   __bic_SR_register(SCG0); 
    
    __delay_cycles(250000);  
    do
  {
    UCSCTL7&=~(XT2OFFG+XT1LFOFFG+DCOFFG);
    SFRIFG1&=~OFIFG;
  }while(SFRIFG1&OFIFG);
}

void SetVcoreUp (unsigned int level)
{
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}
