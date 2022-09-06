#include "transform.h"
#include "math.h"

float f = 50;
int k = 0;
float Ts=0.0001;

Phase3 I_3Phase={0,0,0};
Phase3 V_3Phase={0,0,0};
Phase2 V_2Phase={0,0};
Phase2 I_2Phase={0,0};
Phase2 p2refVolt={0,0};
Phase2 p2refCurrent={0,0};
Phase3 p3refVolt={0,0,0};
Phase3 p3refCurrent={0,0,0};
Phase2 dutyRate_2Phase={0,0};
Phase3 dutyRate_3Phase={0,0,0};
dq V_dq={0,0};
dq I_dq={0,0};
dq dutyRate_dq={0,0};

Phase2 clarkTransformation(Phase3 V)   //3/2变换
{
    float alfa, beta;
    alfa = 0.6667 * (V.a - 0.5 * V.b - 0.5 * V.c);
    beta = 0.57735 * (V.b - V.c);  //0.57735= 三分之根号三
    struct Phase2 temp;
    temp.alfa = alfa;
    temp.beta = beta;
    return temp;
}

 Phase3 invClarkTransformation(Phase2 V)   //2/3变换
{
    float a, b, c;
    a = V.alfa;
    b = (-0.5) * V.alfa + 0.866 * V.beta;
    c = (-0.5) * V.alfa - 0.866 * V.beta;
    struct Phase3 temp;
    temp.a = a;
    temp.b = b;
    temp.c = c;
    return temp;
}

  dq parkTransformation(Phase2 V,int k,float w)
  {
      float d,q;
      d=cosf(w*k*Ts)*V.alfa+sinf(w*k*Ts)*V.beta;
      q=(-sinf(w*k*Ts))*V.alfa+cosf(w*k*Ts)*V.beta;
      dq temp;
      temp.d=d;
      temp.q=q;
      return temp;    //0.00278
  }

  Phase2 invParkTransformation(dq V,int k,float w)
  {
      float alfa,beta;
      alfa=cosf(w*k*Ts)*V.d-sinf(w*k*Ts)*V.q;
      beta=sinf(w*k*Ts)*V.d+cosf(w*k*Ts)*V.q;
      Phase2 temp;
      temp.alfa=alfa;
      temp.beta=beta;
      return temp;
  }
