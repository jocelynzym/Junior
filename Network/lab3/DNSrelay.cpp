#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<string>
#include<fstream>
#include<iostream>
#include<iomanip>
#include <winsock2.h> 
#include <windows.h> 
using namespace std;


#pragma  comment(lib,"ws2_32.lib") //链接Ws2_32.lib库，不必在setting里设置

#define DEF_DNS_ADDRESS "202.38.64.56" //外部DNS服务器地址
#define LOCAL_ADDRESS "114.214.193.126"       //本地DNS服务器地址
#define DNS_PORT 53                     //进行DNS服务的53号端口 
#define BUF_SIZE 512
#define LENGTH 65
#define AMOUNT 300
#define NOTFOUND -1
#define FOUND 1

//DNS报文首部
typedef struct DNSHeader
{
    unsigned short ID;
    unsigned short Flags;
    unsigned short QuestNum;
    unsigned short AnswerNum;
    unsigned short AuthorNum;
    unsigned short AdditionNum;
} DNSHDR, *pDNSHDR; 

typedef struct
{
    char addr[16];
} ip_addr;

//DNS域名解析表结构
typedef struct translate
{
	string IP;						//IP地址
	string domain;					//域名
} Translate;




//ID转换表结构
typedef struct IDChange
{
	unsigned short oldID;			//原有ID
	bool done;						//标记是否完成解析
	SOCKADDR_IN client;				//请求者套接字地址
	int joinTime;                   //加入转换表的时刻 
	char urlName[LENGTH];           //客户询问的url名字 
	int offset;                     //客户发送报文的字节数 
} IDTransform;

Translate DNS_table[AMOUNT];		//DNS域名解析表
IDTransform IDTransTable[AMOUNT];	//ID转换表
int IDcount = 0;					//转换表中的条目个数
char url[LENGTH];					//域名
char recvbuf_temp[BUF_SIZE];		//数据报内容(打印用) 
char send_addr[LENGTH],rcv_addr[LENGTH]; //接收和发送数据包的地址(打印用)
unsigned short send_port,rcv_port;       //接收和发送数据包的端口(打印用)
SYSTEMTIME sys;                     //系统时间
int Day, Hour, Minute, Second, Milliseconds;//保存系统时间的变量
int Day1, Hour1, Minute1, Second1;//保存系统时间的变量
int Day2, Hour2, Minute2, Second2;//保存系统时间的变量
int Day3, Hour3, Minute3, Second3;//保存系统时间的变量

/*功能函数声明*/
//begin:------------------------------------------ 
int GetTable(char *tablePath);          //函数1:获取域名解析表
void GetUrl(char *recvbuf,int recvnum); //函数2: 获取DNS请求中的域名
int IsFind(char *url,int num);          //函数3：判断是否在表中找到DNS请求中的域名，找到返回下标
                                        //函数4：将请求ID转换为新的ID，并将信息写入ID转换表中
unsigned short RegisterNewID(unsigned short oID, SOCKADDR_IN temp, bool ifdone); 
void DisplayInfo(unsigned short newID, int find); //函数5：打印 时间 newID 功能 域名 IP


//end-----------------------------------------------------




//函数1:获取域名解析表
int GetTable(char *tablePath)
{
	int i=0,j,pos=0;
	string table[AMOUNT];
	
	ifstream infile(tablePath,ios::in);
	if(!infile) 
	{
		cerr << "Open " << tablePath << " error!" <<endl;
		exit(1);
	}
	
	while(getline(infile,table[i])&&i<AMOUNT)
		i++;
	
	if(i==AMOUNT-1)
		cout<<"The DNS table memory is full."<<endl;
	
	for(j=0;j<i-1;j++)
	{
		pos=table[j].find(' ');
		if(pos>table[j].size())
			cout << "The record is not in a correct format. " << endl;
		else 
		{
			DNS_table[j].IP = table[j].substr(0, pos);
			DNS_table[j].domain = table[j].substr(pos+1);
		}	
	}
	
	infile.close();
	cout << "Load records succeed. " << endl;
	return i-1;
} 


