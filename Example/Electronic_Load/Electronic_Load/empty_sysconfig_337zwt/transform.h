#ifndef _TRANSFORM_H
#define _TRANSFORM_H

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

Phase2 clarkTransformation(Phase3 );
Phase3 invClarkTransformation(Phase2 );
dq parkTransformation(Phase2,float);
Phase2 invParkTransformation(dq,float);
Phase3 linePhaseTransformation(Phase3 V);

#endif