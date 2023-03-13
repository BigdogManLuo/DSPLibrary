#ifndef _SOGI_H
#define _SOGI_H
#include"filter.h"

float SOGICalcQ(ButterQueue* q, float x, float* SOS, float scale);

#endif