//函数2: 获取DNS请求中的域名
void GetUrl(char *recvbuf, int recvnum)
{
	char urlname[LENGTH];
	int i=0,j,k=0;
	
	memset(url,0,LENGTH);
	memcpy(urlname,&(recvbuf[sizeof(DNSHDR)]),recvnum-16);
	
	int len = strlen(urlname);
	
	//域名转换
	while (i < len) 
	{
		if (urlname[i] > 0 && urlname[i] <= 63)
			for (j = urlname[i], i++; j > 0; j--, i++, k++)
				url[k] = urlname[i];
		
		if (urlname[i] != 0) {
			url[k] = '.';
		    k++;
		}
	}
	
	url[k] = '\0';
}



//函数3：判断是否在表中找到DNS请求中的域名，找到返回下标
int IsFind(char* url, int num)
{
	int find = NOTFOUND;
	char* domain;
	for (int i = 0; i < num; i++) {
		domain = (char *)DNS_table[i].domain.c_str();
		if (strcmp(domain, url) == 0) {	//找到
			find = i;
			break;
		}
	}
	return find;
}


//函数4：将请求ID转换为新的ID，并将信息写入ID转换表中
unsigned short RegisterNewID (unsigned short oID, SOCKADDR_IN temp, bool ifdone,int jTime)
{
	srand(time(NULL));
	IDTransTable[IDcount].oldID = oID;
	IDTransTable[IDcount].client = temp;
	IDTransTable[IDcount].done  = ifdone;
	IDTransTable[IDcount].joinTime = jTime; 
	IDcount++;

	return (unsigned short)(IDcount-1);	//以表中下标作为新的ID
}



