//功能:BUCK电路
//输入：10V~30V
//输出：5V 可调
//采样频率：10KHZ
//控制逻辑：定时器中断10kHZ控制采样，采样完成后在while(1)循环中执行控制
//屏幕显示：设置值和实际值
/***************分割线*******************************/
//最后一次修改日期： 2022.7.16
//修改人：傻宝


#include <msp430.h>
#include "nokia_5110.h"
#include "adc.h"
#include "pwm.h"
#include "clk.h"
#include "key.h"
#include "pid.h"
#include "filter.h"

#define CPU_F ((double)24000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))


/**********************************************************************/
//                                                                     //
//                        全局变量定义                                                                                                    //
//                                                                    //
/**********************************************************************/

/************LCD相关变量*******************************/
int onceLCD=0;//当切换到保护状态的时候清零一次LCD显示OFF
int flesh=0;//计数到一定程度刷新LCD
/**************************************************/

/************保护相关变量*****************************/
#define maxIin 2  //最大电流
#define maxVin 30 //最大输入电压
#define minVin 12 //最小输入电压
#define maxVout 10 //最大输出电压
#define minVout 5   //最小输出电压
/*************************************************/

/*************设定输出******************************/
double Vref=5; //设定的电压
double Iref=1.5;//设定输出电流
/*************************************************/


/*****************AD采样****************************/
#define K 0.185 //ACS712系数
#define _K 5.4051 //1/K的值
#define atten_Vout 52  //输出电压采样时的衰减系数
#define atten_Vin  52  //输入电压采样时的衰减系数
#define atten_hall 2
const double maxVhall =  (2.48-K*maxIin)*atten_hall;//过流的霍尔电压参考
double A0=0;
double A1=0;
double A2=0;
double Vout=0;
double Vin=0;
double Vhall=0;
double Iout=0;
/***************************************************/


/*********************************PID控制****************************************/
PID pid_Vout={0.002,2,0,0,0,0,0.0001};//输出电压的PID环  {Kp Ki Kd lastError sumError prevError Ts}
double errorD=0;//输出的占空比变量
/*******************************************************************************/


/******************************************************FILTER***********************************************************/

//需要滤波的参数初始化队列
ButterQueue qVout={{0,0,0},0,0,0};

//初始化巴特沃斯滤波参数
double SOS_Vout[6] = {1000,2000,1000,1000,-1911.1970674260732039329,914.97583480143374057291794};   // 10k巴特沃斯滤波器
double scale_Vout=0.00094469184384;

/***********************************************************************************************************************/




//******************************************************************//
//                                                                                                                                   //
//                                                                                                                                   //
//                                                                                                                                  //
//                                                                                                                                  //
//                              主函数                                                                                                             //
//                             main.c                                                                                           //
//                                                                                                                                  //
//                                                                                                                                  //
//                                                                                                                                  //
//                                                                                                                                  //
//******************************************************************//

void main(void)
 {
    //关闭看门狗
    WDTCTL = WDTPW + WDTHOLD;
    //标志位初始化
    P7DIR|=BIT3; P7OUT|=BIT3;//标志位
    //时钟初始化
    SetVcoreUp(PMMCOREV_1);  //1.55V
    SetVcoreUp(PMMCOREV_2); //1.75V
    clk_Init();
    //LCD初始化
    LCD_init();
    //按键初始化
    key_Init();
    //ADC初始化
    adc_Init();
    //定时器初始化
    TIMER0_A0_Init();
    //开启全局中断
    _EINT();
    //GPIO初始化
    P3DIR|=BIT1;
    P3OUT&=~BIT1;
    P6DIR|=BIT7;
    P6OUT&=~BIT7;
    //PID参数指针初始化
    PID *pVout=&pid_Vout;
    //滤波器队列指针初始化
    ButterQueue* pButterVout=&qVout;

    while(1)
    {
        P6OUT|=BIT5;

        //不输出的保护状态
        while(P7OUT&BIT3)//如果P7.3输出高电平，则代表不输出
        {
             if(onceLCD==1)
             {
                 LCD_clear();
             }
            errorD=0;
            pwm_out(0);
            LCD_write_english_string(1,1,"OFF");
            onceLCD=0;
        }

         //AD转换
         float Vout_temp=A0*0.0006105*atten_Vout; //如果需要滤波，则AD采样的数值可以用临时变量储存
         Vin=A1*0.0006105*atten_Vin;
         Vhall=A2*0.0006105*atten_hall;
         Iout=_K*(Vhall-2.5);

         //低通滤波
         Vout=ButterWorthFilter(pButterVout,Vout_temp,SOS_Vout,scale_Vout);

         //PID计算
         errorD=PIDCalc(pVout,Vout,Vref,10000);
         pwm_out(errorD);

         //更新显示
         if(flesh==50)
         {
             show_adc_result(Vref, Vout);
             flesh=0;
         }
         flesh++;

         P6OUT^=BIT7;
         P6OUT&=~BIT5;
    }//while(1)

}//main


//AD中断
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    P3OUT^=BIT1;
    switch(__even_in_range(ADC12IV,34))
    {
    case 0:break;
    case 2:break;
    case 4:break;
    case 6:break;
    case 8:break;
    case 10:
     A0=(double)(ADC12MEM0);
     A1=(double)(ADC12MEM1);
     A2=(double)(ADC12MEM2);

    //__bic_SR_register_on_exit(LPM0_bits); //退出低功耗模式0
    //6.0采集输入电压，6.1采集输出电压，6.2采集霍尔电压
     break;
    case 12:break;
    case 14:break;
    case 16:break;
    case 18:break;
    case 20:break;
    case 22:break;
    case 24:break;
    case 26:break;
    case 28:break;
    case 30:break;
    case 32:break;
    case 34:break;
    default:break;
    }
}


//按键中断
#pragma vector =PORT2_VECTOR
__interrupt void Port_2(void)
{
    //启动或关闭程序
    if(P2IFG&BIT7)
    {
        delay_us(2);
        while(!(P2IN&BIT7));
        P7OUT^=BIT3;
        onceLCD=1;//代表程序有输出
    }

    //参考电压＋
    if(P2IFG&BIT0)
    {
        delay_us(2);
        while(!(P2IN&BIT0));
        Vref+=0.1;
        if(Vref>=maxVout)
        {Vref = maxVout;}
    }

    //参考电压-
    if(P2IFG&BIT4)
    {
        delay_us(2);
        while(!(P2IN&BIT4));
        Vref-=0.1;
        if(Vref<=minVout)
        {
            Vref=minVout;
        }
    }

   P2IFG &=~(BIT0+BIT4+BIT7);
}


//定时器中断 目前用来打开AD中断
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Calculate(void)
{
   ADC12CTL0|=ADC12SC;
}

