%区间数
global n;
global k;
global a;
global b;
global h;
k=1;n=2;a=1;b=6;

temp=1.8250e+00;
while k<=12
n=2*k;
h=(b-a)/n;
%中间变量
sum1=0;
for i=0:(k-1)
    sum1=sum1+fun(a+(2*i+1)*h);
end
sum2=0;
for i=1:(k-1)
    sum2=sum2+fun(a+2*i*h);
end
%计算复化积分
T_f=h*(fun(a)+4*sum1+2*sum2+fun(b))/3;
%求误差
error=abs(cos(1)-cos(6)-T_f);
k
format shortE
error
d=log(temp/error);
temp=error;
format short
error
d


k=k+1;
end