//函数5：打印本地构造回复报文的信息(调试等级1,2,3) 
void DisplayInfo(unsigned short newID, int find,int level)
{

    //调试级别为1
	if(level==1)
	{
		//在表中没有找到DNS请求中的域名
		if (find == NOTFOUND) 
		{   
			//中继功能
			cout.setf(ios::left);
			cout << setiosflags(ios::left) << setw(6) << setfill(' ') << "中继";
			cout << "    ";
			//打印域名
			cout.setf(ios::left);
			cout << setiosflags(ios::left) << setw(20) << setfill(' ') << url;
			cout << "    ";
		}

		//在表中找到DNS请求中的域名
		else {
	  		if(DNS_table[find].IP== "0.0.0.0")  //不良网站拦截
			{
				//屏蔽功能
				cout.setf(ios::left); 
		    	cout << setiosflags(ios::left) << setw(6) << setfill(' ') << "屏蔽";
		    	cout << "    ";
				//打印域名(加*)
				cout.setf(ios::left); 
		    	cout << "*" << setiosflags(ios::left) << setw(19) << setfill(' ') << url;
		    	cout << "    ";
			}

			//检索结果为普通IP地址，则向客户返回这个地址
			else
			{
				//打印域名
				cout.setf(ios::left);
		    	cout <<"域名:"<< setiosflags(ios::left) << setw(19) << setfill(' ') << url;
		    	cout << "    ";
				//打印IP
				cout.setf(ios::left);
		    	cout <<"ip地址:" <<setiosflags(ios::left) << setw(20) << setfill(' ') << DNS_table[find].IP << endl;
			}
		}
	} 

	//调试级别为2 
	else if(level==2)
	{
		//打印时间
		GetLocalTime( &sys );
	
		if(sys.wMilliseconds >= Milliseconds)
		{
	    cout << setiosflags(ios::right) << setw(6) << setfill(' ') << "时间:"<<(((sys.wDay - Day) * 24 + sys.wHour - Hour) * 60 + sys.wMinute - Minute) * 60 + sys.wSecond - Second;//设置宽度为7，right对齐方式
	   	 cout << '.' << setiosflags(ios::right) << setw(3) << setfill('0') << sys.wMilliseconds - Milliseconds;
		}
		else {
			cout << setiosflags(ios::right) << setw(5) << setfill(' ') << "时间:"<< (((sys.wDay - Day) * 24 + sys.wHour - Hour) * 60 + sys.wMinute - Minute) * 60 + sys.wSecond - Second - 1;//设置宽度为7，right对齐方式
	    	cout << '.' << setiosflags(ios::right) << setw(3) << setfill('0') << 1000 + sys.wMilliseconds - Milliseconds;
		}
		cout << " ";

		//打印转换后新的ID
		cout.setf(ios::left);
		cout << "ID: "<< newID;
		cout << "  ";

		//在表中没有找到DNS请求中的域名
		if (find == NOTFOUND) 
		{   
			//中继功能
			cout.setf(ios::left);
			cout << setiosflags(ios::left) << setw(6) << setfill(' ') << "中继";
			cout << "    ";
			//打印域名
			cout.setf(ios::left);
			cout << setiosflags(ios::left) << setw(20) << setfill(' ') << url;
			cout << endl;
			
		}

		//在表中找到DNS请求中的域名
		else {
	  		if(DNS_table[find].IP == "0.0.0.0")  //不良网站拦截
			{
				//屏蔽功能
				cout.setf(ios::left); 
		    	cout << setiosflags(ios::left) << setw(6) << setfill(' ') << "屏蔽";
		    	cout << "    ";
				//打印域名(加*)
				cout.setf(ios::left); 
		    	cout << "*" << setiosflags(ios::left) << setw(19) << setfill(' ') << url;
		    	cout << endl;	
			}

			//检索结果为普通IP地址，则向客户返回这个地址
			else
			{
				//打印域名
				cout.setf(ios::left);
		    	cout  <<setiosflags(ios::left) << setw(15) << setfill(' ') <<"域名:"<< url;
		    	
				
				//客户端ip地址
				IDTransTable[IDcount].client.sin_addr;
				char ipTemp[40];
				strcpy(ipTemp,inet_ntoa(IDTransTable[newID].client.sin_addr));
				cout  <<setiosflags(ios::left) << setw(15) << setfill(' ')<<"ip地址:" << ipTemp<<endl;
			
				
			}
		}
	}
	
	else if(level==3)
	{
		//在表中没有找到DNS请求中的域名
		if (find == NOTFOUND) 
		{   
			//中继功能
			cout.setf(ios::left);
			cout << setiosflags(ios::left) << setw(6) << setfill(' ') << "中继";
			cout << "    ";
			
			//打印域名
			cout.setf(ios::left);
			cout << setiosflags(ios::left) << setw(20) << setfill(' ') << url;
			cout<< endl;
				
		}

		//在表中找到DNS请求中的域名
		else {
	  		if(DNS_table[find].IP == "0.0.0.0")  //不良网站拦截
			{
				//屏蔽功能
				cout.setf(ios::left); 
		    	cout << setiosflags(ios::left) << setw(6) << setfill(' ') << "屏蔽";
		    	cout << "    ";
				//打印域名(加*)
				cout.setf(ios::left); 
		    	cout << "*" << setiosflags(ios::left) << setw(19) << setfill(' ') << url;
		    	cout << endl;
			}

			//检索结果为普通IP地址，则向客户返回这个地址
			else
			{
				//打印域名
				cout.setf(ios::left);
		    	cout <<"域名:"<< setiosflags(ios::left) << setw(19) << setfill(' ') << url;
		    	cout << " ";
				//打印IP
				cout.setf(ios::left);
		    	cout <<"ip地址:"<< setiosflags(ios::left) << setw(20) << setfill(' ') << DNS_table[find].IP << endl;
		    	
		    	//打印数据报内容
				cout<<endl;
				cout<<"数据报原始内容:"<<endl;
				for(int i=0;i<BUF_SIZE;i++) printf("%x",recvbuf_temp[i]);
				cout <<endl;
			
			
				//打印接收和发送数据包的地址及端口 
				cout<<endl;
				cout <<"发送地址: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << send_addr<<endl;
				cout <<"发送端口: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << send_port<<endl;
				cout <<"接收地址: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << rcv_addr<<endl;
				cout <<"接收端口: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << rcv_port<<endl;
		    	
			}
		}
	}
	
	
}


