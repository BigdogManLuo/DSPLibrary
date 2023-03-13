#include"queque.h"

// 初始化队列

ButterQueue qV1={{0,0,0},0,0,0};
ButterQueue Vsa={{0,0,0},0,0,0};
ButterQueue Vsb={{0,0,0},0,0,0};
ButterQueue Vsc={{0,0,0},0,0,0};

//初始化巴特沃斯滤波参数
float SOS[6] = { 1,0, - 1 , 1, - 1.9924 , 0.9934 };
float SOS_lowPass[6]={1,2,1,1,-1.956,0.957};
float scale=0.0033;
float scale_lowPass=0.00024;



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

// 入队
void InQueue(Queue*q, float x)
{
	q->rear = (q->rear + 1) % QueueSIZE;    //队尾指针后移
	q->data[(q->rear)] = x;		//将x插入队尾
	
}

// 出队
float OutQueue(Queue* q)
{   
	float frontNum = 0;
	frontNum = q->data[q->front];
	q->front = (q->front + 1) % QueueSIZE; //队头指针后移
	return frontNum;
}



float AverFilter(Queue* q, float x)
{
	float averResult=0;
	if (q->flag_full == 0)
	{
		InQueue(q, x);//入队
		q->sumData += x;//计算均值
		averResult=__divf32(q->sumData,(q->rear + 1));
    }
	else
	{
		q->frontData = OutQueue(q);//先出队伍
		InQueue(q, x);//再入队
		q->sumData = q->sumData + x - q->frontData;//计算均值
		averResult =__divf32(q->sumData,QueueSIZE);
	}
	if (q->rear == QueueSIZE - 1)//判断队满
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
	q->rear = (q->rear + 1) % ButterSIZE;    //队尾指针后移
	q->data[(q->rear)] = x;		//将x插入队尾

}

float ButterWorthFilter(ButterQueue* q, float x, float* SOS,float scale) //二阶巴特沃斯滤波器
{
	float result = 0;
	InButterQueue(q, x);
	result = (1 / SOS[3]) * (SOS[0] * q->data[q->rear] + SOS[1] * q->data[(q->rear+2)%ButterSIZE] + SOS[2] * q->data[(q->rear + 1) % ButterSIZE] - SOS[4] * q->lastResult - SOS[5] * q->prevResult);
	q->prevResult = q->lastResult;
	q->lastResult = result;
	return scale*result;
}
