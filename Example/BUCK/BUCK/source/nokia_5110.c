#include <msp430.h>
#include "nokia_5110.h"
#include "english_68_pixel.h"
#include "write_chinese_string_pixel.h"
#include "clk.h"
#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))


/*-----------------------------------------------------------------------
LCD_init          : 5110LCD��ʼ��
-----------------------------------------------------------------------*/
void LCD_init(void)
  {
  // ����һ����LCD��λ�ĵ͵�ƽ����
  // LCD_RST = 0;
    clk_Init();
    LCD_5110_DIR |=  (0x01 << LCD_CE) + (0x01 << LCD_DC)
                    + (0x01 << LCD_DIN) + (0x01<< LCD_CLK);
    LCD_5110_DIR_RST|=(0x01 << LCD_RST);
    
    LCD_5110_OUT_RST &= ~(0x01 << LCD_RST);
    delay_us(1);

  // LCD_RST = 1;
   LCD_5110_OUT_RST |= (0x01 << LCD_RST);

        // �ر�LCD
   //LCD_CE = 0;
    LCD_5110_OUT &= ~(0x01 << LCD_CE);
    delay_us(1);
        // ʹ��LCD
   //LCD_CE = 1;
   LCD_5110_OUT |= (0x01 << LCD_CE);
   delay_us(1);

    LCD_write_byte(0x21, 0);    // ʹ����չ��������LCDģʽ
    LCD_write_byte(0xc8, 0);    // ����ƫ�õ�ѹ
    LCD_write_byte(0x06, 0);    // �¶�У��
    LCD_write_byte(0x13, 0);    // 1:48
    LCD_write_byte(0x20, 0);    // ʹ�û�������
    LCD_clear();            // ����
    LCD_write_byte(0x0c, 0);    // �趨��ʾģʽ��������ʾ

    // �ر�LCD
    LCD_5110_OUT &= ~(0x01 << LCD_CE);
  }

/*-----------------------------------------------------------------------
LCD_clear         : LCD��������
-----------------------------------------------------------------------*/
void LCD_clear(void)
  {
    unsigned int i;

    LCD_write_byte(0x0c, 0);            
    LCD_write_byte(0x80, 0);            

    for (i=0; i<504; i++)
      LCD_write_byte(0, 1);         
  }

/*-----------------------------------------------------------------------
LCD_set_XY        : ����LCD���꺯��

���������X       ��0��83
          Y       ��0��5
-----------------------------------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
  {
    LCD_write_byte(0x40 | Y, 0);        // column
    LCD_write_byte(0x80 | X, 0);            // row
  }

/*-----------------------------------------------------------------------
LCD_write_char    : ��ʾӢ���ַ�

���������c       ����ʾ���ַ���
-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
  {
    unsigned char line;

    c -= 32;

    for (line=0; line<6; line++)
    { LCD_write_byte(font6x8[c][line], 1);}
  }
/*-----------------------------------------------------------------------
LCD_write_english_String  : Ӣ���ַ�����ʾ����

���������*s      ��Ӣ���ַ���ָ�룻
          X��Y    : ��ʾ�ַ�����λ��,x 0-83 ,y 0-5
-----------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
  {
    LCD_set_XY(X,Y);
    while (*s) 
      {
     LCD_write_char(*s);
     s++;
      }
  }
/*-----------------------------------------------------------------------
LCD_write_chinese_string: ��LCD����ʾ����

���������X��Y    ����ʾ���ֵ���ʼX��Y���ꣻ
          ch_with �����ֵ���Ŀ��
          num     ����ʾ���ֵĸ�����  
          line    �����ֵ��������е���ʼ����
          row     ��������ʾ���м��
���ԣ�
    LCD_write_chi(0,0,12,7,0,0);
    LCD_write_chi(0,2,12,7,0,0);
    LCD_write_chi(0,4,12,7,0,0);    
-----------------------------------------------------------------------*/                        
void LCD_write_chinese_string(unsigned char X, unsigned char Y, 
                   unsigned char ch_with,unsigned char num,
                   unsigned char line,unsigned char row)
  {
    unsigned char i,n;

    LCD_set_XY(X,Y);                             //���ó�ʼλ��

    for (i=0;i<num;)
      {
        for (n=0; n<ch_with*2; n++)              //дһ������
          { 
            if (n==ch_with)                      //д���ֵ��°벿��
              {
                if (i==0) LCD_set_XY(X,Y+1);
                else
                   LCD_set_XY((X+(ch_with+row)*i),Y+1);
              }
            LCD_write_byte(write_chinese[line+i][n],1);
          }
        i++;
        LCD_set_XY((X+(ch_with+row)*i),Y);
      }
  }
