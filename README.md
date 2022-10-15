# Digital Signal Process Library for Electronic Design Contest


一款用于单片机、DSP控制器的数字信号处理库，其中控制单元包括PID控制器、PR控制器、SOGI二阶广义积分器、单相&三相锁相环、2阶巴特沃斯滤波的C语言实现，以及附有BUCK、Boost、单相和三相逆变、单相和三相整流器的实例（代码基于C200Ware的DSP开发库）。可用于电子设计竞赛的快速上手开发以及电子电子方向的相关项目应用。<br>
A digital signal processing (DSP) library for microcomputer or DSP controller, in which includes the PID controller, PR controller, SOGI second-order generalized integrator, single-phase & three-phase phase-locked loop(PLL), and second-order Butterworth filter. C language implementation, and with examples of BUCK, Boost, single-phase and three-phase inverter, single-phase and three-phase rectifier (based on C200Ware's DSP development library). It can be used for the quick development of electronic design competitions and related projects or applications in Power Electronics.

# Documention

## 1. DSPTools
(1) 使用时请在项目中包含xx.h文件，并在main.c文件中定义参数并初始化，xx.c文件中不做定义。具体使用方法参考xx.c文件中的函数文档。<br>
(1) When using the library, **please include the xx.h file in the project**, and define & initialize the related variables in the main.c file, and do not define the them in the xx.c file. For specific usage, please refer to the function documentation in the xx.c file. <br>
(2) 对于PR控制器、SOGI等控制单元，要求用户自行设计参数，参数设计器采用MATLAB编写，请运行MATLAB脚本文件得到设计参数（SOS二阶节矩阵和scale参数），再在main.c文件中定义参数结构体和参数结构体指针。<br>
(2) For control units such as PR controller or SOGI, users are required to design their parameters. The parameter designer is written by MATLAB. Please run the MATLAB script file to get the these parameters (SOS second-order section matrix and scale parameters), and then in main.c, the parameter structure and the parameter structure pointer are defined in this file. <br>
(3) 对于滤波器的设计，我们建议在MATLAB中运行filterDesigner脚本，设计好参数之后可导出SOS矩阵。<br>
(3) For the design of filter, we recommend running the filterDesigner script in MATLAB. After designing the parameters, the SOS matrix can be exported. <br>
(4) **请一次性将DSPToos中所有xx.c和xx.h文件导入工程中，因为文件之间存在相互调用，以免报错**<br>
(4) **Please import all xx.c and xx.h files in the DSPTools folder into the project at one time, because there are mutual calls between the files to avoid errors**
## 2. Test
对DSPTool代码的C语言仿真测试
C language simulation test for DSPTool code
## 3.Example
Buck：Buck 直流斩波DC-DC电路<br>
Electronic_Load:单相电子负载，2022年四川省电子设计竞赛赛题，先单相整流再单相逆变<br>
PFC_1.4:三相PWM整流，2021年全国赛B题<br>
Inversion_3Phase_1.5：三相逆变<br>
Buck: Buck DC-DC chopper<br>
Electronic_Load: Single-phase electronic load, the subject of the 2022 Sichuan Electronic Design Competition, firstly single-phase rectification and then single-phase inverter<br>
PFC_1.4: Three-phase PWM rectification, question B of the 2021 national competition<br>
Inversion_3Phase_1.5: Three-phase inverter
