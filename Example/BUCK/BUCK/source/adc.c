#include <msp430.h>
#include "adc.h"

void adc_Init()//24MHz ADCCLK,128采样周期，采样频率170kHz
  {
          P6SEL|=BIT0+BIT1+BIT2;  
          ADC12CTL0&=~ADC12ENC;//ADC12转换使能
          REFCTL0 &= ~REFMSTR;//
          ADC12CTL0=ADC12SHT0_0+ADC12ON+ADC12REFON+ADC12REF2_5V+ADC12MSC;  //采样周期所需128个ADCCLK，ADC内核打开，内部参考电压打开，参考电压为2.5V  删去ADC12REF2_5，0x7ff 就是3.3V的一半，只需要一个SHI信号的上升沿触发采样定时器
          ADC12CTL1=ADC12SHP+ADC12CONSEQ_1;//SAMPCON信号来自采样定时器，由SHI信号的上升沿触发采样定时器；序列通道多次转换
          ADC12MCTL0 = ADC12SREF_1+ADC12INCH_0;//参考电压选择，A0作为输入通道
          ADC12MCTL1 = ADC12SREF_1+ADC12INCH_1;
          ADC12MCTL2 = ADC12SREF_1+ADC12INCH_2+ADC12EOS;//序列转换借宿，本通道转换为该序列最后一次转换
          ADC12IE =0X04;
          ADC12CTL0|=ADC12ENC;//使能转换

  }
void adc()
{
   // ADC12CTL0|=ADC12SC;
 // __bis_SR_register(LPM0_bits);
}
