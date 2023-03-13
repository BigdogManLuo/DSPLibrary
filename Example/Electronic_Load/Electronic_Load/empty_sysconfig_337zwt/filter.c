#include"filter.h"

// �����ﶨ����Ҫ�˲��ı�����ʼ����








//�⺯������

void InitQueue(Queue* q)
{
    int i=0;
    q->front = 0;
    q->rear = -1;
    q->flag_full = 0;
    for(i=0;i<=QueueSIZE-1;i++)
    {
        q->data[i] =0;
    }
    q->frontData = 0;
    q->sumData = 0;
}

// ���
void InQueue(Queue*q, float x)
{
    q->rear = (q->rear + 1) % QueueSIZE;    //��βָ�����
    q->data[(q->rear)] = x;     //��x�����β

}

// ����
float OutQueue(Queue* q)
{   
    float frontNum = 0;
    frontNum = q->data[q->front];
    q->front = (q->front + 1) % QueueSIZE; //��ͷָ�����
    return frontNum;
}



float AverFilter(Queue* q, float x)
{
    float averResult=0;
    if (q->flag_full == 0)
    {
        InQueue(q, x);//���
        q->sumData += x;//�����ֵ
        averResult=__divf32(q->sumData,(q->rear + 1));
        //averResult=q->sumData/(q->rear + 1);
    }
    else
    {
        q->frontData = OutQueue(q);//�ȳ�����
        InQueue(q, x);//�����
        q->sumData = q->sumData + x - q->frontData;//�����ֵ
        averResult =__divf32(q->sumData,QueueSIZE);
        //averResult=q->sumData/QueueSIZE;
    }
    if (q->rear == QueueSIZE - 1)//�ж϶���
    {
        q->flag_full = 1;
    }
    return averResult;
}

void InitButterQueue(ButterQueue* q)
{
    int i=0;
    q->rear = -1;
    for(i=0;i<=ButterSIZE-1;i++)
    {
        q->data[i] =0;
    }
    q->lastResult = 0;
    q->prevResult = 0;
}
void InButterQueue(ButterQueue* q, float x)
{
    q->rear = (q->rear + 1) % ButterSIZE;    //��βָ�����
    q->data[(q->rear)] = x;     //��x�����β

}


//�÷�ʾ����
//ButterQueue* px =&qx;
//y=ButterWorthFilter( qx, x, SOS,scale)

float ButterWorthFilter(ButterQueue* q, float x, float* SOS,float scale) //���װ�����˹�˲���
{
    double result = 0;
    double a = 0;
    InButterQueue(q, x);
    //a=1/SOS[3];
    a = __divf32(1,SOS[3]);
    result = a * (SOS[0] * q->data[q->rear] + SOS[1] * q->data[(q->rear+2)%ButterSIZE] + SOS[2] * q->data[(q->rear + 1) % ButterSIZE] - SOS[4] * q->lastResult - SOS[5] * q->prevResult);
    q->prevResult = q->lastResult;
    q->lastResult = result;
    return scale*result;
}