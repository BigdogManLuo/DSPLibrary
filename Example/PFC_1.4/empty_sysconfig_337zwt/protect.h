#pragma once


#define maxVin  20
#define minVin  0

extern int inFlag;
extern int lastInFlag;

extern int protectFlag(double in,double max,double min);
extern int Switch(float V1,float V2,float Vout);

