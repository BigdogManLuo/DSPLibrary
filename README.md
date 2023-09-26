# DSPLibrary: A C/C++ Based Digital Signal Process Toolbox for Electronic Design Contest

Our toolbox provides digital signal processing capabilities for microcomputers. The toolbox includes a range of control units, such as PID Controllers and PR Controllers, Second-Order Generalized Integrator(SOGI), Phase-Locked Loop(PLL), and Butterworth filter. Our toolbox is implemented in C language and includes examples for BUCK, Boost, single-phase and three-phase inverter, single-phase and three-phase rectifier. The code is based on C200Ware's DSP development library. 

This library can be used for a quick start of "Ti Cup National Undergraduate Electronics Design Contest".

## Function
- :white_check_mark: ButterWorth_Filter(C/C++ & MATLAB) 巴特沃斯滤波器
- :white_check_mark: abc<->dq Transformation 静止-同步坐标系变换模块
- :white_check_mark: PID Controller PID控制器
- :white_check_mark: PR Controller 比例谐振（PR）控制器
- :white_check_mark: 3 phase and 1 phase PLL 3相和单相锁相环
- :white_check_mark: SOGI 二阶广义积分器

## Installation
To use our digital signal processing toolbox, you will need a C/C++ development environment and have installed CCS and C2000Ware. 
![](https://img.shields.io/badge/language-C%2FC%2B%2B-brightgreen)   ![](https://img.shields.io/badge/IDE-CCS-red)  ![](https://img.shields.io/badge/depend-C2000Ware-yellow)

## Usage

 1.DSPTools folder contains the package that you can include in your main.c. Please make sure to include the necessary headers and source files from the DSPTools folder to call the functions provided by our toolbox.**Do not define any parameters in the xx.c file**. For detailed usage instructions, please refer to the function documentation in the xx.c file.
For example, you can include the following header in your code:

```C
#include "DSPTools/PID.h"
```
This will allow you to call the PID controller functions from our toolbox.

2.For PR controllers, SOGI and other control units, **users are required to design their own parameters**. A MATLAB-based parameter designer is provided for this purpose. Please run the MATLAB script file to obtain the design parameters (SOS second-order matrix and scale parameters), and then define the parameter structure and parameter structure pointer in the main.c file.


3.For filter design, **we recommend using the filterDesigner script in MATLAB to design the parameters**. After designing the parameters, you can export the SOS matrix for use in the library.

4.To avoid errors, **we recommend importing all the xx.c and xx.h files in the DSPTools folder into your project at once**, as these files have mutual calls between each other.

## Test
The Test folder contains test code for parts of our toolbox. You can run the test code to verify the functionality of specific functions or modules.

## Examples

This library provides examples for various control units and electronic circuits, including:

- **Buck**: a DC-DC circuit with a buck converter
- **Electronic_Load**: a single-phase electronic load circuit, which was a competition topic in the 2022 Sichuan Province Electronic Design Competition. It involves rectification and inversion of a single-phase circuit
- **PFC_1.4**: a three-phase PWM rectification circuit, which was a topic in the 2021 National Electronic Design Competition.**Our scheme won the national first prize in this competition**.
- **Inversion_3Phase_1.5**: a three-phase inverter circuit.

Please refer to the code comments and documentation for more information on how to use our library in your projects.

## Maintainers
I would like to acknowledge my dear teammates  Xide Li and [Jiang Chen](https://github.com/Chen-JIANG-HHH) for their great contribution to the development of this digital signal processing toolbox. Xide Li provided hardware support, while Jiang Chen worked on the implementation of these toolkits on the DSP controller.

## License
![](https://img.shields.io/badge/license-MIT-blue)


