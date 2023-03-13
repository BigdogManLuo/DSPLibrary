#ifndef __pid_h_
#define __pid_h_

//PID参数的结构体定义
typedef struct PID {

	float Proportion; // 比例常数 Proportional Const

	float Integral; // 积分常数 Integral Const

	float Derivative; // 微分常数 Derivative Const

	float LastError;

	float SumError;

	float PrevError;

	float Ts;   //积分周期
} PID;


//PID相关函数声明
extern float PIDCalc(PID*, float, float, float);

#endif


