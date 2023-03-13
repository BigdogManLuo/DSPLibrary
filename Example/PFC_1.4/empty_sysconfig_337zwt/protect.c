#include "protect.h"

int inFlag = 0;
int lastInFlag = 0;

int protectFlag(double IN, double max, double min)
{
    if (IN> max)
    {
        return 1;
    }
    else if (IN<min)
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

int Switch(float V1,float V2,float Vout)
{
    if(Vout>=0&&Vout<V1)//  0<Vout<V1
    {
        return 1;
    }
    else if(Vout>=V1&&Vout<V2)//V1<Vout<V2
    {
        return 2;
    }
    else if(Vout>=V2)//Vout>V2
    {
        return 3;
    }
    else
    {
       return 0;
    }

}
