#pragma once
#include"pid.h"
#include"filter.h"

extern void singlePhasePLLCalc(PID* pPIDtheta,float V,ButterQueue* p,float* SOS,float scale);
extern void threePhasePLLCalc(PID*,float);
extern float theta;