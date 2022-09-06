//****************************************************//
//1.2版本更新内容：
//1.保护子程序放在了文件protect.h和protect.c中调用
//2.采用#pragma once的方式保护头文件
//3.坐标变换放在了文件transform.h和transform.c
//4.占空比加偏置使得输出正弦波的零点对应占空比50%
//5.添加占空比前馈
//6.dq变换下的闭环控制

//1.3版本更新内容：
//1.修改了三相负载
//2.使用了math.h，可以开环调整频率，基础值50，步进值0.01
//3.将电流环转变成电压环，增加了3路电压的采集
//4.程序可以正常运行，下一步进行双闭环

//1.4版本更新内容
//1.双闭环芜锁胃了
//2.修改了lcd程序，现在为硬件spi通信，未使用FIFO、DAM和中断，每次传输数据有5us延时
//3.下一步在spi中添加fifo，增加通信效率

//1.5版本更新内容
//1.parkTransformation函数增加参数w
//2.每个开关周期改变w的值，也就是改变f的值，让电源侧Vq电压趋近于0
//
//****************************************************//

#include "driverlib.h"
#include "device.h"
#include "nokia_5110.h"
#include "pid.h"
#include "protect.h"
#include"transform.h"
#include "math.h"

#define EX_ADC_RESOLUTION       12
#define atten_Vin 11
#define atten_Vout 11
#define atten_Vhall 2
#define _K 5.4051  //ACS712系数
#define pi 3.1415
float result0 = 0; //aVhall
float result1 = 0;//bVhall
float result2 = 0;//cVhall
float result3 = 0;//Vin
float result4 = 0;//aVout
float result5 = 0;//bVout
float result6 = 0;//cVout
float Vin = 0;    //输入电压
float aVhall=0, bVhall=0, cVhall=0; //3相电感电流
float aVout=0, bVout=0, cVout=0;
int Ra=7,Rb=7,Rc=7;//3相负载


int kf = 0;
extern int k;
extern float f;
extern float Ts;
float w=0;


int outFlag = 0;  //是否有输出的标志位

//int k = 0;    //离散的时基 k*Ts=t

float Im=0.5;   //设定输出电流赋值
float set_volt = 5.00;//设定输出电压
int flag=0;

int m;//累加计数
int LCD_i = 0;
float Vdq_aver = 0;
float Vdq_sum = 0;
int Vdq_i = 0;

void keyboard()
{
    if(GPIO_readPin(125)==0)
    {
        DEVICE_DELAY_US(2);
        while(GPIO_readPin(125)==0);
        set_volt += 0.1;
        if(set_volt>12)
        {
            set_volt=12;
        }

    }

    if(GPIO_readPin(124)==0)
        {
            DEVICE_DELAY_US(2);
            while(GPIO_readPin(124)==0);
            set_volt -= 0.1;
            if(set_volt<2)
            {
                set_volt=2;
            }

        }
    if(GPIO_readPin(29)==0)
        {
            DEVICE_DELAY_US(2);
            while(GPIO_readPin(29)==0);
            GPIO_writePin(25,0);
            GPIO_writePin(52,0);
            GPIO_writePin(56,0);
        }
    if(GPIO_readPin(59)==0)
           {
               DEVICE_DELAY_US(2);
               while(GPIO_readPin(59)==0);
               f += 0.01;
               if(f>50.2)
               {f = 50.2;}
           }

    if(GPIO_readPin(58)==0)
           {
               DEVICE_DELAY_US(2);
               while(GPIO_readPin(58)==0);
               f -= 0.01;
               if(f<49.8)
               {f = 49.8;}
              }
}
void configurePhase(uint32_t base, uint32_t masterBase, uint16_t phaseVal)
{
    uint32_t readPrdVal, phaseRegVal;

    //
    // Read Period value to calculate value for Phase Register
    //
    readPrdVal = EPWM_getTimeBasePeriod(masterBase);

    //
    // Caluclate phase register values based on Time Base counter mode
    //
    if((HWREGH(base + EPWM_O_TBCTL) & 0x3U) == EPWM_COUNTER_MODE_UP_DOWN)
    {
        phaseRegVal = (2U * readPrdVal * phaseVal) / 360U;
    }
    else if((HWREGH(base + EPWM_O_TBCTL) & 0x3U) < EPWM_COUNTER_MODE_UP_DOWN)
    {
        phaseRegVal = (readPrdVal * phaseVal) / 360U;
    }

    EPWM_selectPeriodLoadEvent(base, EPWM_SHADOW_LOAD_MODE_SYNC);
    EPWM_setPhaseShift(base, phaseRegVal);
    EPWM_setTimeBaseCounter(base, phaseRegVal);
}


