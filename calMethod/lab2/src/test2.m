%插值点
global N;
N=16;
inc=10/N;
init=pi/(2*N+2):pi/(N+1):pi*(2*N+1)/(2*N+2);
x=-5*cos(init); 
y=f(x);

%插值函数
x0=-5:0.001:5;
y0=lagrange(x,y,x0);
y1=f(x0);

%作图
figure
plot(x0,y0,x0,y1);
grid on

%求误差
x2=-5:0.02:5;
y2=f(x2);
y3=lagrange(x,y,x2);
error=abs(y3-y2);
out=max(error);
vpa(out,12)
