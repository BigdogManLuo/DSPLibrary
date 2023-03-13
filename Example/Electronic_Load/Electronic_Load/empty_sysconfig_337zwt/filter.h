#ifndef _FILTER_H
#define _FILTER_H
/*
#define true 1 
#define false 0
typedef int bool
*/

#define QueueSIZE 40    //定义队列中元素的最大个数
#define ButterSIZE 3

typedef struct {
    float data[QueueSIZE];  //队列元素
    int front;         //队头指针
    int rear;          //队尾指针
    int flag_full;
    float sumData;
    float frontData;
}Queue;

typedef struct {
    float data[ButterSIZE];
    int rear;          //队尾指针
    float lastResult;
    float prevResult;
}ButterQueue;


//extern double SOS_Vout[6];   // 10k巴特沃斯滤波器
//extern double scale_Vout;


//库函数的声明
void InitQueue(Queue* q);
void InQueue(Queue* q, float x);
float OutQueue(Queue* q);
float AverFilter(Queue* p, float x);
void InitButterQueue(ButterQueue* q);
void InButterQueue(ButterQueue* q, float x);
float ButterWorthFilter(ButterQueue* q, float x, float* SOS, float scale); //二阶巴特沃斯滤波器

#endif
