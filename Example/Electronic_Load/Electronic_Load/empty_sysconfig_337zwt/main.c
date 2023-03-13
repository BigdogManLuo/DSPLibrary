//����һ���������еĳ���
//1.2.3.4���ⶼOK

#include "driverlib.h"
#include "device.h"
#include "nokia_5110.h"
#include "math.h"
#include "DSPTool.h"


#define EX_ADC_RESOLUTION       12
#define Ts  0.00005 //20K
#define f   50    //��Ƶ
#define TS 5000  //�Ƚϼ�����ֵ
#define Vin_rms 30 //�����ѹ��Чֵ

#define Iref_rms 2 //��е����ο�����Чֵ 2sqrt(2)
#define pi 3.1415
#define Inductive 1  //����
#define Capacitive 2 //����
#define Resistive 3  //����
#define R 14.8
#define Udc_ref 60  //ֱ��ĸ�ߵ�ѹ�ο�

float Iref_p=2.8284; //��е�����ֵ


//arccos���ұ�
#define MAX 121
#define CenterIndex 0.5*(MAX-1)
int index=CenterIndex;  //λ�ڲ��ұ�����
//����phi��   0.5~~����~~1~~����~~0.5 ��Ӧpi/3~~1~~pi/3
float arccos_lookuptable[]={1.15927948072741,1.14834226460814,1.13735100672501,1.12630354985908,1.11519765339907,1.1040309877476,1.09280112827594,1.08150554878078,1.07014161439031,1.05870657386075,1.0471975511966,1.0356115365193,1.02394537609895,1.0121957614521,1.00035921739497,0.988432088926153,0.976410526793834,0.96429047158181,0.952067636122645,0.939737486016875,0.927295218001612,0.914735735869974,0.902053623592525,0.88924311523178,0.876298061168341,0.863211890069541,0.849977565924807,0.836587539341538,0.823033692134976,0.809307274047263,0.795398830184144,0.781298117448725,0.766994007861867,0.752474376163337,0.737725968453249,0.722734247813416,0.707483211779343,0.691955175126317,0.676130509560661,0.659987329387498,0.643501108793284,0.626644211640741,0.609385308030795,0.591688642426544,0.573513104423097,0.554811032980071,0.535526654314388,0.51559400624609,0.494934126340896,0.473451157272066,0.451026811796262,0.427512264944869,0.402715841580661,0.376383482317728,0.348166021272961,0.317560429291521,0.283794109208328,0.245565517515292,0.20033484232312,0.141539473324427,0,0.141539473324427,0.20033484232312,0.245565517515292,0.283794109208328,0.317560429291522,0.348166021272961,0.376383482317728,0.402715841580662,0.427512264944869,0.451026811796262,0.473451157272066,0.494934126340896,0.515594006246091,0.535526654314388,0.554811032980072,0.573513104423097,0.591688642426544,0.609385308030795,0.626644211640741,0.643501108793284,0.659987329387498,0.676130509560661,0.691955175126317,0.707483211779343,0.722734247813416,0.737725968453249,0.752474376163337,0.766994007861867,0.781298117448725,0.795398830184144,0.809307274047263,0.823033692134976,0.836587539341538,0.849977565924807,0.863211890069541,0.876298061168341,0.88924311523178,0.902053623592525,0.914735735869974,0.927295218001612,0.939737486016875,0.952067636122646,0.96429047158181,0.976410526793834,0.988432088926153,1.00035921739497,1.0121957614521,1.02394537609895,1.0356115365193,1.0471975511966,1.05870657386075,1.07014161439031,1.08150554878078,1.09280112827594,1.1040309877476,1.11519765339907,1.12630354985908,1.13735100672501,1.14834226460814,1.15927948072741};

/*****************************ȫ�ֱ�������**********************************/
//�жϼ�����
unsigned int i =0;

//ռ�ձ�
float W_rectifier=0;
float W3_rectifier=0;
float W5_rectifier=0;
float W_inverter=0;
//���������
float error = 0;

//������������
unsigned int pfFlag=3;
float PF=1;

