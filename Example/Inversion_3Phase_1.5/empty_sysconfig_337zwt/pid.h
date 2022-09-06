#ifndef __pid_h_
#define __pid_h_

typedef struct PID {

	double Proportion; // �������� Proportional Const

	double Integral; // ���ֳ��� Integral Const

	double Derivative; // ΢�ֳ��� Derivative Const

	double A;//��Ҫ���ۻ���ֵ

	double LastError;

	double SumError;

	double PrevError;

	double  a0;

	double  a1;

	double  a2;
} PID;

extern PID pidId,pidIq;
extern PID pidVd,pidVq;
extern PID pidTheta;
extern double PIDCalc(PID*, double, double);
extern PID pid_Init(PID, double, double, double, double);



#endif