//
// Function Prototypes
//

void configureADC(uint32_t adcBase);
void configurePhase(uint32_t base, uint32_t masterBase, uint16_t phaseVal);
void initEPWM(void);
void initADCSOC(void);
__interrupt void adcA1ISR(void);
__interrupt void epwm1ISR(void);

void GPIO_init(void)
{
    EALLOW;

    //myGPIOOutput0 initialization


    GPIO_setPinConfig(GPIO_6_EPWM4A);
    GPIO_setPinConfig(GPIO_8_EPWM5A);
    GPIO_setPinConfig(GPIO_10_EPWM6A);

        //myGPIOOutput0 initialization
    GPIO_setDirectionMode(66, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(66, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(66, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(66, GPIO_QUAL_SYNC);

    GPIO_setPadConfig(125, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO66
    GPIO_setPinConfig(GPIO_125_GPIO125);               // GPIO66 = GPIO66
    GPIO_setDirectionMode(125, GPIO_DIR_MODE_IN);     // GPIO66 = input
    GPIO_setPadConfig(124, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO131
    GPIO_setPinConfig(GPIO_124_GPIO124);               // GPIO131 = GPIO131
    GPIO_setDirectionMode(124, GPIO_DIR_MODE_IN);     // GPIO131 = input
    GPIO_setPadConfig(59, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_59_GPIO59);
    GPIO_setDirectionMode(59, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(58, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_58_GPIO58);
    GPIO_setDirectionMode(58, GPIO_DIR_MODE_IN);

    GPIO_setDirectionMode(25, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(25, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_25_GPIO25);
    GPIO_setDirectionMode(52, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(52, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_52_GPIO52);
    GPIO_setDirectionMode(56, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(56, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(GPIO_56_GPIO56);
    GPIO_setPadConfig(29, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO131
    GPIO_setPinConfig(GPIO_29_GPIO29);               // GPIO131 = GPIO131
    GPIO_setDirectionMode(29, GPIO_DIR_MODE_IN);     // GPIO131 = input

    EDIS;
}

//
// Main
//
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
    Interrupt_register(INT_ADCA4, &adcA1ISR);
    Interrupt_register(INT_EPWM4, &epwm1ISR);
    //
    // Set up the ADC and the ePWM and initialize the SOC
    //
    configureADC(ADCA_BASE);
    configureADC(ADCB_BASE);
    configureADC(ADCC_BASE);
    //configureADC(ADCD_BASE);
    initEPWM();

    initADCSOC();
    //initCPUTimers();
    GPIO_init();

    LCD_GPIO_Init();
    LCD_init();
    LCD_clear();

    DEVICE_DELAY_US(1000);
    //
    // Enable ADC interrupt
    //
    //configCPUTimer(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, 1000);
    //CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    Interrupt_enable(INT_ADCA4);
    Interrupt_enable(INT_EPWM4);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Start ePWM1, enabling SOCA and putting the counter in up-count mode
    //
    EPWM_enableADCTrigger(EPWM4_BASE, EPWM_SOC_A);

    EPWM_setTimeBaseCounterMode(EPWM6_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setTimeBaseCounterMode(EPWM5_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setTimeBaseCounterMode(EPWM4_BASE, EPWM_COUNTER_MODE_UP_DOWN);

    while(1)
    {
        keyboard();
        if(LCD_i==4000)
        {
        show_adc_result(set_volt,Vdq_aver);
        LCD_i = 0;
        }
        LCD_i++;

        /*

       if(LCD_i==15000)
               {
                 //  LCD_clear();

                   LCD_i = 0;
               }

       LCD_i++;
*/
      //

    //  GPIO_togglePin(66);
        //DEVICE_DELAY_US(1000000);



        //
        // Wait while ePWM causes ADC conversions.
        // ADCA1 ISR processes each new set of conversions.
        //
    }

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

    EPWM_setClockPrescaler(EPWM6_BASE,EPWM_CLOCK_DIVIDER_1,EPWM_HSCLOCK_DIVIDER_1);
    EPWM_setClockPrescaler(EPWM5_BASE,EPWM_CLOCK_DIVIDER_1,EPWM_HSCLOCK_DIVIDER_1);
    EPWM_setClockPrescaler(EPWM4_BASE,EPWM_CLOCK_DIVIDER_1,EPWM_HSCLOCK_DIVIDER_1);
/*
      EPWM_setTimeBaseCounter(EPWM6_BASE, 0);
      EPWM_setTimeBaseCounter(EPWM5_BASE, 0);
      EPWM_setTimeBaseCounter(EPWM4_BASE, 0);
*/
    EPWM_setTimeBaseCounterMode(EPWM6_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setTimeBaseCounterMode(EPWM5_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setTimeBaseCounterMode(EPWM4_BASE, EPWM_COUNTER_MODE_UP_DOWN);

    EPWM_setTimeBasePeriod(EPWM6_BASE, 5000);
    EPWM_setTimeBasePeriod(EPWM5_BASE, 5000);
    EPWM_setTimeBasePeriod(EPWM4_BASE, 5000);
    // set period load mode here if necessary

    EPWM_setCounterCompareShadowLoadMode(EPWM6_BASE,EPWM_COUNTER_COMPARE_A,EPWM_COMP_LOAD_ON_CNTR_PERIOD);
    EPWM_setCounterCompareShadowLoadMode(EPWM5_BASE,EPWM_COUNTER_COMPARE_A,EPWM_COMP_LOAD_ON_CNTR_PERIOD);
    EPWM_setCounterCompareShadowLoadMode(EPWM4_BASE,EPWM_COUNTER_COMPARE_A,EPWM_COMP_LOAD_ON_CNTR_PERIOD);

    EPWM_setCounterCompareValue(EPWM6_BASE,EPWM_COUNTER_COMPARE_A,0);
    EPWM_setCounterCompareValue(EPWM5_BASE,EPWM_COUNTER_COMPARE_A,0);
    EPWM_setCounterCompareValue(EPWM4_BASE,EPWM_COUNTER_COMPARE_A,0);

    EPWM_setActionQualifierAction(EPWM6_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM6_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(EPWM5_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM5_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(EPWM4_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM4_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    //固定相位差

/*
    EPWM_disablePhaseShiftLoad(EPWM4_BASE);
    EPWM_setPhaseShift(EPWM4_BASE, 0U);
    configurePhase(EPWM5_BASE, EPWM4_BASE, 120);
    configurePhase(EPWM6_BASE, EPWM4_BASE, 240);
    EPWM_enablePhaseShiftLoad(EPWM5_BASE);
    EPWM_enablePhaseShiftLoad(EPWM6_BASE);
    */


    EPWM_setSyncOutPulseMode(EPWM4_BASE, EPWM_SYNC_OUT_PULSE_ON_COUNTER_ZERO);
    EPWM_setSyncOutPulseMode(EPWM5_BASE,EPWM_SYNC_OUT_PULSE_ON_EPWMxSYNCIN);
    EPWM_setSyncOutPulseMode(EPWM6_BASE,EPWM_SYNC_OUT_PULSE_ON_EPWMxSYNCIN);


    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    EPWM_setInterruptSource(EPWM4_BASE, EPWM_INT_TBCTR_PERIOD);
    EPWM_enableInterrupt(EPWM4_BASE);
    EPWM_setInterruptEventCount(EPWM4_BASE, 1U);

    EPWM_disableADCTrigger(EPWM4_BASE, EPWM_SOC_A);
    EPWM_setADCTriggerSource(EPWM4_BASE, EPWM_SOC_A, EPWM_SOC_TBCTR_PERIOD);
    EPWM_setADCTriggerEventPrescale(EPWM4_BASE, EPWM_SOC_A, 1);

    EPWM_setTimeBaseCounterMode(EPWM6_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
    EPWM_setTimeBaseCounterMode(EPWM5_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
    EPWM_setTimeBaseCounterMode(EPWM4_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);

}



//
// Function to configure SOCs on ADCA and ADCD to be triggered by ePWM1.
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
    //
    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM4_SOCA,
                 ADC_CH_ADCIN4, acqps);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM4_SOCA,
                 ADC_CH_ADCIN5, acqps);
    ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM4_SOCA,
                 ADC_CH_ADCIN5, acqps);
    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_EPWM4_SOCA,
                 ADC_CH_ADCIN3, acqps);
    ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER4, ADC_TRIGGER_EPWM4_SOCA,
                 ADC_CH_ADCIN4, acqps);
    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER5, ADC_TRIGGER_EPWM4_SOCA,
                 ADC_CH_ADCIN5, acqps);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER6, ADC_TRIGGER_EPWM4_SOCA,
                 ADC_CH_ADCIN4, acqps);


    /*
    ADC_setupSOC(ADCD_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM1_SOCA,
                 ADC_CH_ADCIN3, acqps);
    ADC_setupSOC(ADCD_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM1_SOCA,
                 ADC_CH_ADCIN4, acqps);*/

    //
    // Selec SOC2 on ADCA as the interrupt source.  SOC2 on ADCD will end at
    // the same time, so either SOC2 would be an acceptable interrupt triggger.
    //

    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER4, ADC_SOC_NUMBER6);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER4);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER4);

}

//
// ADC A Interrupt 1 ISR
//


__interrupt void adcA1ISR(void)
{
    //
    // Store results
    //

  //  GPIO_togglePin(7);
    //adcAResult0[i] = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
    //adcAResult1[i] = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);
    //adcAResult2[i]= ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER2);

    result0 = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER0);
    result1 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);
    result2 = ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER2);
    result3 = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER3);
    result5 = ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER4);
    result6 = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER5);
    result4 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER6);
    aVhall = atten_Vhall*(result0*0.0002442*3)+0.01 ;
    bVhall = atten_Vhall*(result1*0.0002442*3)+0.01 ;
    cVhall = atten_Vhall*(result2*0.0002442*3)+0.01 ;
    Vin    = atten_Vin*(result3*0.0002442*3)+0.01;
    aVout  = atten_Vout*(result4*0.0002442*3)+0.01;
    bVout  = atten_Vout*(result5*0.0002442*3)+0.01;
    cVout  = atten_Vout*(result6*0.0002442*3)+0.01;
    /*
    adcDResult1 = ADC_readResult(ADCDRESULT_BASE, ADC_SOC_NUMBER1);
    adcDResult2 = ADC_readResult(ADCDRESULT_BASE, ADC_SOC_NUMBER2);*/

    //
    // Clear the interrupt flag
    //
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER4);

    //
    // Check if overflow has occurred
    //
    if(true == ADC_getInterruptOverflowStatus(ADCA_BASE, ADC_INT_NUMBER4))
    {
        ADC_clearInterruptOverflowStatus(ADCA_BASE, ADC_INT_NUMBER4);
        ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER4);
    }

    //
    // Acknowledge the interrupt
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP10);
}


