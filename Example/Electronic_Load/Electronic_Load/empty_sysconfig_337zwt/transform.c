//32变换 dq变换的库
//最后一次修改时间：2022.7.20
//作者：BigDogManLuo  Sichuan University

#include "transform.h"
#include "math.h"


//线相变换
Phase3 linePhaseTransformation(Phase3 V)  
{
    float a, b, c;
    a = (V.a - V.c)*0.3333;
    b = (V.b - V.a)*0.3333;
    c = (V.c - V.b)*0.3333;
    struct Phase3 temp;
    temp.a = a;
    temp.b = b;
    temp.c = c;
    return temp;
}

//32变换
//输入：三相a,b,c分量
//输出：两相alpha beta分量
Phase2 clarkTransformation(Phase3 V)   
{
    float alfa, beta;
    alfa = 0.6667 * (V.a - 0.5 * V.b - 0.5 * V.c);
    beta = 0.57735 * (V.b - V.c);  
    struct Phase2 temp;
    temp.alfa = alfa;
    temp.beta = beta;
    return temp;
}

//32变换逆变换
//输入：两相alpha beta分量
//输出：三相a,b,c分量
 Phase3 invClarkTransformation(Phase2 V)   
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

//dq变换
//输入：两相alpha beta分量,alpha(a)分量的相角
//输出：d,q分量
  dq parkTransformation(Phase2 V,float theta)
  {
      float d,q;
      d=__cos(theta)*V.alfa+__sin(theta)*V.beta;
      //d=cos(theta)*V.alfa+sin(theta)*V.beta;
      q=(-__sin(theta))*V.alfa+__cos(theta)*V.beta;
      //q=(-sin(theta))*V.alfa+cos(theta)*V.beta;
      dq temp;
      temp.d=d;
      temp.q=q;
      return temp;    //0.00278
  }
//dq变换逆变换
//输入：d,q分量,alpha(a)分量的相角
//输出：两相alpha beta分量
  Phase2 invParkTransformation(dq V,float theta)
  {
      float alfa,beta;
      alfa=__cos(theta)*V.d-__sin(theta)*V.q;
      //alfa=cos(theta)*V.d-sin(theta)*V.q;
      beta=__sin(theta)*V.d+__cos(theta)*V.q;
      //beta=sin(theta)*V.d+cos(theta)*V.q;
      Phase2 temp;
      temp.alfa=alfa;
      temp.beta=beta;
      return temp;
  }


