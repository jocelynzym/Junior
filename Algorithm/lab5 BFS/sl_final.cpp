#include<iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include<stack>
#include<time.h>
#include<map>
#include<fstream>
#include<sstream>
using namespace std;


map<int,vector<int> > lxc;

void readfile()
{
    ifstream fin;
    int i=0,j=0,countline=0,countpoint=0;
    char comma;
    map<int,vector<int> >::iterator iter;
//     fin.open("twitter_small.txt",ifstream::in);
     fin.open("twitter_large.txt",ifstream::in);
     string s;
	 while(getline(fin,s))
	 {
	 	istringstream ss(s);
	 	ss>>i;
	 	ss>>comma;
	 	ss>>j;
	 	lxc[i].push_back(j);
	 	countline++;
	  } 
	  
	  for(iter = lxc.begin();iter!=lxc.end();iter++)
	     {
		   iter->second.push_back(0);
           countline++;
		 }
           
       cout<<">>>读文件"<<endl<<"顶点:"<<lxc.size()<<",边："<<countline<<endl;  
         
}

void bfs(int v)
{
    int a;
    vector<int>::iterator it;
    map<int,vector<int> >::iterator iter;
    lxc[v].push_back(1);
    queue<int> M;
    M.push(v);
  
    while (!M.empty())
    {
        v = M.front();
        M.pop();
        
        for (it = lxc[v].begin(); *it != 0; it++)
        {
        	a=*it;
        	if(lxc.find(a)!= lxc.end())
        	{
			
            	if (0==lxc[a].back()) 
            	{
                	M.push(a);
                	lxc[a].push_back(1);
            	}
        }
        }
    }
}

int main()
{
    clock_t begin,end;
    begin=clock();
    readfile();
    end=clock();
    cout <<"读文件:" << end-begin <<"ms" << endl;
    map<int,vector<int> >::iterator it=lxc.begin();
    int v=it->first;
    begin=clock(); 
    bfs(v);
    end=clock();
    cout <<"BFS:" << end-begin<<"ms" <<endl;
    return 0;
}