//AD����
float result0 = 0;//Vout
float result1 = 0;//Vhall
float result2 = 0;
float result3 = 0;
float result4 = 0;
float V1=0, V2=0,V3=0,V4=0,V5=0;

//����
float Vin=0;//���ཻ����ѹ
float IL = 0;//������е���
float Vbus=0;//ֱ��ĸ�ߵ�ѹ
float IL_out=0;//������е���
float Vout=0;//����ཻ����ѹ
float Vbus_lowPass=0;//ֱ��ĸ���˲����ѹ
float Vin_BandPass=0;//�����˲���ĵ�ѹ
float c=0; //ռ�ձȹ�һ��ϵ��
float ILref=0; //������е����ο�
float fi=0; //����������

//����
float overVolt=0;
float overCurrent=0;
unsigned int k=0;
unsigned int shutdown= 1; //�������ķ�����־λ
float DAout=0;
unsigned int onceLCD=0;

//��ʼ���˲������� 45~55.56Hz ��ͨ�˲�
ButterQueue qVin={{0,0,0},0,0,0};
//float SOS_BandPass[6]={1000,0,-1000,1000,-1998.521697668364627276105238706804811954,998.768280663740903868585974123561754823};//48~52Hz
float SOS_BandPass[6]={1000,0,-1000,1000,-1996.441622616478728247102480963803827763,996.687968987759798089598461956484243274};//45~55.56Hz
float scale_BandPass=0.001655326640591102448432647697984521074;//45~55.56Hz

//��ʼ���˲������� ��ͨ�˲� ��ֹƵ��50Hz
ButterQueue qVbus={{0,0,0},0,0,0};
float SOS_lowPass[6]={1000,2000,1000,1000,-1977.78648377676380,978.0305084917960698};
float scale_lowPass=0.00006100617875806425746;

//��ʼ��PID����
PID pidTheta={0.01,0.5,0,0,0,0,0.0001};//���໷
PID pidVbus={0.002,0.0002,0,0,0,0,0.00005};//ĸ�ߵ�ѹ����

//��ʼ��PLL����SOGI���в���
ButterQueue qVin_PLL={{0,0,0},0,0,0};
float SOS_PLL[6]={0.61200584083713873220777834660566,1.2240116816742774644155566932113,0.61200584083713873220777834660566,10000.0,-19841.70596900410865615071998036,9844.1539923674570289335861161817};//20k Sample Time
float scale_PLL=1;

//��ʼ��PR����������
ButterQueue qIL={{0,0,0},0,0,0};
ButterQueue qIL_3={{0,0,0},0,0,0};
ButterQueue qIL_5={{0,0,0},0,0,0};
double SOS_PR[6]={0.06282403041950,0,-0.06282403041950,1000.0,-1999.62764254861986,999.874351939160988};//20k Sample Time
double SOS_PR3[6]={0.06279304715072,0,-0.06279304715072,1000.0,-1997.65512442990985,999.874413905698930};//20k Sample Time 3��Ƶ
double SOS_PR5[6]={0.06273117220393,0,-0.06273117220393,1000.0,-1993.71591922600006,999.874537655592007};//20k Sample Time 5��Ƶ
float upLimit=0;
float upLimit_3=0;
float upLimit_5=0;


//��������
void keyboard(void);
void configurePhase(uint32_t base, uint32_t masterBase, uint16_t phaseVal);
void configureADC(uint32_t adcBase);
void configurePhase(uint32_t base, uint32_t masterBase, uint16_t phaseVal);
void initEPWM(void);
void initADCSOC(void);
__interrupt void adcA1ISR(void);
__interrupt void epwm1ISR(void);
void GPIO_init(void);
void configureDAC(void);

