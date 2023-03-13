#ifndef _PLL_H
#define _PLL_H

#include"pid.h"
#include"filter.h"

void singlePhasePLLCalc(PID* pPIDtheta,float V,ButterQueue* p,float* SOS,float scale);
void threePhasePLLCalc(PID*,float);
extern float theta;

#endif
