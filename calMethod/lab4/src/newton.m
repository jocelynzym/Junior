
global x0;
global x1;
global k;
x0=0; x1=0; k=0;

while k<100000
    k
    format longE
    x0
    f(x0)
    x1=x0-f(x0)/f_1(x0);
    x0=x1;
    k=k+1;
end 

k
format long
x0
f(x0)
    



