#ifndef __nokia_5110_h_
#define __nokia_5110_h_
#define LCD_5110_DIR_RST P10DIR
#define LCD_5110_DIR P9DIR
#define LCD_5110_OUT P9OUT
#define LCD_5110_OUT_RST P10OUT 

#define   LCD_RST    0
#define   LCD_CE     7
#define   LCD_DC     6
#define   LCD_DIN    1
#define   LCD_CLK    3


//extern void LCD_PORT_INIT(void);
extern void LCD_init(void);
extern void LCD_clear(void);
extern void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
extern void LCD_write_chinese_string(unsigned char X, unsigned char Y,
                   unsigned char ch_with,unsigned char num,
                   unsigned char line,unsigned char row);
extern void LCD_write_char(unsigned char c);
extern void LCD_write_byte(unsigned char dat, unsigned char dc);
extern void LCD_ShowChar(unsigned char X,unsigned char Y,unsigned char c);
extern unsigned int LCD_mypow(unsigned char m,unsigned char n);
extern void LCD_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len);
extern void LCD_ShowFloat(unsigned char x, unsigned char y,float num,unsigned int n1,unsigned int n2);
extern void LCD_ShowString(unsigned char X,unsigned char Y,char *s);
extern void show_adc_result(float set_volt,float Vin);
extern void show_warning1();
extern void show_warning2();
extern void show_warning3();

#endif
