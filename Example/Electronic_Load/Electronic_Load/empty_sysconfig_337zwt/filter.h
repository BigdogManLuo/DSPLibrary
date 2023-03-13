#ifndef _FILTER_H
#define _FILTER_H
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
void InitQueue(Queue* q);
void InQueue(Queue* q, float x);
float OutQueue(Queue* q);
float AverFilter(Queue* p, float x);
void InitButterQueue(ButterQueue* q);
void InButterQueue(ButterQueue* q, float x);
float ButterWorthFilter(ButterQueue* q, float x, float* SOS, float scale); //���װ�����˹�˲���

#endif
