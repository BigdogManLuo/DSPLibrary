#include"pid.h"

float PIDCalc( PID* pp, float NextPoint, float set,float sumMax)//位置式
{
    float dError,Error;
    Error = set - NextPoint;       // 偏差
    //积分限幅
    if(pp->SumError>sumMax)
    {
        pp->SumError=sumMax;
    }
    else
    {
    pp->SumError += Error*pp->Ts;
    }
    dError = pp->LastError -pp->PrevError; // 当前微分
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    return (pp->Proportion * Error   // 比例项
    + pp->Integral * pp->SumError    // 积分项
    + pp->Derivative * dError        // 微分项
    );
}

