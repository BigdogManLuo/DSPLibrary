#pragma once

typedef struct Phase3
{
    double a;
    double b;
    double c;
} Phase3;


typedef struct Phase2
{
    double alfa;
    double beta;
} Phase2;

typedef struct dq
{
    double d;
    double q;
}dq;

extern Phase3 I_3Phase;
extern Phase3 V_3Phase;
extern Phase2 I_2Phase;
extern Phase2 V_2Phase;
extern Phase2 p2refCurrent;
extern Phase2 p2refVolt;
extern Phase3 p3refCurrent;
extern Phase3 p3refVolt;
extern Phase2 dutyRate_2Phase;
extern Phase3 dutyRate_3Phase;
extern dq I_dq;
extern dq V_dq;
extern dq dutyRate_dq;

extern  Phase2 clarkTransformation(Phase3 );
extern  Phase3 invClarkTransformation(Phase2 );
extern  dq parkTransformation(Phase2,int,float);
extern  Phase2 invParkTransformation(dq,int,float);
