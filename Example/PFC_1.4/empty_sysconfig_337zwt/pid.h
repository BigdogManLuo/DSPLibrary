#ifndef __pid_h_
#define __pid_h_

typedef struct PID {

	float Proportion; // �������� Proportional Const

	float Integral; // ���ֳ��� Integral Const

	float Derivative; // ΢�ֳ��� Derivative Const

	float LastError;

	float SumError;

	float PrevError;

	float Ts;   //��������
} PID;


extern PID pidI3d,pidI3q,pidI5d,pidI5q;
extern PID pidVd,pidVq,pidIL,pidId,pidIq;
extern PID pidTheta;
extern float PIDCalc(PID*, float, float, float);
extern PID pid_Init(PID, float, float, float, float);
float PIDCalcInc(PID*pp, float, float,float);


#endif


