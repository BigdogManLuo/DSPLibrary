#include"pid.h"

    //�ߴ�г��������
    PID pidI3d={0.05,50,0,0,0,0,0.0001};
    PID pidI3q={0.05,50,0,0,0,0,0.0001};
    PID pidI5d={0.05,50,0,0,0,0,0.0001};
    PID pidI5q={0.05,50,0,0,0,0,0.0001};

    //��ѹ��
    PID pidVd={0.013,1.3,0,0,0,0,0.0001};
    PID pidVq={0.013,1.3,0,0,0,0,0.0001};

    //���໷
    PID pidTheta={0.05,500,0,0,0,0,0.0001};

    //������
    PID pidId={0.1,0.1,0,0,0,0,0.0001};
    PID pidIq={0.1,0.1,0,0,0,0,0.0001};
    PID pidIL={0.01,1,0,0,0,0,0.0001};

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

float PIDCalcInc(PID*pp, float In,float Ref,float outMax) //����ʽPID
{
    float result = 0;
    float error=Ref-In;
    result= pp->Proportion*(error-pp->LastError)+pp->Integral*(error)+pp->Derivative*(error-2*pp->LastError+pp->PrevError);
    if (result > outMax)  //����޷� 
    {
        result = outMax;
    }
    return result;

}