void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();
    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Interrupts that are used in this example are re-mapped to ISR functions
    // found within this file.
    //
    Interrupt_register(INT_ADCB2, &adcA1ISR);
    Interrupt_register(INT_EPWM3, &epwm1ISR);
    //
    // Set up the ADC and the ePWM and initialize the SOC
    //
    configureADC(ADCA_BASE);
    configureADC(ADCB_BASE);
    configureADC(ADCC_BASE);
    initEPWM();

    initADCSOC();
    //initCPUTimers();
    GPIO_init();
    LCD_GPIO_Init();
    LCD_init();
    LCD_clear();

    configureDAC();

    DEVICE_DELAY_US(15000);
    SysCtl_setWatchdogPrescaler(SYSCTL_WD_PRESCALE_1);
    SysCtl_setWatchdogMode(SYSCTL_WD_MODE_RESET);
    SysCtl_serviceWatchdog();

    //SysCtl_setWatchdogWindowValue(0.5);
    SysCtl_enableWatchdog();//���Ź�10ms
    //
    // Enable ADC interrupt
    //
    //configCPUTimer(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, 1000);
    //CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    Interrupt_enable(INT_ADCB2);
    Interrupt_enable(INT_EPWM3);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Start ePWM1, enabling SOCA and putting the counter in up-count mode
    //
    EPWM_enableADCTrigger(EPWM3_BASE, EPWM_SOC_A);

    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setTimeBaseCounterMode(EPWM2_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setTimeBaseCounterMode(EPWM3_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setTimeBaseCounterMode(EPWM4_BASE, EPWM_COUNTER_MODE_UP_DOWN);

    //CLAY
    LCD_clear();
    LCD_ShowString(10,10,"CLAY");
    DEVICE_DELAY_US(3000000);
    LCD_clear();

    while(1)
    {
        keyboard();
        //show_statusofLoad(pfFlag,index); test
        //�������

        if (shutdown==1)
        {
            if (onceLCD==0)
            {
                LCD_clear();
                onceLCD=1;
            }
            LCD_ShowString(2,2,"OFF");
            //LCD_ShowFloat(50,0.7,Vbus,2,2);
        }
        //��ʾ�趨�����
        else if (overVolt==0&&shutdown==0)
        {
            //show_adc_result(Iref_p,IL);
            show_statusofLoad(index,PF,CenterIndex);//��������cosfi
            float temp=abs(__divf32(fi,2*pi)*360);
            LCD_ShowFloat(0,3.2,temp,2,2); //fi �Ƕ�ֵ
            LCD_ShowFloat(0,5,index,3,1); //����

        }
        //��ѹ����
        else if(overVolt==1)
        {
            LCD_clear();
            show_warning1();

        }

    }

}

__interrupt void adcA1ISR(void)
{



    result0 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);//���뽻����ѹ
    result1 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);//ֱ��ĸ�ߵ�ѹ
    result2 = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER2);//���������ѹ
    result3 = ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER3);//�����е���
    result4 = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER4);//�����е���

    //
    // Clear the interrupt flag
    //
    ADC_clearInterruptStatus(ADCB_BASE, ADC_INT_NUMBER2);

    //
    // Check if overflow has occurred
    //
    if(true == ADC_getInterruptOverflowStatus(ADCB_BASE, ADC_INT_NUMBER2))
    {
        ADC_clearInterruptOverflowStatus(ADCB_BASE, ADC_INT_NUMBER2);
        ADC_clearInterruptStatus(ADCB_BASE, ADC_INT_NUMBER2);
    }

    //
    // Acknowledge the interrupt
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP10);
}

