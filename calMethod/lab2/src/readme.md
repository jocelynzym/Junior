## 代码说明

**1.定义函数y=1/(4+x+x^2),将其保存在f.m文件中**

```matlab
function y = f(x)
y= 1./(4+x+x.^2);
end
```

**2.定义拉格朗日插值函数，将其保存在lagrange.m文件中**

```matlab
function yy = lagrange(x1,y1,xx)
syms x
n=length(x1);
for i=1:n
    t=x1;
    t(i)=[];
    L(i)=prod((x-t)./(x1(i)-t));
end
u=sum(L.*y1);
p=simplify(u)
yy=subs(p,x,xx);
end
```

**3.建立测试程序，将其保存在test.m文件中**

```matlab
%插值点
global N;
N=16;
inc=10/N;
   %插值方法1
   x=-5:inc:5;
   %插值方法2
   %init=pi/(2*N+2):pi/(N+1):pi*(2*N+1)/(2*N+2);
   %x=-5*cos(init); 
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

```

