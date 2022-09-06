//SOGI���׹����������ʵ��
//���һ���޸�ʱ�䣺2022.7.20
//���ߣ�BigDogManLuo  Sichuan University

#include "SOGI.h"

//����ʹ���ź�����90��
//���룺
//q:�����źŶ���ָ�룬���ڼ���֮ǰ������x
//x����ǰ�ź�����
//SOS:���׽ھ��󣬺���ο�https://ww2.mathworks.cn/help/signal/ug/discrete-time-system-models.html#f1-973
//  ��ͨ��matlab tfָ������ݺ��� c2d��������tusbin Z�任�õ���ɢ�Ĵ��ݺ��������ɵõ����׽ھ���SOS
//scale��SOGI�����棬ͨ������Ϊ1
float SOGICalcQ(ButterQueue* q, float x, float* SOS, float scale)
{
    float result = 0;
    float a = 0;
    InButterQueue(q, x);
    a = __divf32(1,SOS[3]);
    //a=1/SOS[3];
    result = a * ( (SOS[0] * q->data[q->rear] + SOS[1] * q->data[(q->rear + 2) % ButterSIZE] + SOS[2] * q->data[(q->rear + 1) % ButterSIZE]) - SOS[4] * q->lastResult - SOS[5] * q->prevResult);
    q->prevResult = q->lastResult;
    q->lastResult = result;

    return scale* result;
}