__interrupt void epwm1ISR(void)
{
    //�˲�������ָ���ʼ��
     ButterQueue* pVin= &qVin;
     ButterQueue* pVbus = &qVbus;

     //PIDָ���ʼ��
     PID* pPIDTheta=&pidTheta;
     PID* pPIDVbus=&pidVbus;

     //PLL_SOGI����ָ���ʼ��
     ButterQueue* pVin_PLL=&qVin_PLL;

     //PR����������ָ���ʼ��
     ButterQueue* pIL=&qIL;
     ButterQueue* pIL_3=&qIL_3;
     ButterQueue* pIL_5=&qIL_5;
     float* pUplimit=&upLimit;
     float* pUplimit_3=&upLimit_3;
     float* pUplimit_5=&upLimit_5;

    //AD�����ϵ�ʵ�ʵ�ѹ
    V1 = result0*0.0002442*3;//����ཻ����ѹ
    V2 = result1*0.0002442*3;//ֱ��ĸ�ߵ�ѹ
    V3 = result2*0.0002442*3;//����ཻ����ѹ
    V4 = result3*0.0002442*3;//������е���
    V5 = result4*0.0002442*3;//������е���

    //��ѹ��������
    //Vin=69.366*V1-104.96;
    Vin = 93-62*V1;//����ཻ����ѹ   δУ׼
    Vbus = 57.04*V2+0.26;//ֱ��ĸ�ߵ�ѹ
    Vout = 93-62*V3;//����ཻ����ѹ δУ׼
    IL_out =25-20*V4;//������е��� δУ׼
    IL = 20.126*(V5-1.25);//������е���

    //�����ѹ��ͨ�˲�
    Vin_BandPass=ButterWorthFilter(pVin,Vin,SOS_BandPass,scale_BandPass);

    //ֱ��ĸ�ߵ�ѹ�˲�
    Vbus_lowPass=ButterWorthFilter(pVbus,Vbus,SOS_lowPass,scale_lowPass);

    //���໷
    singlePhasePLLCalc(pPIDTheta,Vin_BandPass,pVin_PLL,SOS_PLL,scale_PLL);

/*
    //����
    if((abs(IL)>10)||(Vbus_lowPass>70))
    {
        //SHUT DOWN
        GPIO_writePin(56,1);
        GPIO_writePin(104,1);
        //�����һֱ���
        //GPIO_writePin(60,1);
        //GPIO_writePin(18,1);
        //�������������������־λ
        shutdown = 1;
        //LCD��ʾ����
        overVolt=1;
        onceLCD=0;
    }
    else
    {
        overVolt=0;
    }
*/

    //���㹦�������� ��Index����  ����У׼��������
    if(index>CenterIndex)//����Ǹ���
    {
        fi=-arccos_lookuptable[index];
        fi+=(0.0001976*index*index-0.005175*index+0.09318);
    }
    else if(index<CenterIndex)//���������
    {
        fi=arccos_lookuptable[index];
        fi+=(-0.0006746*index-0.03732);
    }
    else if(index==CenterIndex)
    {
        fi=0;
    }
    //���㹦������
    PF = 1-abs(CenterIndex-index)*0.01;


    if(shutdown==0)
    {
        /******************************����������******************************************/

        //�����е����ο�ILref
        ILref=Iref_p*__cos(theta+fi);

        //DAC
        DAout=IL*0.1*4095+2048;
        if(DAout>4095){DAout=4095;}
        DAC_setShadowValue(DACB_BASE,DAout);

        //PR����
        W_rectifier=Vin_BandPass+Iref_p*1*__sin(theta)-PRCalc(pIL,ILref-IL,SOS_PR,1,4,50,0.5*65,10,pUplimit);
        //W_rectifier=Vin_BandPass+Iref_p*1*__sin(theta)-20*(ILref-IL);

        //г������
        W3_rectifier=-PRCalc(pIL_3,0-IL,SOS_PR3,1,0,10,0.5*65,10,pUplimit_3);
        W5_rectifier=-PRCalc(pIL_5,0-IL,SOS_PR5,1,0,10,0.5*65,10,pUplimit_5);

        //��һ��
        W_rectifier=W_rectifier+W3_rectifier+W5_rectifier;
        W_rectifier=__divf32(W_rectifier,60);
        W_rectifier=W_rectifier*0.5;
        W_rectifier+=0.5;

        //ռ�ձ��޷�
        if (W_rectifier > 0.95)
        {
            W_rectifier=0.95;
        }
        else if (W_rectifier<0.05)
        {
            W_rectifier=0.05;
        }
        /**********************************************************************************************/

        /**********************************���������****************************************************/

        //ֱ��ĸ�ߵ�ѹ����
        float W_feedback=-PIDCalc(pPIDVbus,Vbus_lowPass,Udc_ref,10);
        //PID����޷�
        if (W_feedback>=0.2)
        {
            W_feedback=0.2;
        }
        else if (W_feedback<=-0.2)
        {
            W_feedback=-0.2;
        }

        //����ǰ��  ����
        float Vout_peak=__sqrt(120*R*PF);
        float Dr_peak=0.5*(__divf32(Vout_peak,Udc_ref)+1);
        float A=W_feedback+(Dr_peak-0.5); //feedback��С����Ҫ������һ��

        W_inverter=A*__cos(theta);//�õ��������Ʋ�
        W_inverter+=0.5;//ֱ��ƫ��

    }

    else
    {
        pIL->data[0]=0;pIL->data[1]=0;pIL->data[2]=0;
        pIL->lastResult=0;
        pIL->prevResult=0;
        pIL->rear=0;
        pIL_3->data[0]=0;pIL_3->data[1]=0;pIL_3->data[2]=0;
        pIL_3->lastResult=0;
        pIL_3->prevResult=0;
        pIL_3->rear=0;
        pIL_5->data[0]=0;pIL_5->data[1]=0;pIL_5->data[2]=0;
        pIL_5->lastResult=0;
        pIL_5->prevResult=0;
        pIL_5->rear=0;
        W_rectifier=0;
        W3_rectifier=0;
        W5_rectifier=0;
        Iref_p=2.8284;
        pPIDVbus->LastError=0;
        pPIDVbus->PrevError=0;
        pPIDVbus->SumError=0;
        float Vout_peak=__sqrt(120*R*PF);
        float Dr_peak=0.5*(__divf32(Vout_peak,Udc_ref)+1);
        float A=(Dr_peak-0.5); //feedback��С����Ҫ������һ��
        W_inverter=A*__cos(theta);//�õ��������Ʋ�
        W_inverter+=0.5;//ֱ��ƫ��

    }

    //   EPWM_setCounterCompareValue(base,EPWM_COUNTER_COMPARE_A,duty);

    //������PWM���
    EPWM_setCounterCompareValue(EPWM4_BASE,EPWM_COUNTER_COMPARE_A,(int)((1-W_rectifier)*TS));//����A
    EPWM_setCounterCompareValue(EPWM3_BASE,EPWM_COUNTER_COMPARE_A,(int)(W_rectifier*TS));//����B
    //�����PWM���
    EPWM_setCounterCompareValue(EPWM2_BASE,EPWM_COUNTER_COMPARE_A,(int)(W_inverter*TS));//����C
    EPWM_setCounterCompareValue(EPWM1_BASE,EPWM_COUNTER_COMPARE_A,(int)(W_inverter*TS));//����D

    // Clear INT flag for this timer
    //
    SysCtl_serviceWatchdog();
    EPWM_clearEventTriggerInterruptFlag(EPWM3_BASE);

    //
    // Acknowledge interrupt group
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);
}

