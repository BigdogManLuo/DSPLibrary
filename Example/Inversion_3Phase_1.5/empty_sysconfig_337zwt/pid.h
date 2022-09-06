#ifndef __pid_h_
#define __pid_h_

typedef struct PID {

	double Proportion; // 比例常数 Proportional Const

	double Integral; // 积分常数 Integral Const

	double Derivative; // 微分常数 Derivative Const

	double A;//需要标幺化的值

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


