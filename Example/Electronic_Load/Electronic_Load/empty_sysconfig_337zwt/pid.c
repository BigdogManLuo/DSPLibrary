#include"pid.h"

float PIDCalc( PID* pp, float NextPoint, float set,float sumMax)//λ��ʽ
{
    float dError,Error;
    Error = set - NextPoint;       // ƫ��
    //�����޷�
    if(pp->SumError>sumMax)
    {
        pp->SumError=sumMax;
    }
    else
    {
    pp->SumError += Error*pp->Ts;
    }
    dError = pp->LastError -pp->PrevError; // ��ǰ΢��
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    return (pp->Proportion * Error   // ������
    + pp->Integral * pp->SumError    // ������
    + pp->Derivative * dError        // ΢����
    );
}

