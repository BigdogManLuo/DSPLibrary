#include <msp430.h>
#include "nokia_5110.h"
#include "english_68_pixel.h"
#include "write_chinese_string_pixel.h"
#include "clk.h"
#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))


/*-----------------------------------------------------------------------
LCD_init          : 5110LCD初始化
-----------------------------------------------------------------------*/
void LCD_init(void)
  {
  // 产生一个让LCD复位的低电平脉冲
  // LCD_RST = 0;
    clk_Init();
    LCD_5110_DIR |=  (0x01 << LCD_CE) + (0x01 << LCD_DC)
                    + (0x01 << LCD_DIN) + (0x01<< LCD_CLK);
    LCD_5110_DIR_RST|=(0x01 << LCD_RST);
    
    LCD_5110_OUT_RST &= ~(0x01 << LCD_RST);
    delay_us(1);

  // LCD_RST = 1;
   LCD_5110_OUT_RST |= (0x01 << LCD_RST);

        // 关闭LCD
   //LCD_CE = 0;
    LCD_5110_OUT &= ~(0x01 << LCD_CE);
    delay_us(1);
        // 使能LCD
   //LCD_CE = 1;
   LCD_5110_OUT |= (0x01 << LCD_CE);
   delay_us(1);

    LCD_write_byte(0x21, 0);    // 使用扩展命令设置LCD模式
    LCD_write_byte(0xc8, 0);    // 设置偏置电压
    LCD_write_byte(0x06, 0);    // 温度校正
    LCD_write_byte(0x13, 0);    // 1:48
    LCD_write_byte(0x20, 0);    // 使用基本命令
    LCD_clear();            // 清屏
    LCD_write_byte(0x0c, 0);    // 设定显示模式，正常显示

    // 关闭LCD
    LCD_5110_OUT &= ~(0x01 << LCD_CE);
  }

/*-----------------------------------------------------------------------
LCD_clear         : LCD清屏函数
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
LCD_set_XY        : 设置LCD坐标函数

输入参数：X       ：0－83
          Y       ：0－5
-----------------------------------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
  {
    LCD_write_byte(0x40 | Y, 0);        // column
    LCD_write_byte(0x80 | X, 0);            // row
  }

/*-----------------------------------------------------------------------
LCD_write_char    : 显示英文字符

输入参数：c       ：显示的字符；
-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
  {
    unsigned char line;

    c -= 32;

    for (line=0; line<6; line++)
    { LCD_write_byte(font6x8[c][line], 1);}
  }
/*-----------------------------------------------------------------------
LCD_write_english_String  : 英文字符串显示函数

输入参数：*s      ：英文字符串指针；
          X、Y    : 显示字符串的位置,x 0-83 ,y 0-5
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
LCD_write_chinese_string: 在LCD上显示汉字

输入参数：X、Y    ：显示汉字的起始X、Y坐标；
          ch_with ：汉字点阵的宽度
          num     ：显示汉字的个数；  
          line    ：汉字点阵数组中的起始行数
          row     ：汉字显示的行间距
测试：
    LCD_write_chi(0,0,12,7,0,0);
    LCD_write_chi(0,2,12,7,0,0);
    LCD_write_chi(0,4,12,7,0,0);    
-----------------------------------------------------------------------*/                        
void LCD_write_chinese_string(unsigned char X, unsigned char Y, 
                   unsigned char ch_with,unsigned char num,
                   unsigned char line,unsigned char row)
  {
    unsigned char i,n;

    LCD_set_XY(X,Y);                             //设置初始位置

    for (i=0;i<num;)
      {
        for (n=0; n<ch_with*2; n++)              //写一个汉字
          { 
            if (n==ch_with)                      //写汉字的下半部分
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
LCD_write_byte    : 使用SPI接口写数据到LCD

输入参数：data    ：写入的数据；
          command ：写数据/命令选择；

-----------------------------------------------------------------------*/
void LCD_write_byte(unsigned char dat, unsigned char command)
  {
    unsigned char i;
    LCD_5110_OUT &= ~(0x01 << LCD_CE); // 5110片选有效，允许输入数据

    if (command == 0)
     // PORTB &= ~LCD_DC ;         // 传送命令
    //     LCD_DC = 0;
      LCD_5110_OUT &= ~(0x01 << LCD_DC);
    else
     // PORTB |= LCD_DC ;               // 传送数据
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
   // SPDR = data;          // 传送数据到SPI寄存器

    //while ((SPSR & 0x80) == 0);         // 等待数据传送完毕

    //PORTB |= LCD_CE ;         // 关闭LCD
    // LCD_CE = 1;
     LCD_5110_OUT |= (0x01 << LCD_CE);
  }

/*************************************MY PRO***********************************************/
/*
 * 指定位置显示字符
 * 输入参数：    X   列位置
 *          Y   行位置
 *          c   显示的字符
 */
void LCD_ShowChar(unsigned char X,unsigned char Y,unsigned char c)
  {
    LCD_set_XY(X,Y);
    LCD_write_char(c);
  }
//m^n函数
unsigned int LCD_mypow(unsigned char m,unsigned char n)
{
    unsigned int result=1;
    while(n--)result*=m;
    return result;
}
//显示一个数
//x,y :起点坐标
//len :数字的位数
//num:数值(0~4294967295);
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
//显示浮点型数字
//x,y:起点坐标; num：待显示数字; n1,n2：小数点前后各要保存的位数; size：字号
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
   LCD_write_chinese_string(0,0,12,4,0,1);//“设定值”     0行0列，12/16，4个汉字，从中文字库第几行（也就是第几个字）开始，控制字间距：
   LCD_ShowFloat(50,0.7,set_volt,2,2);//展示设定值的数字
   LCD_write_chinese_string(0,2,12,4,4,1);//“实际值”
   LCD_ShowFloat(50,2.8,Vout,2,2);//展示设定值的数字
   //LCD_write_chinese_string(0,4,12,4,20,1);//“电流值”
   //LCD_ShowFloat(50,4.8,Iout,2,2);//展示设定值的数字
}

void show_warning1()//过压警告
{
    //LCD_clear();
    LCD_write_chinese_string(0,0,12,4,8,1);
}
void show_warning2()//欠压警告
{
    //LCD_clear();
    LCD_write_chinese_string(0,0,12,4,12,1);
}
void show_warning3()//过流警告
{
    //LCD_clear();
    LCD_write_chinese_string(0,0,12,4,16,1);
}
