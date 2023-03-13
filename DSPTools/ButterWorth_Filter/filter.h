#pragma once

#define QueueSIZE 40	//定义队列中元素的最大个数
#define ButterSIZE 3

//普通队列
typedef struct {
	float data[QueueSIZE];  //队列元素
	int front;		   //队头指针
	int rear;		   //队尾指针
	int flag_full;
	float sumData;
	float frontData;
}Queue;

//巴特沃斯滤波器队列
typedef struct {
	float data[ButterSIZE];
	int rear;		   //队尾指针
	float lastResult;
	float prevResult;
}ButterQueue;

//库函数的声明
extern void InitQueue(Queue* q);
extern void InQueue(Queue* q, float x);
extern float OutQueue(Queue* q);
extern float AverFilter(Queue* p, float x);
extern void InitButterQueue(ButterQueue* q);
extern void InButterQueue(ButterQueue* q, float x);
extern double ButterWorthFilter(ButterQueue* q, float x, double* SOS, double scale); //二阶巴特沃斯滤波器