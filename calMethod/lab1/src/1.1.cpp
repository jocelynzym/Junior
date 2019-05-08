#include<stdio.h>
#include<math.h>

int main()
{
	float x[10]={
		pow(8,-1),pow(8,-2),pow(8,-3),pow(8,-4),pow(8,-5),
		pow(8,-6),pow(8,-7),pow(8,-8),pow(8,-9),pow(8,-10)
	};
	
	float xf[10],xs[10];
//计算方法1 
//	float temp;
//	for(int i=0;i<10;i++)
//	{
//		temp=sqrt(x[i]*x[i]+4);
//		xf[i]=temp-2;
//		xs[i]=(x[i]*x[i])/(temp+2);
//	}
	
//计算方法2	
		for(int i=0;i<10;i++)
	{
		xf[i]=sqrt(x[i]*x[i]+4)-2;
		xs[i]=(x[i]*x[i])/(sqrt(x[i]*x[i]+4)+2);
	}

//计算方法3 
//	double temp;
//	for(int i=0;i<10;i++)
//	{
//		temp=sqrt(x[i]*x[i]+4);
//		xf[i]=temp-2;
//		xs[i]=(x[i]*x[i])/(temp+2);
//	}
	
	for(int i=0;i<10;i++){
		printf("%.11e\t",x[i]);
		printf("%.11e\t",xf[i]);
		printf("%.11e",xs[i]);
		printf("\n");
	}


	return 0;
}
