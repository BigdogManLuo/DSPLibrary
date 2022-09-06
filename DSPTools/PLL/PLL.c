#include"PLL.h"
#include"pid.h"
#include"SOGI.h"
#include"transform.h"

//全局变量定义
float theta=0;
float w0=2*3.14159*50;

//三相锁相环
//输入：锁相环pid参数指针，输入电压q轴分量
//无返回值，直接修改全局变量theta，即相角
//使用方法：参考PLLTest.c
void threePhasePLLCalc(PID*pPIDtheta,float Vq)
{
    float deltaw=PIDCalc(pPIDtheta,0,Vq,4000);
    theta=theta+deltaw+w0;
    int temp=theta*0.15916; // 数一下当前theta是2pi的几倍 注意一定要用int类型
    theta=theta-temp*(2*3.14159); //相当于对2pi求余
}

//单相锁相环
//输入：锁相环pid参数指针，输入电压，SOGI 队列指针,SOGI的SOS(二阶节)参数，SOGI的增益（通常设定为1）
//无返回值，直接修改全局变量theta，即相角
//使用方法：参考PLLTest.c
void singlePhasePLLCalc(PID* pPIDtheta,float V,ButterQueue* p,float* SOS,float scale)
{
    float V_beta=SOGICalcQ(p,V,SOS,scale);
    float V_alpha=V;
    Phase2 p2V={V_alpha,V_beta};
    dq dqV=parkTransformation(p2V,theta);
    float deltaw=PIDCalc(pPIDtheta,0,dqV.q,4000);
    theta=theta+deltaw+w0;
    int temp=theta*0.15916; // 数一下当前theta是2pi的几倍 注意一定要用int类型
    theta=theta-temp*(2*3.14159); //相当于对2pi求余
}