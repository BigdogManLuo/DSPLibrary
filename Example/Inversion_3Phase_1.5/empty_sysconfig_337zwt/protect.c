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