void keyboard()
{
    if(GPIO_readPin(125)==0)  //Index+1
    {
        DEVICE_DELAY_US(2);
        while(GPIO_readPin(125)==0);
        index+=1;
        //index�޷� ��ֹ�ڴ����
        if (index>=MAX-1)
        {
            index=MAX-1;
        }
        else if (index<=0)
        {
            index=0;
        }
    }

    if(GPIO_readPin(124)==0)  //index-
    {
        DEVICE_DELAY_US(2);
        while(GPIO_readPin(124)==0);
        index-=1;
        //index�޷� ��ֹ�ڴ����
        if (index>=MAX-1)
        {
            index=MAX-1;
        }
        else if (index<=0)
        {
            index=0;
        }
    }

    if(GPIO_readPin(29)==0) //shut down
    {

        DEVICE_DELAY_US(2);
        while(GPIO_readPin(29)==0);
        //��תshut down GPIO             ��ʱ���ε���������   �õ�.
        GPIO_togglePin(56);
        GPIO_togglePin(104);
        //GPIO_togglePin(60);
        //GPIO_togglePin(18);
        //��ת����
        if(shutdown==0){shutdown=1;}
        else if(shutdown==1){shutdown=0;}
        if(onceLCD==0){onceLCD=1;}
        else if(onceLCD==1){onceLCD=0;}
        pfFlag = Resistive;
        //LCD Clear
        LCD_clear();
    }

   if(GPIO_readPin(59)==0)//index+10
    {

        DEVICE_DELAY_US(2);
        while(GPIO_readPin(59)==0);
        index+=10;
        //index�޷� ��ֹ�ڴ����
        if (index>=MAX-1)
        {
            index=MAX-1;
        }
        else if (index<=0)
        {
            index=0;
        }
    }

   if(GPIO_readPin(58)==0)//index-10;
    {

       DEVICE_DELAY_US(2);
       while(GPIO_readPin(58)==0);
       index-=10;
       //index�޷� ��ֹ�ڴ����
       if (index>=MAX-1)
       {
           index=MAX-1;
       }
       else if (index<=0)
       {
           index=0;
       }
    }

   if(GPIO_readPin(122)==0)
    {

       DEVICE_DELAY_US(2);
       while(GPIO_readPin(122)==0);
    }
   if(GPIO_readPin(123)==0)
    {

       DEVICE_DELAY_US(2);
       while(GPIO_readPin(123)==0);
       Iref_p+=0.01;
    }
   if(GPIO_readPin(61)==0)
    {

       DEVICE_DELAY_US(2);
       while(GPIO_readPin(61)==0);
       Iref_p-=0.01;
    }
}

