#include"filter.h"
#include<stdio.h>
#include"add.h"
#include"pr.h"
#include<math.h>

#define pi 3.14159
//初始化PR控制器参数
ButterQueue qVout={{0,0,0},0,0,0};
double SOS_PR[6]={1.22487525234486,0,-1.22487525234486,1000,-1901.34877213705,997.55024949531};  
//double SOS_PR[6]={1.25616925921,0,-1.25616925921,10000,-19987.62173119,9997.48766148};//10k
double scale_PR=1;
float upLimit=0;
/*
int main()
{
    printf("hello,world!\n");
    //system("pause"); 
    
    //PR控制器队列指针初始化
    ButterQueue* pVout=&qVout;
    float* pUpLimit=&upLimit;
    //信号定义
    float Vin=40;
    float Vref=0;
    float f=50;
    float Ts=0.001;
    float Vout=0;
    float error=0;
    float dutyRate=0;
    //文件指针
     FILE *p = fopen("data.txt", "w");
    //打开文件
    if(p == NULL)
    {
        printf("open error!\n");
        return 0;
    }

    //测试PR控制器
    for(int i=0;i<=1500;i++)
    {
        //参考变化
        Vref=10*sin(2*pi*f*i*Ts);
        
        //PR控制
        error=(Vref-Vout);
        dutyRate=PRCalc(pVout,error,SOS_PR,scale_PR,0.01,2,0, 0, pUpLimit);

        //同步BUCK数学模型
        Vout=dutyRate*Vin;

        //写文件
        fprintf(p,"%f",Vref);
        fprintf(p," ");
        fprintf(p,"%f",Vout);
        fprintf(p,"\n");
    }
    
}
*/
