
global x0;
global x1;
global x2;
global k;
x0=0.5; x1=1.0; x2=0.0; k=0;

while abs(f(x0))>=1.0e-10
    k
    format longE
    x0
    f(x0)
    x2=x1-f(x1)*(x1-x0)/(f(x1)-f(x0));
    x0=x1;
    x1=x2;
    k=k+1;
end 

k
format long
x0
f(x0)




