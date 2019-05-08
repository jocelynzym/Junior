%区间数
global n;
global a;
global b;
global h;
n=2;a=1;b=6;

temp=1.8250e+00;
while n<=24
h=(b-a)/n;
%中间变量
sum=0
for i=1:(n-1)
    sum=sum+fun(a+i*h);
end
%计算复化积分
T_f=h*(0.5*fun(a)+sum+0.5*fun(b));
%求误差
error=abs(cos(1)-cos(6)-T_f);
p=n/2
d=log(temp/error);
temp=error;
format short
error
d


n=n+2;
end










