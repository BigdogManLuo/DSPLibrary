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
extern Phase2 Vs_2Phase;
extern Phase2 p2refCurrent;
extern Phase2 p2refVolt;
extern Phase2 W_2Phase;
extern Phase3 W_3Phase;
extern Phase3 p3refCurrent;
extern Phase3 p3refVolt;
extern Phase2 dutyRate_2Phase;
extern Phase3 dutyRate_3Phase;
extern Phase2 IL_2Phase;
extern dq I_dq;
extern dq V_dq;
extern dq W_dq;
extern dq dutyRate_dq;
extern Phase3 IL_3Phase;
extern Phase3 Vs_3Phase;
extern dq IL_dq;
extern dq Vs_dq;
extern  Phase2 clarkTransformation(Phase3 );
extern  Phase3 invClarkTransformation(Phase2 );
extern  dq parkTransformation(Phase2,float);
extern  Phase2 invParkTransformation(dq,float);

