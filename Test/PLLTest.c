#include"filter.h"
#include<stdio.h>
#include"pr.h"
#include<math.h>
#include"PLL.h"
#include"pid.h"
#include"transform.h"
#include"SOGI.h"

#define pi 3.14159

//初始化PID参数
PID pidTheta={0.3,150,0,0,0,0,0.0001};
//初始化队列参数
ButterQueue qVin={{0,0,0},0,0,0};
float SOS_PLL[6]={2.4286526673565359750288361606607,4.8573053347130719500576723213214,2.4286526673565359750288361606607,10000.0,-19681.059727680092485968543769559,9690.7743383495181799247575327172};
float scale_PLL=1;
//全局变量定义
Phase2 p2Vin={0,0};
dq dqVin={0,0};


int main()
{
//PID指针初始化
PID* pPIDTheta=&pidTheta;
//队列指针初始化
ButterQueue* pVin=&qVin;

//信号定义
float Vin=0;//输入电压
float Va=0,Vb=0,Vc=0; //三相工频交流电
unsigned int f=50; //工频
float Ts=0.0001;//采样周期
float Vin_beta=0;

//文件指针
FILE *p = fopen("data.txt", "w");
//打开文件
if(p == NULL)
{
    printf("open error!\n");
    return 0;
}

//锁相环测试
for (int i=0;i<=999;i++)
{
    Vin=6*sin(2*pi*f*i*Ts);
    Va=6*sin(2*pi*f*i*Ts);
    Vb=6*sin(2*pi*f*i*Ts+(2*pi/3));
    Vc=6*sin(2*pi*f*i*Ts+(4*pi/3));

    //运行三相锁相环
    /*
    Phase3 p3Vin={Va,Vb,Vc};
    p2Vin=clarkTransformation(p3Vin);
    dqVin=parkTransformation(p2Vin,theta);
    threePhasePLLCalc(pPIDTheta,dqVin.q);
    */
   
    //运行单相锁相环
    singlePhasePLLCalc(pPIDTheta,Vin,pVin,SOS_PLL,scale_PLL);

    //写文件
    /*
    //三相锁相环测试
    fprintf(p,"%f",Va);
    fprintf(p," ");
    fprintf(p,"%f",Vb);
    fprintf(p," ");
    fprintf(p,"%f",Vc);
    fprintf(p," ");
    fprintf(p,"%f",theta);
    fprintf(p,"\n");
    */
   //单相锁相环测试
   fprintf(p,"%f",Vin);
   fprintf(p," ");
   fprintf(p,"%f",theta);
   fprintf(p,"\n");
}

}