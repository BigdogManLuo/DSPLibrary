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

extern  Phase2 clarkTransformation(Phase3 );
extern  Phase3 invClarkTransformation(Phase2 );
extern  dq parkTransformation(Phase2,float);
extern  Phase2 invParkTransformation(dq,float);
extern Phase3 linePhaseTransformation(Phase3 V);
