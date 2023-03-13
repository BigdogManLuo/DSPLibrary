//PR比例谐振控制器的实现
//最后一次修改时间：2022.7.20
//作者：BigDogManLuo  Sichuan University

#include"pr.h"
#include"filter.h"
#include"math.h"

//PR控制器
//输入：
//q:  队列结构体指针，用于记忆输出
//x:  PR控制器当前输入
//SOS：二阶节矩阵，含义参考https://ww2.mathworks.cn/help/signal/ug/discrete-time-system-models.html#f1-973
//scale:给1就可以
//Kp Kr:比例谐振系数
//MAX,Klim,pUpLimit：通过反馈控制来实现PR控制器限幅，如果不需要调用，则屏蔽MAX相关语句，Klim=0
//使用：
//参考PRTest.c的main()函数

double PRCalc(ButterQueue* q, float x, double* SOS, float scale, float Kp, float Kr, float MAX, float Klim, float* pupLimit)
{
    float Rresult = 0;
    float result = 0;
    float Presult = 0;
    float a = 0;
    InButterQueue(q, x-Klim * (*pupLimit));

    Presult = Kp * x;
    //a = __divf32(1,SOS[3]);
    a=1/SOS[3];
    Rresult = a * (Kr * (SOS[0] * q->data[q->rear] + SOS[1] * q->data[(q->rear + 2) % ButterSIZE] + SOS[2] * q->data[(q->rear + 1) % ButterSIZE]) - SOS[4] * q->lastResult - SOS[5] * q->prevResult);
    q->prevResult = q->lastResult;
    q->lastResult = Rresult;

    result = Rresult + Presult;

    /*
    if (result >= MAX)
    {
        *pupLimit = result - MAX;
        result = MAX;
    }
    else if (result <= (-MAX))
    {
        *pupLimit = result + MAX;
        result = -MAX;
    }
    else
    {
        *pupLimit = 0;
    }
    */
    return scale * result;
}
