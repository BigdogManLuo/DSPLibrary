#include"pid.h"

//�˴���������Ҫͨ��PID�������ı�����PID����    �ֱ�ΪKp,Ki,Kd,a�����ۣ�,lastError,sumError,prevError,a0,a1,a2

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

double PIDCalc( PID* pp, double NextPoint, double set )//λ��ʽ
{
    double dError,Error;
    Error = set - NextPoint;       // ƫ��
    pp->SumError += Error;
    dError = pp->LastError -pp->PrevError; // ��ǰ΢��
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    return pp->A*(pp->Proportion * Error   // ������
    + pp->Integral * pp->SumError    // ������
    + pp->Derivative * dError        // ΢����
    );
}

