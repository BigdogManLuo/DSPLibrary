#include"PLL.h"
#include"pid.h"
#include"SOGI.h"
#include"transform.h"

//全局变量定义
float theta=0;
float w0=2*3.14159*50;

void threePhasePLLCalc(PID*pPIDtheta,float Vq)
{
    float deltaw=PIDCalc(pPIDtheta,0,Vq,4000);
    theta=theta+deltaw+w0;
    int temp=theta*0.15916; // 数一下当前theta是2pi的几倍 注意一定要用int类型
    theta=theta-temp*(2*3.14159); //相当于对2pi求余
}

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
