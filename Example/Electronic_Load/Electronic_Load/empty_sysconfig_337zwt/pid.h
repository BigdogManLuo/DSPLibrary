#ifndef __pid_h_
#define __pid_h_

//PID�����Ľṹ�嶨��
typedef struct PID {

    float Proportion; // �������� Proportional Const

    float Integral; // ���ֳ��� Integral Const

    float Derivative; // ΢�ֳ��� Derivative Const

    float LastError;

    float SumError;

    float PrevError;

    float Ts;   //��������
} PID;


//PID��غ�������
float PIDCalc(PID*, float, float, float);

#endif