/*-----------------------------------------------------------------------
LCD_write_byte    : ʹ��SPI�ӿ�д���ݵ�LCD

���������data    ��д������ݣ�
          command ��д����/����ѡ��

-----------------------------------------------------------------------*/
void LCD_write_byte(unsigned char dat, unsigned char command)
  {
    unsigned char i;
    LCD_5110_OUT &= ~(0x01 << LCD_CE); // 5110Ƭѡ��Ч��������������

    if (command == 0)
     // PORTB &= ~LCD_DC ;         // ��������
    //     LCD_DC = 0;
      LCD_5110_OUT &= ~(0x01 << LCD_DC);
    else
     // PORTB |= LCD_DC ;               // ��������
    // LCD_DC = 1;
     LCD_5110_OUT |= (0x01 << LCD_DC);
        for(i=0;i<8;i++)
        {
            if(dat&0x80)
                //SDIN = 1;
               LCD_5110_OUT |= (0x01 << LCD_DIN);
            else
                //SDIN = 0;
               LCD_5110_OUT &= ~(0x01 << LCD_DIN);
            //SCLK = 0;
            LCD_5110_OUT &= ~(0x01 << LCD_CLK);
            //P11OUT&=~BIT1;
            dat = dat << 1;
            //SCLK = 1;
            LCD_5110_OUT |= (0x01 << LCD_CLK);
           // P11OUT|=BIT1;
        }
   // SPDR = data;          // �������ݵ�SPI�Ĵ���

    //while ((SPSR & 0x80) == 0);         // �ȴ����ݴ������

    //PORTB |= LCD_CE ;         // �ر�LCD
    // LCD_CE = 1;
     LCD_5110_OUT |= (0x01 << LCD_CE);
  }

/*************************************MY PRO***********************************************/
/*
 * ָ��λ����ʾ�ַ�
 * ���������    X   ��λ��
 *          Y   ��λ��
 *          c   ��ʾ���ַ�
 */
void LCD_ShowChar(unsigned char X,unsigned char Y,unsigned char c)
  {
    LCD_set_XY(X,Y);
    LCD_write_char(c);
  }
//m^n����
unsigned int LCD_mypow(unsigned char m,unsigned char n)
{
    unsigned int result=1;
    while(n--)result*=m;
    return result;
}
//��ʾһ����
//x,y :�������
//len :���ֵ�λ��
//num:��ֵ(0~4294967295);
void LCD_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len)
{
    unsigned char t,temp;
    unsigned char enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/LCD_mypow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                LCD_ShowChar(x+6*t,y,'0');
                continue;
            }else enshow=1;
        }
        LCD_ShowChar(x+6*t,y,temp+'0');
    }
}
//��ʾ����������
//x,y:�������; num������ʾ����; n1,n2��С����ǰ���Ҫ�����λ��; size���ֺ�
void LCD_ShowFloat(unsigned char x, unsigned char y,float num,unsigned int n1,unsigned int n2)
{
    int num0,num1,i,p=1;
    num0=num;
    for(i=0;i<n2;i++)   p=p*10;
    num1=(int)(num*p)%p;
    LCD_ShowNum(x,y,num0,n1);
    LCD_ShowChar(x+n1*6+1,y,'.');
    LCD_ShowNum(x+(n1+1)*6,y,num1,n2);
}

void LCD_ShowString(unsigned char X,unsigned char Y,char *s)
  {
    LCD_set_XY(X,Y);
    while (*s)
      {
     LCD_write_char(*s);
     s++;
      }
  }
void show_adc_result(float set_volt,float Vout)
{     
   LCD_write_chinese_string(0,0,12,4,0,1);//���趨ֵ��     0��0�У�12/16��4�����֣��������ֿ�ڼ��У�Ҳ���ǵڼ����֣���ʼ�������ּ�ࣺ
   LCD_ShowFloat(50,0.7,set_volt,2,2);//չʾ�趨ֵ������
   LCD_write_chinese_string(0,2,12,4,4,1);//��ʵ��ֵ��
   LCD_ShowFloat(50,2.8,Vout,2,2);//չʾ�趨ֵ������
   //LCD_write_chinese_string(0,4,12,4,20,1);//������ֵ��
   //LCD_ShowFloat(50,4.8,Iout,2,2);//չʾ�趨ֵ������
}

void show_warning1()//��ѹ����
{
    //LCD_clear();
    LCD_write_chinese_string(0,0,12,4,8,1);
}
void show_warning2()//Ƿѹ����
{
    //LCD_clear();
    LCD_write_chinese_string(0,0,12,4,12,1);
}
void show_warning3()//��������
{
    //LCD_clear();
    LCD_write_chinese_string(0,0,12,4,16,1);
}
