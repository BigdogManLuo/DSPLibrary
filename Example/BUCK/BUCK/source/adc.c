#include <msp430.h>
#include "adc.h"

void adc_Init()//24MHz ADCCLK,128�������ڣ�����Ƶ��170kHz
  {
          P6SEL|=BIT0+BIT1+BIT2;  
          ADC12CTL0&=~ADC12ENC;//ADC12ת��ʹ��
          REFCTL0 &= ~REFMSTR;//
          ADC12CTL0=ADC12SHT0_0+ADC12ON+ADC12REFON+ADC12REF2_5V+ADC12MSC;  //������������128��ADCCLK��ADC�ں˴򿪣��ڲ��ο���ѹ�򿪣��ο���ѹΪ2.5V  ɾȥADC12REF2_5��0x7ff ����3.3V��һ�룬ֻ��Ҫһ��SHI�źŵ������ش���������ʱ��
          ADC12CTL1=ADC12SHP+ADC12CONSEQ_1;//SAMPCON�ź����Բ�����ʱ������SHI�źŵ������ش���������ʱ��������ͨ�����ת��
          ADC12MCTL0 = ADC12SREF_1+ADC12INCH_0;//�ο���ѹѡ��A0��Ϊ����ͨ��
          ADC12MCTL1 = ADC12SREF_1+ADC12INCH_1;
          ADC12MCTL2 = ADC12SREF_1+ADC12INCH_2+ADC12EOS;//����ת�����ޣ���ͨ��ת��Ϊ���������һ��ת��
          ADC12IE =0X04;
          ADC12CTL0|=ADC12ENC;//ʹ��ת��

  }
void adc()
{
   // ADC12CTL0|=ADC12SC;
 // __bis_SR_register(LPM0_bits);
}
