//SOGI二阶广义积分器的实现
//最后一次修改时间：2022.7.20
//作者：BigDogManLuo  Sichuan University

#include "SOGI.h"

//用于使得信号移向90°
//输入：
//q:输入信号队列指针，用于记忆之前的输入x
//x：当前信号输入
//SOS:二阶节矩阵，含义参考https://ww2.mathworks.cn/help/signal/ug/discrete-time-system-models.html#f1-973
//  可通过matlab tf指令创建传递函数 c2d函数进行tusbin Z变换得到离散的传递函数，即可得到二阶节矩阵SOS
//scale：SOGI的增益，通常设置为1
float SOGICalcQ(ButterQueue* q, float x, float* SOS, float scale)
{
    float result = 0;
    float a = 0;
    InButterQueue(q, x);
    a = __divf32(1,SOS[3]);
    //a=1/SOS[3];
    result = a * ( (SOS[0] * q->data[q->rear] + SOS[1] * q->data[(q->rear + 2) % ButterSIZE] + SOS[2] * q->data[(q->rear + 1) % ButterSIZE]) - SOS[4] * q->lastResult - SOS[5] * q->prevResult);
    q->prevResult = q->lastResult;
    q->lastResult = result;

    return scale* result;
}
