function y = error_T(a,b,n)
syms x
yy=diff(fun(x),2);
theta=fminbnd(yy,a,b);
y=theta*(a-b)*(a-b)*(a-b)/(12*n*n);
end