__interrupt void epwm1ISR(void)
{

        //AD采样得到三相电流

        I_3Phase.a = _K * (aVhall - 2.5);
        I_3Phase.b = _K * (bVhall - 2.5);
        I_3Phase.c = _K * (cVhall - 2.5);

        //kf = (int)(k*Ts*f);
   //     GPIO_writePin(66,1);

   //     GPIO_togglePin(66);

        //更新w

        w=2*pi*f*k*Ts;


        V_3Phase.a = aVout;
        V_3Phase.b = bVout;
        V_3Phase.c = cVout;
        //3/2变换得到alfa beta相电流
        //I_2Phase = clarkTransformation(I_3Phase);
        V_2Phase = clarkTransformation(V_3Phase);
        //得到dq分量
        //I_dq=parkTransformation(I_2Phase,k);
        V_dq=parkTransformation(V_2Phase,k,w);

        //锁相环 过零检测调整

        PID* pPIDTheta=&pidTheta;
        f+=PIDCalc(pPIDTheta,V_dq.q,0);


        Vdq_sum += V_dq.d;
        Vdq_i++;
        if(Vdq_i==50)
        {
            Vdq_aver = 0.02*Vdq_sum;
            Vdq_i = 0;
            Vdq_sum = 0;
        }

        //与参考值对比通过PID得到占空比参考加上之前计算出来的占空比前馈
        //PID* pPIDId = &pidId;
        //PID* pPIDIq = &pidIq;
        PID* pPIDVd = &pidVd;
        PID* pPIDVq = &pidVq;

        dutyRate_dq.d=PIDCalc(pPIDVd, V_dq.d, set_volt);
        dutyRate_dq.q=PIDCalc(pPIDVq, V_dq.q, 0);

        //dq分量变为alfa  beta分量
        dutyRate_2Phase=invParkTransformation(dutyRate_dq,k,w);
        //两相占空比变为三相占空比
        dutyRate_3Phase=invClarkTransformation(dutyRate_2Phase);

        //占空比加偏置使得输出正弦波的零点对应占空比50%
        dutyRate_3Phase.a+=0.5;
        dutyRate_3Phase.b+=0.5;
        dutyRate_3Phase.c+=0.5;
        //防止占空比超过1
        if(dutyRate_3Phase.a>1){dutyRate_3Phase.a=0.99;}
        if(dutyRate_3Phase.b>1){dutyRate_3Phase.b=0.99;}
        if(dutyRate_3Phase.c>1){dutyRate_3Phase.c=0.99;}
        //防止小于0
        if(dutyRate_3Phase.a<0){dutyRate_3Phase.a=0;}
        if(dutyRate_3Phase.b<0){dutyRate_3Phase.b=0;}
        if(dutyRate_3Phase.c<0){dutyRate_3Phase.c=0;}

        //输出EPWM波
        EPWM_setCounterCompareValue(EPWM6_BASE,EPWM_COUNTER_COMPARE_A,(1-dutyRate_3Phase.a)*5000);
        EPWM_setCounterCompareValue(EPWM5_BASE,EPWM_COUNTER_COMPARE_A,(1-dutyRate_3Phase.b)*5000);
        EPWM_setCounterCompareValue(EPWM4_BASE,EPWM_COUNTER_COMPARE_A,(1-dutyRate_3Phase.c)*5000);
        k++; //更新相位
        if(k==(int)(1/(f*Ts)))
        {

            k=0;

        }


    //   EPWM_setCounterCompareValue(base,EPWM_COUNTER_COMPARE_A,duty);
    //
    // Clear INT flag for this timer
    //

    EPWM_clearEventTriggerInterruptFlag(EPWM4_BASE);

    //
    // Acknowledge interrupt group
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);

}
