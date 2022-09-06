#ifndef __pid_h_
#define __pid_h_

typedef struct PID {

	float Proportion; // 比例常数 Proportional Const

	float Integral; // 积分常数 Integral Const

	float Derivative; // 微分常数 Derivative Const

	float LastError;

	float SumError;

	float PrevError;

	float Ts;   //积分周期
} PID;


extern PID pidI3d,pidI3q,pidI5d,pidI5q;
extern PID pidVd,pidVq,pidIL,pidId,pidIq;
extern PID pidTheta;
extern float PIDCalc(PID*, float, float, float);
extern PID pid_Init(PID, float, float, float, float);
float PIDCalcInc(PID*pp, float, float,float);


#endif