//函数6:读取响应报文中url 
void readurl(char* buf, char* dest)
{
    int len = strlen(buf);
    int i = 0, j = 0, k = 0;
    while (i < len)
    {
        if (buf[i] > 0 && buf[i] <= 63) //如果是个计数
        {
            for (j = buf[i], i++; j > 0; j--, i++, k++) //j是计数是几，k是目标位置下标，i是报文里的下标
                dest[k] = buf[i];
        }

        if (buf[i] != 0)    //如果没结束就在dest里加个'.'
        {
            dest[k] = '.';
            k++;
        }
    }
    dest[k] = '\0';
}



//函数7：打印外部DNS响应报文的相关信息(调试级别1&3)
void DisplayAnswer(int m,int level,char *recvbuf)
{
	
	char url_r[65];
    int length = -1;

    length = IDTransTable[m].offset;
    int nquery = ntohs(*((unsigned short*)(recvbuf + 4))), nresponse = ntohs(*((unsigned short*)(recvbuf + 6)));    //问题个数；回答个数
    char* p = recvbuf + 12; //跳过DNS包头的指针
    ip_addr ip;
    int ip1, ip2, ip3, ip4;

        //读取每个问题里的查询url
        for (int i = 0; i < nquery; ++i)
        {
            readurl(p, url_r);    //这么写url里只会记录最后一个问题的url
            while (*p > 0)  	//读取标识符前的计数跳过这个url
                p += (*p) + 1;
            p += 5; //跳过url后的信息，指向下一个报文
        }

        if (nresponse > 0 && level >= 1)
            printf("查询域名: %s  ", url);
        //分析回复
        //具体参考DNS回复报文格式
        for (int i = 0; i < nresponse; ++i)
        {
            if ((unsigned char)*p == 0xc0) //是指针就跳过
                p += 2;
            else
            {
                //根据计数跳过url
                while (*p > 0)
                    p += (*p) + 1;
                ++p;    //指向后面的内容
            }
            unsigned short resp_type = ntohs(*(unsigned short*)p);  //回复类型
            p += 2;
            unsigned short resp_class = ntohs(*(unsigned short*)p); //回复类
            p += 2;
            unsigned short high = ntohs(*(unsigned short*)p);   //生存时间高位
            p += 2;
            unsigned short low = ntohs(*(unsigned short*)p);    //生存时间低位
            p += 2;
            int ttl = (((int)high) << 16) | low;    //高低位组合成生存时间
            int datalen = ntohs(*(unsigned short*)p);   //后面数据长度
            p += 2;
            
                
            
            if (resp_type == 1) //是A类型，回复的是url的ip
            {
                memset(ip.addr, 0, sizeof(ip.addr));
                //读取4个ip部分
                ip1 = (unsigned char)*p++;
                ip2 = (unsigned char)*p++;
                ip3 = (unsigned char)*p++;
                ip4 = (unsigned char)*p++;
				
                printf("ip地址: %d.%d.%d.%d\n", ip1, ip2, ip3, ip4);
                if (level == 3)
            	{
            	printf("type:%d  class:%d  ttl:%d\n", resp_type, resp_class, ttl);
				}
			
                break;
            }
            else p += datalen;  //直接跳过
        }

	
	if(level==3)
	{
		//打印接收和发送数据包的地址及端口 
		cout <<"发送方地址: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << send_addr<<endl;
		cout <<"发送方端口: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << send_port<<endl;
		cout <<"接收方地址: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << rcv_addr<<endl;
		cout <<"接收方端口: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << rcv_port<<endl;
		cout<<"数据报原始内容:"<<endl;
		for(int i=0;i<BUF_SIZE;i++) printf("%x",recvbuf_temp[i]);
		cout <<endl; 
	}
	
} 






