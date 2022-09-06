%PR控制器设计

Kp=1;
Kr=1;
wc=2*pi*0.2;
w0=2*pi*50;
Ts=1e-4; %采样周期
%构建传递函数
PRs=tf([2*wc,0],[1,2*wc,w0^2])
%Z变换
PRz = c2d(PRs,Ts,'tustin')
%构建SOS矩阵
SOS=10000*[vpa(PRz.Numerator,15),vpa(PRz.Denominator,15)]
%复制到剪切板1.2561692592100306238231399724725,0,-1.2561692592100306238231399724725,10000.0,-19987.621731190576834791272631264,9997.4876614815755271337138765375
string(SOS(1))
strSOS=string(SOS);
strCopy=strcat(strSOS(1),",",strSOS(2),",",strSOS(3),",",strSOS(4),",",strSOS(5),",",strSOS(6));
clipboard("copy",strCopy)
