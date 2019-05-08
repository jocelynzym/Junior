#include<stdio.h>
#include<math.h>

int main()
{
	double x[5]=
	{4040.045551380452, 
	-2759471.276702747, 
	-31.64291531266504,  
	2755462.874010974,  
	0.0000557052996742893
	};
	
	double result1,result2,result3,t[4];
	//方法a
	for(int i=0;i<4;i++) t[i]=0;
	t[0]=x[0]+x[1];
	t[1]=t[0]+x[2];
	t[2]=t[1]+x[3];
	t[3]=t[2]+x[4];
	for(int i=0;i<4;i++)
	{
		printf("%7e\t",t[i]);
	}
	printf("\n");
	
	//方法b
	for(int i=0;i<4;i++) t[i]=0;
	t[0]=x[3]+x[4];
	t[1]=t[0]+x[2];
	t[2]=t[1]+x[1];
	t[3]=t[2]+x[0];
	for(int i=0;i<4;i++)
	{
		printf("%.20e\t",t[i]);
	}
	printf("\n");
	
	//方法c
		for(int i=0;i<4;i++) t[i]=0;
	t[0]=x[3]+x[0];
	t[1]=t[0]+x[4];
	t[2]=x[2]+x[1];
	t[3]=t[2]+t[1];
	for(int i=0;i<4;i++)
	{
		printf("%.20e\t",t[i]);
	}
	printf("\n");
	
	return 0;
}