//主函数
int main(int argc, char** argv)
{
	WSADATA wsaData; 
    SOCKET  socketServer, socketLocal;				//本地DNS和外部DNS两个套接字
    SOCKADDR_IN serverName, clientName, localName;	//本地DNS、外部DNS和请求端三个网络套接字地址
    char sendbuf[BUF_SIZE];
    char recvbuf[BUF_SIZE]; 
    char tablePath[100];
    char outerDns[16];
    int iLen_cli, iSend, iRecv;
    int num;
    int jTime;
    int level = atoi(argv[1]);
    cout<<"level="<<level<<endl;
//    strcpy(outerDns, DEF_DNS_ADDRESS);
//	strcpy(tablePath, "example.txt");
    
    //调试级别1
	if (argc == 2) {
		strcpy(outerDns, DEF_DNS_ADDRESS);
		strcpy(tablePath, "example.txt");
	}
    //调试级别2
	else if (argc == 4) {
		strcpy(outerDns, argv[2]);
		strcpy(tablePath, argv[3]);
	}
    //调试级别3
	else if (argc == 3) {
		strcpy(outerDns, argv[2]);
		strcpy(tablePath, "example.txt");
	}

	num = GetTable(tablePath);						//获取域名解析表
    
	//保存系统的时间
	GetLocalTime(&sys);
	Day          = sys.wDay;
    Hour         = sys.wHour;
	Minute       = sys.wMinute;
	Second       = sys.wSecond;
	Milliseconds = sys.wMilliseconds;
	Day2          = sys.wDay;
    Hour2         = sys.wHour;
	Minute2       = sys.wMinute;
	Second2       = sys.wSecond;
	

	for (int i=0; i < AMOUNT; i++) {				//初始化ID转换表
		IDTransTable[i].oldID = 0;
		IDTransTable[i].done  = FALSE;
		IDTransTable[i].joinTime = 0; 
		IDTransTable[i].offset = 0; 
		memset(&(IDTransTable[i].client), 0, sizeof(SOCKADDR_IN));
		memset(&(IDTransTable[i].urlName), 0, LENGTH*sizeof(char));
	}

    WSAStartup(MAKEWORD(2,2), &wsaData);			//初始化ws2_32.dll动态链接库

	//创建本地DNS和外部DNS套接字
    socketServer = socket(AF_INET, SOCK_DGRAM, 0);
    unsigned long ul=1;
    int ret;
    ret=ioctlsocket(socketServer,FIONBIO,(unsigned long *)&ul);
	socketLocal = socket(AF_INET, SOCK_DGRAM, 0);

	//设置本地DNS和外部DNS两个套接字
	localName.sin_family = AF_INET;//地址族 
	localName.sin_port = htons(DNS_PORT);//16位端口号 
	localName.sin_addr.s_addr = inet_addr(LOCAL_ADDRESS);//32位IPv4地址 

	serverName.sin_family = AF_INET;
	serverName.sin_port = htons(DNS_PORT);
	serverName.sin_addr.s_addr = inet_addr(outerDns);

	//绑定本地DNS服务器地址
	if (bind(socketLocal, (SOCKADDR*)&localName, sizeof(localName))) 
	{
		cout << "Binding Port 53 failed." << endl;
		exit(1);
	}
	else
		cout << "Binding Port 53 succeed." << endl;
   
	//本地DNS中继服务器的具体操作
	while (1) 
	{
		iLen_cli = sizeof(clientName);
        memset(recvbuf, '\0', BUF_SIZE);
        
        //每19秒处理一次ID转换表中的超时情况
		GetLocalTime(&sys);
	    int countTime=(((sys.wDay - Day) * 24 + sys.wHour - Hour) * 60 + sys.wMinute - Minute) * 60 + sys.wSecond - Second;
		int dealTime=countTime%20;
		if(dealTime==19)
		{
			for(int i=0;i<IDcount;i++)
			{
				if(IDTransTable[i].done==FALSE&&(countTime-IDTransTable[i].joinTime>=10))
				
				cout<<"query from client【"<< inet_ntoa(IDTransTable[i].client.sin_addr)<<"】 has run out of time"<<endl;
			} 
		}
        
        
        
        //判断是否收到外部DNS的反馈
        long int count=0;
        while(count<3)
		{
			//接收来自外部DNS服务器的响应报文
			iRecv = recvfrom(socketServer, recvbuf, sizeof(recvbuf), 0, (SOCKADDR*)&clientName, &iLen_cli);
			send_port=ntohs(clientName.sin_port);
			strcpy(send_addr,inet_ntoa(clientName.sin_addr));
			rcv_port=DNS_PORT;
			strcpy(rcv_addr,LOCAL_ADDRESS);
			if(iRecv>0)
			{
				cout<<endl;
				cout<<"收到外部dns回答"<<endl; 
				//ID转换
				unsigned short *pID = (unsigned short *)malloc(sizeof(unsigned short));
				memcpy(pID, recvbuf, sizeof(unsigned short));
				int m = ntohs(*pID);
				unsigned short oID = htons(IDTransTable[m].oldID);
				memcpy(recvbuf, &oID, sizeof(unsigned short));
				IDTransTable[m].done = TRUE;

				
				//从ID转换表中获取发出DNS请求者的信息
				clientName = IDTransTable[m].client;
					
				//打印id信息 
                DisplayAnswer(m, level,recvbuf);

				//把recvbuf转发至请求者处
				iSend = sendto(socketLocal, recvbuf, iRecv, 0, (SOCKADDR*)&clientName, sizeof(clientName));
				if (iSend == SOCKET_ERROR) {
					cout << "sendto Failed: " << WSAGetLastError() << endl;
					continue;
				}
				else if (iSend == 0)
					break;
            
				free(pID);	//释放动态分配的内存 
			} 
				
			count++;
		}
		
        
        
		//接收DNS请求
		memset(recvbuf, '\0', BUF_SIZE);
		cout<<endl;
		cout<<endl;
		cout<<"新的客户DNS查询"<<endl;
		iRecv = recvfrom(socketLocal, recvbuf, sizeof(recvbuf), 0, (SOCKADDR*)&clientName, &iLen_cli);
		strcpy(recvbuf_temp,recvbuf);
		send_port=ntohs(clientName.sin_port);
		strcpy(send_addr,inet_ntoa(clientName.sin_addr));
		rcv_port=DNS_PORT;
		strcpy(rcv_addr,LOCAL_ADDRESS);
		
		
     
		if (iRecv == SOCKET_ERROR) {
			cout << "Recvfrom Failed: " << WSAGetLastError() << endl;
			continue;
		}
		else if (iRecv == 0) {
			break;
		}
		else {	
			GetUrl(recvbuf, iRecv);				//获取域名
			int find = IsFind(url, num);		//在域名解析表中查找


			//在域名解析表中没有找到
			if (find == NOTFOUND) {
				//ID转换
				{
					GetLocalTime(&sys);
					Day1         = sys.wDay;
    				Hour1        = sys.wHour;
					Minute1      = sys.wMinute;
					Second1      = sys.wSecond;
					jTime=(((sys.wDay - Day1) * 24 + sys.wHour - Hour1) * 60 + sys.wMinute - Minute1) * 60 + sys.wSecond - Second1;
				}
				unsigned short *pID = (unsigned short *)malloc(sizeof(unsigned short));
				memcpy(pID, recvbuf, sizeof(unsigned short));
				unsigned short nID = htons(RegisterNewID(ntohs(*pID), clientName, FALSE,jTime));
				memcpy(recvbuf, &nID, sizeof(unsigned short));
				strcpy(IDTransTable[IDcount-1].urlName,url);
                IDTransTable[IDcount-1].offset=iRecv;
                
                
				//打印 时间 newID 功能 域名 IP
				DisplayInfo(ntohs(nID), find,level);

				//把recvbuf转发至指定的外部DNS服务器
				cout<<endl;
				cout<<"send to outer dns server"<<endl;
				iSend = sendto(socketServer, recvbuf, iRecv, 0, (SOCKADDR*)&serverName, sizeof(serverName));
				if (iSend == SOCKET_ERROR) {
					cout << "sendto Failed: " << WSAGetLastError() << endl;
					continue;
				}
				
				else if (iSend == 0)
					break;
				
				free(pID);	//释放动态分配的内存			
			}

			//在域名解析表中找到
			else {	
				//获取请求报文的ID
				unsigned short *pID = (unsigned short *)malloc(sizeof(unsigned short));
				memcpy(pID, recvbuf, sizeof(unsigned short));

				//转换ID
				{
					GetLocalTime(&sys);
					Day1         = sys.wDay;
    				Hour1        = sys.wHour;
					Minute1      = sys.wMinute;
					Second1      = sys.wSecond;
					jTime=(((sys.wDay - Day1) * 24 + sys.wHour - Hour1) * 60 + sys.wMinute - Minute1) * 60 + sys.wSecond - Second1;
				}
				unsigned short nID = RegisterNewID(ntohs(*pID), clientName, TRUE,jTime);

				//打印 时间 newID 功能 域名 IP
				DisplayInfo(nID, find,level);

				//构造响应报文返回
				memcpy(sendbuf, recvbuf, iRecv);						//拷贝请求报文
				unsigned short a = htons(0x8180);
				memcpy(&sendbuf[2], &a, sizeof(unsigned short));		//修改标志域

				//修改回答数域
				if (strcmp(DNS_table[find].IP.c_str(), "0.0.0.0") == 0)	
					a = htons(0x0000);	//屏蔽功能：回答数为0
				else
					a = htons(0x0001);	//服务器功能：回答数为1
				memcpy(&sendbuf[6], &a, sizeof(unsigned short));
				int curLen = 0;

				//构造DNS响应部分
				char answer[16];
				unsigned short Name = htons(0xc00c);
				memcpy(answer, &Name, sizeof(unsigned short));
				curLen += sizeof(unsigned short);

				unsigned short TypeA = htons(0x0001);
				memcpy(answer+curLen, &TypeA, sizeof(unsigned short));
				curLen += sizeof(unsigned short);

				unsigned short ClassA = htons(0x0001);
				memcpy(answer+curLen, &ClassA, sizeof(unsigned short));
				curLen += sizeof(unsigned short);

				unsigned long timeLive = htonl(0x7b);
				memcpy(answer+curLen, &timeLive, sizeof(unsigned long));
				curLen += sizeof(unsigned long);

				unsigned short IPLen = htons(0x0004);
				memcpy(answer+curLen, &IPLen, sizeof(unsigned short));
				curLen += sizeof(unsigned short);

				unsigned long IP = (unsigned long) inet_addr(DNS_table[find].IP.c_str());
				memcpy(answer+curLen, &IP, sizeof(unsigned long));
				curLen += sizeof(unsigned long);
				curLen += iRecv;

				//请求报文和响应部分共同组成DNS响应报文存入sendbuf
				memcpy(sendbuf+iRecv, answer, curLen);

				//发送DNS响应报文
				iSend = sendto(socketLocal, sendbuf, curLen, 0, (SOCKADDR*)&clientName, sizeof(clientName));
				if (iSend == SOCKET_ERROR) {
					cout << "sendto Failed: " << WSAGetLastError() << endl;
					continue;
				}
				else if (iSend == 0)
					break;
			
				free(pID);		//释放动态分配的内存
			}
		}
	}

    closesocket(socketServer);	//关闭套接字
	closesocket(socketLocal);
    WSACleanup();				//释放ws2_32.dll动态链接库初始化时分配的资源

    return 0;
		
} 












    