void configureDAC(void)
{
    //
    // Set VDAC as the DAC reference voltage.
    // Edit here to use ADC VREF as the reference voltage.
    //
    DAC_setReferenceVoltage(DACB_BASE, DAC_REF_ADC_VREFHI);

    //
    // Enable the DAC output
    //
    DAC_enableOutput(DACB_BASE);

    //
    // Set the DAC shadow output to 0
    //
    DAC_setShadowValue(DACB_BASE, 0);

    //
    // Delay for buffered DAC to power up
    //
    DEVICE_DELAY_US(10);
}
void GPIO_init(void)
{
    EALLOW;

    //myGPIOOutput0 initialization


    GPIO_setPinConfig(GPIO_0_EPWM1A);
    GPIO_setPinConfig(GPIO_1_EPWM1B);
    GPIO_setPinConfig(GPIO_2_EPWM2A);
    GPIO_setPinConfig(GPIO_3_EPWM2B);
    GPIO_setPinConfig(GPIO_4_EPWM3A);
    GPIO_setPinConfig(GPIO_5_EPWM3B);
    GPIO_setPinConfig(GPIO_6_EPWM4A);
    GPIO_setPinConfig(GPIO_7_EPWM4B);


    //keyboard
    GPIO_setPadConfig(29, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_29_GPIO29);
    GPIO_setDirectionMode(29, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(125, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_125_GPIO125);
    GPIO_setDirectionMode(125, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(124, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_124_GPIO124);
    GPIO_setDirectionMode(124, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(59, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_59_GPIO59);
    GPIO_setDirectionMode(59, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(58, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_58_GPIO58);
    GPIO_setDirectionMode(58, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(122, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_122_GPIO122);
    GPIO_setDirectionMode(122, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(123, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_123_GPIO123);
    GPIO_setDirectionMode(123, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(61, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_61_GPIO61);
    GPIO_setDirectionMode(61, GPIO_DIR_MODE_IN);
    //shutdown
    GPIO_setDirectionMode(56, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(56, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_56_GPIO56);
    GPIO_setDirectionMode(104, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(104, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_104_GPIO104);
    GPIO_setDirectionMode(60, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(60, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_60_GPIO60);
    GPIO_setDirectionMode(18, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(18, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_18_GPIO18);





    //shutdown��������
    GPIO_writePin(56,1);
    GPIO_writePin(104,1);
    //�����һֱ���
    GPIO_writePin(60,0);
    GPIO_writePin(18,0);
    //test

    EDIS;
}
//
// configureADC - Write ADC configurations and power up the ADC for the
// selected ADC
//
void configureADC(uint32_t adcBase)
{
    //
    // Set ADCDLK divider to /4
    //

    ADC_setPrescaler(adcBase, ADC_CLK_DIV_4_0);

    //
    // Set resolution and signal mode (see #defines above) and load
    // corresponding trims.
    //
#if(EX_ADC_RESOLUTION == 12)
    ADC_setMode(adcBase, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
#elif(EX_ADC_RESOLUTION == 16)
    ADC_setMode(adcBase, ADC_RESOLUTION_16BIT, ADC_MODE_DIFFERENTIAL);
#endif

    //
    // Set pulse positions to late
    //
    ADC_setInterruptPulseMode(adcBase, ADC_PULSE_END_OF_CONV);

    //
    // Power up the ADCs and then delay for 1 ms
    //
    ADC_enableConverter(adcBase);

    //
    // Delay for 1ms to allow ADC time to power up
    //
    DEVICE_DELAY_US(1000);
}

//
// Function to configure ePWM1 to generate the SOC.
//
void initEPWM(void)
{

    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    EPWM_setClockPrescaler(EPWM1_BASE,EPWM_CLOCK_DIVIDER_1,EPWM_HSCLOCK_DIVIDER_1);
    EPWM_setClockPrescaler(EPWM2_BASE,EPWM_CLOCK_DIVIDER_1,EPWM_HSCLOCK_DIVIDER_1);
    EPWM_setClockPrescaler(EPWM3_BASE,EPWM_CLOCK_DIVIDER_1,EPWM_HSCLOCK_DIVIDER_1);
    EPWM_setClockPrescaler(EPWM4_BASE,EPWM_CLOCK_DIVIDER_1,EPWM_HSCLOCK_DIVIDER_1);


    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setTimeBaseCounterMode(EPWM2_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setTimeBaseCounterMode(EPWM3_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setTimeBaseCounterMode(EPWM4_BASE, EPWM_COUNTER_MODE_UP_DOWN);

    EPWM_setTimeBasePeriod(EPWM1_BASE, TS);
    EPWM_setTimeBasePeriod(EPWM2_BASE, TS);
    EPWM_setTimeBasePeriod(EPWM3_BASE, TS);
    EPWM_setTimeBasePeriod(EPWM4_BASE, TS);

    EPWM_setCounterCompareShadowLoadMode(EPWM1_BASE,EPWM_COUNTER_COMPARE_A,EPWM_COMP_LOAD_ON_CNTR_PERIOD);
    EPWM_setCounterCompareShadowLoadMode(EPWM2_BASE,EPWM_COUNTER_COMPARE_A,EPWM_COMP_LOAD_ON_CNTR_PERIOD);
    EPWM_setCounterCompareShadowLoadMode(EPWM3_BASE,EPWM_COUNTER_COMPARE_A,EPWM_COMP_LOAD_ON_CNTR_PERIOD);
    EPWM_setCounterCompareShadowLoadMode(EPWM4_BASE,EPWM_COUNTER_COMPARE_A,EPWM_COMP_LOAD_ON_CNTR_PERIOD);

    EPWM_setCounterCompareValue(EPWM1_BASE,EPWM_COUNTER_COMPARE_A,0);
    EPWM_setCounterCompareValue(EPWM2_BASE,EPWM_COUNTER_COMPARE_A,0);
    EPWM_setCounterCompareValue(EPWM3_BASE,EPWM_COUNTER_COMPARE_A,0);
    EPWM_setCounterCompareValue(EPWM4_BASE,EPWM_COUNTER_COMPARE_A,0);

    EPWM_setActionQualifierAction(EPWM1_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM1_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM1_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(EPWM1_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM1_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM1_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

    EPWM_setActionQualifierAction(EPWM2_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM2_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM2_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(EPWM2_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM2_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM2_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

    EPWM_setActionQualifierAction(EPWM3_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM3_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM3_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(EPWM3_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM3_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM3_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

    EPWM_setActionQualifierAction(EPWM4_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM4_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM4_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(EPWM4_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM4_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM4_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);



    EPWM_setSyncOutPulseMode(EPWM3_BASE,EPWM_SYNC_OUT_PULSE_ON_COUNTER_ZERO);
    EPWM_setSyncOutPulseMode(EPWM1_BASE,EPWM_SYNC_OUT_PULSE_ON_EPWMxSYNCIN);
    EPWM_setSyncOutPulseMode(EPWM2_BASE,EPWM_SYNC_OUT_PULSE_ON_EPWMxSYNCIN);
    EPWM_setSyncOutPulseMode(EPWM4_BASE,EPWM_SYNC_OUT_PULSE_ON_EPWMxSYNCIN);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    EPWM_setInterruptSource(EPWM3_BASE, EPWM_INT_TBCTR_ZERO_OR_PERIOD);//�޸Ĺ�
    EPWM_enableInterrupt(EPWM3_BASE);
    EPWM_setInterruptEventCount(EPWM3_BASE, 1U);

    EPWM_disableADCTrigger(EPWM3_BASE, EPWM_SOC_A);
    EPWM_setADCTriggerSource(EPWM3_BASE, EPWM_SOC_A, EPWM_SOC_TBCTR_ZERO_OR_PERIOD);//�޸Ĺ�
    EPWM_setADCTriggerEventPrescale(EPWM3_BASE, EPWM_SOC_A, 1);

    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
    EPWM_setTimeBaseCounterMode(EPWM2_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
    EPWM_setTimeBaseCounterMode(EPWM3_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
    EPWM_setTimeBaseCounterMode(EPWM4_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);

}
//
// Function to configure SOCs on ADCA and ADCD to be triggered by ePWM3.
//

void initADCSOC(void)
{
        uint16_t acqps;

    //
    // Determine minimum acquisition window (in SYSCLKS) based on resolution
    //
    if(EX_ADC_RESOLUTION == 12)
    {
        acqps = 14; // 75ns
    }
    else //resolution is 16-bit
    {
        acqps = 63; // 320ns
    }
    //
    // - NOTE: A longer sampling window will be required if the ADC driving
    //   source is less than ideal (an ideal source would be a high bandwidth
    //   op-amp with a small series resistance). See TI application report
    //   SPRACT6 for guidance on ADC driver design.
    // - NOTE: SOCs need not use the same S+H window duration, but SOCs
    //   occurring in parallel (in this example, SOC0 on both ADCs occur in
    //   parallel, as do the SOC1s on both ADCs, etc.) should usually
    //   use the same value to ensure simultaneous samples and synchronous
    //   operation.

    //
    // Select the channels to convert and the configure the ePWM trigger

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM3_SOCA,
                     ADC_CH_ADCIN2, acqps);//����ཻ����ѹ
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM3_SOCA,
                     ADC_CH_ADCIN0, acqps);//ֱ��ĸ�ߵ�ѹ
    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM3_SOCA,
                     ADC_CH_ADCIN3, acqps);//����ཻ����ѹ
    ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_EPWM3_SOCA,
                     ADC_CH_ADCIN3, acqps);//������е���
    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER4, ADC_TRIGGER_EPWM3_SOCA,
                     ADC_CH_ADCIN2, acqps);//������е���


    /*
    ADC_setupSOC(ADCD_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM1_SOCA,
                 ADC_CH_ADCIN3, acqps);
    ADC_setupSOC(ADCD_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM1_SOCA,
                 ADC_CH_ADCIN4, acqps);*/

    //
    // Selec SOC2 on ADCA as the interrupt source.  SOC2 on ADCD will end at
    // the same time, so either SOC2 would be an acceptable interrupt triggger.
    //

    ADC_setInterruptSource(ADCB_BASE, ADC_INT_NUMBER2, ADC_SOC_NUMBER4);
    ADC_enableInterrupt(ADCB_BASE, ADC_INT_NUMBER2);
    ADC_clearInterruptStatus(ADCB_BASE, ADC_INT_NUMBER2);

}