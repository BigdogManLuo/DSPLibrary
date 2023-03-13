#ifndef __nokia_5110_h_
#define __nokia_5110_h_
#define uint unsigned int
#define uchar unsigned char


//extern void LCD_PORT_INIT(void);
extern void LCD_init(void);
extern void LCD_clear(void);
extern void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
extern void LCD_write_chinese_string(unsigned char X, unsigned char Y,
                   unsigned char ch_with,unsigned char num,
                   unsigned char line,unsigned char row);
extern void LCD_write_char(unsigned char c);
extern void LCD_write_byte(unsigned int dat, unsigned char command);
extern void LCD_ShowChar(unsigned char X,unsigned char Y,unsigned char c);
extern unsigned int LCD_mypow(unsigned char m,unsigned char n);
extern void LCD_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len);
extern void LCD_ShowFloat(unsigned char x, unsigned char y,float num,unsigned int n1,unsigned int n2);
extern void LCD_ShowSetFloat(unsigned char x, unsigned char y,float num,unsigned int n1,unsigned int n2);
extern void LCD_ShowString(unsigned char X,unsigned char Y,char *s);
extern void show_adc_result(float set_volt,float Vin);
void show_statusofLoad(unsigned int index,float PF,int);
extern void show_warning1();
extern void show_warning2();
extern void show_warning3();
extern void LCD_GPIO_Init();
extern void board_init();
extern void pinMux_init();
extern void SPI_init();
#endif