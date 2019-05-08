#include<iostream>
#include<ctime>
#include<cmath>
#include<algorithm>
#include<iomanip>

using namespace std;

#define NO_DISTANCE 10000000

//point 
typedef struct POINT
{
	float x,y;
}POINT;



//read file
void ReadFile(POINT *points,int length,FILE *fpread)
{
	//if(fpread==NULL) return 0;
	float a[length];
	float b[length];
	int i=0;
	while(!feof(fpread)){
	 	fscanf(fpread,"%f,%f;",&a[i],&b[i]);
	 	i++;
	 }
	for(int i=0;i<length;i++)
	{
		points[i].x=a[i];
		points[i].y=b[i];
	}
	
	fclose(fpread);
}



//calculate distance
float Distance(POINT a,POINT b)
{
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}


//compare 2 points by x value
bool CmpX(POINT a,POINT b)
{
	return a.x<b.x;
}


//compare 2 points by y value
bool CmpY(POINT a,POINT b)
{
	return a.y<b.y;
}

//get the closest 2 points
float ClosestPair(POINT points_x[],POINT points_y[],int length,POINT &a,POINT &b)
{
	float distance;
	float d1,d2;
	int i=0,j=0,k=0;
	POINT a1,b1,a2,b2;
	
	if(length<2)
	{
		printf("WARNING!");
		return NO_DISTANCE;
	}
	
	if(length==2)
	{
		a=points_x[0];
		b=points_x[1];
		distance=Distance(points_x[0],points_x[1]);
		return distance;
	}
	
	if(length==3)
	{
		float distance1,distance2,distance3;
		distance1=Distance(points_x[0],points_x[1]);
		distance2=Distance(points_x[0],points_x[2]);
		distance3=Distance(points_x[1],points_x[2]);
		if(distance1>distance2&&distance1>distance3){
			a=points_x[0];
			b=points_x[1];
			distance=distance1;
		}
		else if(distance2>distance1&&distance2>distance3){
			a=points_x[0];
			b=points_x[2];
			distance=distance2;
		}
		else{
			a=points_x[1];
			b=points_x[2];
			distance=distance3;
		}
	   return distance;
	}
	
	else
	{
		POINT *pts1=new POINT[length];
		POINT *pts2=new POINT[length];
		POINT *pts3=new POINT[length];
		POINT *pts4=new POINT[length];
		 
		
		float mid=points_x[(length+1)/2-1].x;
		
		for(i=0;i<length/2;i++)
		{
			pts1[i]=points_x[i];
		}
		
		for(j=0,i=length/2;i<length;i++)
		{
			pts2[j++]=points_x[i];
		}
		
			for(i=0,j=0,k=0;i<length;i++)
		{
			if(points_y[i].x<mid)
				pts3[j++]=points_y[i];
			else if(points_y[i].x>=mid)
			{
				pts4[k++]=points_y[i];
				if(k>=length-length/2)
				{
					for(int t=i+1;t<length;t++)
						pts3[j++]=points_y[t];	
					break;
				}
			}	
		}
		
		
		
		d1=ClosestPair(pts1,pts3,length/2,a1,b1);
		d2=ClosestPair(pts2,pts4,length-length/2,a2,b2);
		
		if(d1<d2)  {distance=d1; a=a1; b=b1;}
		else 	   {distance=d2; a=a2; b=b2;}
		
		POINT *pts5=new POINT[length];
		for(i=0,k=0;i<length;i++)
		{
			if(abs(points_y[i].x-mid)<=distance) pts5[k++]=points_y[i];
		}
		
		for(i=0;i<k;i++)
			for(j=i+1;j<=i+7&&j<k;j++)
			{
				if(Distance(pts5[i],pts5[j])<distance)
				{
					distance=Distance(pts5[i],pts5[j]);
					a=pts5[i];
					b=pts5[j];
					//printf("mid area\n");
				}
			}
	}
	
	return distance;	
}


void GetOutcome(FILE *fpread,int N)
{
	POINT a,b;
	float distance;
	
	POINT *points=new POINT[N];
	POINT *points_x=new POINT[N];
	POINT *points_y=new POINT[N];
	
	ReadFile(points,N,fpread);
	 
	 
	for(int i=0;i<N;i++)
	{
		points_x[i]=points[i];
		points_y[i]=points[i];
	 } 
	
	sort(points_x,points_x+N,CmpX);   //pre_sort
	sort(points_y,points_y+N,CmpY);
	
	distance=ClosestPair(points_x,points_y,N,a,b);
	cout<<endl<<endl<<"OUTCOME:"<<endl;
    cout << setiosflags( ios::fixed)<< setprecision( 2 );
	cout<<endl<<"Closest Pair:"<<"("<<a.x<<","<<a.y<<")and"<<"("<<b.x<<","<<b.y<<")"<<endl<<"Minimum Distance:"<<distance<<endl<<endl;
    
}

int main()
{
	FILE *fpread1=fopen("test1.txt","r");
	FILE *fpread2=fopen("test2.txt","r");
	FILE *fpread3=fopen("test3.txt","r");
	 
	GetOutcome(fpread1,11);
	GetOutcome(fpread2,6);
	GetOutcome(fpread3,6);	 
	 
	system("pause");
}









