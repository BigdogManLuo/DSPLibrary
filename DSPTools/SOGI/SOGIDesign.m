%SOGI控制器设计

k=1;%系数
f=50;%工频
w=2*pi*f;
%建立传递函数
SOGI_s=tf(k*w^2,[1,k*w,w^2])
%Z变换
SOGI_z=c2d(SOGI_s,1e-4,'tustin')
%构造SOS矩阵
SOS=10000*[vpa(SOGI_z.Numerator,15),vpa(SOGI_z.Denominator,15)]
%将SOS矩阵复制到剪切板
string(SOS(1))
strSOS=string(SOS);
strCopy=strcat(strSOS(1),",",strSOS(2),",",strSOS(3),",",strSOS(4),",",strSOS(5),",",strSOS(6));
clipboard("copy",strCopy)
