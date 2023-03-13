# Digital Signal Process Library for Electronic Design Contest

A digital signal processing library for single-chip microcomputer and DSP controller, in which the control unit includes PID controller, PR controller, SOGI second-order generalized integrator, single-phase & three-phase phase-locked loop, and second-order Butterworth filter. C language implementation, and with examples of BUCK, Boost, single-phase and three-phase inverter, single-phase and three-phase rectifier (the code is based on C200Ware's DSP development library). It can be used for the rapid development of electronic design competitions and related project applications in the direction of electronic electronics.

# Documention

## 1. DSPTools
(1) When using, please include the xx.h file in the project, and define and initialize the parameters in the main.c file, and do not define it in the xx.c file. For specific usage, please refer to the function documentation in the xx.c file. <br>
(2) For control units such as PR controller and SOGI, users are required to design their own parameters. The parameter designer is written in MATLAB. Please run the MATLAB script file to get the design parameters (SOS second-order section matrix and scale parameters), and then in main.c The parameter structure and the parameter structure pointer are defined in the file. <br>
(3) For filter design, we recommend running the filterDesigner script in MATLAB. After designing the parameters, the SOS matrix can be exported. <br>
(4) **Please import all xx.c and xx.h files in DSPToos into the project at one time, because there are mutual calls between the files to avoid errors**
## 2. Test
C language simulation test for DSPTool code
## 3.Example
Buck: Buck DC chopper DC-DC circuit<br>
Electronic_Load: Single-phase electronic load, the subject of the 2022 Sichuan Electronic Design Competition, first single-phase rectification and then single-phase inverter<br>
PFC_1.4: Three-phase PWM rectification, question B of the 2021 national competition<br>
Inversion_3Phase_1.5: Three-phase inverter
