#pragma once

/*
#define true 1 
#define false 0
typedef int bool
*/

#define QueueSIZE 40    //���������Ԫ�ص�������
#define ButterSIZE 3

typedef struct {
    float data[QueueSIZE];  //����Ԫ��
    int front;         //��ͷָ��
    int rear;          //��βָ��
    int flag_full;
    float sumData;
    float frontData;
}Queue;

typedef struct {
    float data[ButterSIZE];
    int rear;          //��βָ��
    float lastResult;
    float prevResult;
}ButterQueue;


//extern double SOS_Vout[6];   // 10k������˹�˲���
//extern double scale_Vout;


//�⺯��������
extern void InitQueue(Queue* q);
extern void InQueue(Queue* q, float x);
extern float OutQueue(Queue* q);
extern float AverFilter(Queue* p, float x);
extern void InitButterQueue(ButterQueue* q);
extern void InButterQueue(ButterQueue* q, float x);
extern double ButterWorthFilter(ButterQueue* q, float x, double* SOS, double scale); //���װ�����˹�˲���