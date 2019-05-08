#include<stdio.h>
#include<string.h>
#define K 75
#define L 76
#define U 85
#define M 7 
#define N 6

void LCS_LENGTH(char *x,char *y,int (*c)[7],int (*b)[7],int m,int n)
{
	for(int i=1;i<=m;i++)
		for(int j=1;j<=n;j++){
			if(x[i-1]==y[j-1]){
				c[i][j]=c[i-1][j-1]+1;
				b[i][j]=K;
			}
			else if(c[i-1][j]>=c[i][j-1]){
				c[i][j]=c[i-1][j];
				b[i][j]=U;
			}
			else{
				c[i][j]=c[i][j-1];
				b[i][j]=L;
			}
		}
}
	
void PRINT_LCS(int (*b)[7],char *x,int i,int j)
{
	if(i==0||j==0) return;
	
	if(b[i][j]==K){
		PRINT_LCS(b,x,i-1,j-1);
		printf("%c",x[i-1]);
	}
	
	else if(b[i][j]==U)
		PRINT_LCS(b,x,i-1,j);
	else 
		PRINT_LCS(b,x,i,j-1);	
}




int main()
{
	char x[M];
	gets(x);
	char y[N];
	gets(y);
	int  c[M+1][N+1],b[M+1][N+1];
	for(int i=0;i<M+1;i++)
		for(int j=0;j<N+1;j++){
			c[i][j]=0;
			b[i][j]=0;
		}
	LCS_LENGTH(x,y,c,b,M,N);
	PRINT_LCS(b,x,7,6);
	printf("\n");
	for(int i=0;i<8;i++){
		for(int j=0;j<7;j++){
			printf("%d%c ",c[i][j],b[i][j]);
		}
		printf("\n");
    }
		
	return 0;
}


