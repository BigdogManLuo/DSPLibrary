#include"pid.h"

//此处定义了需要通过PID控制器的变量的PID参数    分别为Kp,Ki,Kd,a（标幺）,lastError,sumError,prevError,a0,a1,a2

    PID pidId={0.05,0.005,0,0.5,0,0,0,0,0,0};
    PID pidIq={0.05,0.005,0,0.5,0,0,0,0,0,0};
    PID pidVd={0.013,0.00013,0,0.5,0,0,0,0,0,0};
    PID pidVq={0.013,0.00013,0,0.5,0,0,0,0,0,0};
    PID pidTheta={0.01,0,0,1,0,0,0,0,0,0};


PID pid_Init(PID pp,double p,double i,double d,double a)
{
    pp.Proportion=p;
    pp.Integral=i;
    pp.Derivative=d;
    pp.A=a;
    pp.LastError=0;
    pp.PrevError=0;
    pp.SumError=0;
    return pp;
}

double PIDCalc( PID* pp, double NextPoint, double set )//位置式
{
    double dError,Error;
    Error = set - NextPoint;       // 偏差
    pp->SumError += Error;
    dError = pp->LastError -pp->PrevError; // 当前微分
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    return pp->A*(pp->Proportion * Error   // 比例项
    + pp->Integral * pp->SumError    // 积分项
    + pp->Derivative * dError        // 微分项
    );
}

