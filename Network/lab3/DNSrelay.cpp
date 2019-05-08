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


#pragma  comment(lib,"ws2_32.lib") //����Ws2_32.lib�⣬������setting������

#define DEF_DNS_ADDRESS "202.38.64.56" //�ⲿDNS��������ַ
#define LOCAL_ADDRESS "114.214.193.126"       //����DNS��������ַ
#define DNS_PORT 53                     //����DNS�����53�Ŷ˿� 
#define BUF_SIZE 512
#define LENGTH 65
#define AMOUNT 300
#define NOTFOUND -1
#define FOUND 1

//DNS�����ײ�
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

//DNS����������ṹ
typedef struct translate
{
	string IP;						//IP��ַ
	string domain;					//����
} Translate;




//IDת����ṹ
typedef struct IDChange
{
	unsigned short oldID;			//ԭ��ID
	bool done;						//����Ƿ���ɽ���
	SOCKADDR_IN client;				//�������׽��ֵ�ַ
	int joinTime;                   //����ת�����ʱ�� 
	char urlName[LENGTH];           //�ͻ�ѯ�ʵ�url���� 
	int offset;                     //�ͻ����ͱ��ĵ��ֽ��� 
} IDTransform;

Translate DNS_table[AMOUNT];		//DNS����������
IDTransform IDTransTable[AMOUNT];	//IDת����
int IDcount = 0;					//ת�����е���Ŀ����
char url[LENGTH];					//����
char recvbuf_temp[BUF_SIZE];		//���ݱ�����(��ӡ��) 
char send_addr[LENGTH],rcv_addr[LENGTH]; //���պͷ������ݰ��ĵ�ַ(��ӡ��)
unsigned short send_port,rcv_port;       //���պͷ������ݰ��Ķ˿�(��ӡ��)
SYSTEMTIME sys;                     //ϵͳʱ��
int Day, Hour, Minute, Second, Milliseconds;//����ϵͳʱ��ı���
int Day1, Hour1, Minute1, Second1;//����ϵͳʱ��ı���
int Day2, Hour2, Minute2, Second2;//����ϵͳʱ��ı���
int Day3, Hour3, Minute3, Second3;//����ϵͳʱ��ı���

/*���ܺ�������*/
//begin:------------------------------------------ 
int GetTable(char *tablePath);          //����1:��ȡ����������
void GetUrl(char *recvbuf,int recvnum); //����2: ��ȡDNS�����е�����
int IsFind(char *url,int num);          //����3���ж��Ƿ��ڱ����ҵ�DNS�����е��������ҵ������±�
                                        //����4��������IDת��Ϊ�µ�ID��������Ϣд��IDת������
unsigned short RegisterNewID(unsigned short oID, SOCKADDR_IN temp, bool ifdone); 
void DisplayInfo(unsigned short newID, int find); //����5����ӡ ʱ�� newID ���� ���� IP


//end-----------------------------------------------------




//����1:��ȡ����������
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


//����2: ��ȡDNS�����е�����
void GetUrl(char *recvbuf, int recvnum)
{
	char urlname[LENGTH];
	int i=0,j,k=0;
	
	memset(url,0,LENGTH);
	memcpy(urlname,&(recvbuf[sizeof(DNSHDR)]),recvnum-16);
	
	int len = strlen(urlname);
	
	//����ת��
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



//����3���ж��Ƿ��ڱ����ҵ�DNS�����е��������ҵ������±�
int IsFind(char* url, int num)
{
	int find = NOTFOUND;
	char* domain;
	for (int i = 0; i < num; i++) {
		domain = (char *)DNS_table[i].domain.c_str();
		if (strcmp(domain, url) == 0) {	//�ҵ�
			find = i;
			break;
		}
	}
	return find;
}


//����4��������IDת��Ϊ�µ�ID��������Ϣд��IDת������
unsigned short RegisterNewID (unsigned short oID, SOCKADDR_IN temp, bool ifdone,int jTime)
{
	srand(time(NULL));
	IDTransTable[IDcount].oldID = oID;
	IDTransTable[IDcount].client = temp;
	IDTransTable[IDcount].done  = ifdone;
	IDTransTable[IDcount].joinTime = jTime; 
	IDcount++;

	return (unsigned short)(IDcount-1);	//�Ա����±���Ϊ�µ�ID
}



//����5����ӡ���ع���ظ����ĵ���Ϣ(���Եȼ�1,2,3) 
void DisplayInfo(unsigned short newID, int find,int level)
{

    //���Լ���Ϊ1
	if(level==1)
	{
		//�ڱ���û���ҵ�DNS�����е�����
		if (find == NOTFOUND) 
		{   
			//�м̹���
			cout.setf(ios::left);
			cout << setiosflags(ios::left) << setw(6) << setfill(' ') << "�м�";
			cout << "    ";
			//��ӡ����
			cout.setf(ios::left);
			cout << setiosflags(ios::left) << setw(20) << setfill(' ') << url;
			cout << "    ";
		}

		//�ڱ����ҵ�DNS�����е�����
		else {
	  		if(DNS_table[find].IP== "0.0.0.0")  //������վ����
			{
				//���ι���
				cout.setf(ios::left); 
		    	cout << setiosflags(ios::left) << setw(6) << setfill(' ') << "����";
		    	cout << "    ";
				//��ӡ����(��*)
				cout.setf(ios::left); 
		    	cout << "*" << setiosflags(ios::left) << setw(19) << setfill(' ') << url;
		    	cout << "    ";
			}

			//�������Ϊ��ͨIP��ַ������ͻ����������ַ
			else
			{
				//��ӡ����
				cout.setf(ios::left);
		    	cout <<"����:"<< setiosflags(ios::left) << setw(19) << setfill(' ') << url;
		    	cout << "    ";
				//��ӡIP
				cout.setf(ios::left);
		    	cout <<"ip��ַ:" <<setiosflags(ios::left) << setw(20) << setfill(' ') << DNS_table[find].IP << endl;
			}
		}
	} 

	//���Լ���Ϊ2 
	else if(level==2)
	{
		//��ӡʱ��
		GetLocalTime( &sys );
	
		if(sys.wMilliseconds >= Milliseconds)
		{
	    cout << setiosflags(ios::right) << setw(6) << setfill(' ') << "ʱ��:"<<(((sys.wDay - Day) * 24 + sys.wHour - Hour) * 60 + sys.wMinute - Minute) * 60 + sys.wSecond - Second;//���ÿ��Ϊ7��right���뷽ʽ
	   	 cout << '.' << setiosflags(ios::right) << setw(3) << setfill('0') << sys.wMilliseconds - Milliseconds;
		}
		else {
			cout << setiosflags(ios::right) << setw(5) << setfill(' ') << "ʱ��:"<< (((sys.wDay - Day) * 24 + sys.wHour - Hour) * 60 + sys.wMinute - Minute) * 60 + sys.wSecond - Second - 1;//���ÿ��Ϊ7��right���뷽ʽ
	    	cout << '.' << setiosflags(ios::right) << setw(3) << setfill('0') << 1000 + sys.wMilliseconds - Milliseconds;
		}
		cout << " ";

		//��ӡת�����µ�ID
		cout.setf(ios::left);
		cout << "ID: "<< newID;
		cout << "  ";

		//�ڱ���û���ҵ�DNS�����е�����
		if (find == NOTFOUND) 
		{   
			//�м̹���
			cout.setf(ios::left);
			cout << setiosflags(ios::left) << setw(6) << setfill(' ') << "�м�";
			cout << "    ";
			//��ӡ����
			cout.setf(ios::left);
			cout << setiosflags(ios::left) << setw(20) << setfill(' ') << url;
			cout << endl;
			
		}

		//�ڱ����ҵ�DNS�����е�����
		else {
	  		if(DNS_table[find].IP == "0.0.0.0")  //������վ����
			{
				//���ι���
				cout.setf(ios::left); 
		    	cout << setiosflags(ios::left) << setw(6) << setfill(' ') << "����";
		    	cout << "    ";
				//��ӡ����(��*)
				cout.setf(ios::left); 
		    	cout << "*" << setiosflags(ios::left) << setw(19) << setfill(' ') << url;
		    	cout << endl;	
			}

			//�������Ϊ��ͨIP��ַ������ͻ����������ַ
			else
			{
				//��ӡ����
				cout.setf(ios::left);
		    	cout  <<setiosflags(ios::left) << setw(15) << setfill(' ') <<"����:"<< url;
		    	
				
				//�ͻ���ip��ַ
				IDTransTable[IDcount].client.sin_addr;
				char ipTemp[40];
				strcpy(ipTemp,inet_ntoa(IDTransTable[newID].client.sin_addr));
				cout  <<setiosflags(ios::left) << setw(15) << setfill(' ')<<"ip��ַ:" << ipTemp<<endl;
			
				
			}
		}
	}
	
	else if(level==3)
	{
		//�ڱ���û���ҵ�DNS�����е�����
		if (find == NOTFOUND) 
		{   
			//�м̹���
			cout.setf(ios::left);
			cout << setiosflags(ios::left) << setw(6) << setfill(' ') << "�м�";
			cout << "    ";
			
			//��ӡ����
			cout.setf(ios::left);
			cout << setiosflags(ios::left) << setw(20) << setfill(' ') << url;
			cout<< endl;
				
		}

		//�ڱ����ҵ�DNS�����е�����
		else {
	  		if(DNS_table[find].IP == "0.0.0.0")  //������վ����
			{
				//���ι���
				cout.setf(ios::left); 
		    	cout << setiosflags(ios::left) << setw(6) << setfill(' ') << "����";
		    	cout << "    ";
				//��ӡ����(��*)
				cout.setf(ios::left); 
		    	cout << "*" << setiosflags(ios::left) << setw(19) << setfill(' ') << url;
		    	cout << endl;
			}

			//�������Ϊ��ͨIP��ַ������ͻ����������ַ
			else
			{
				//��ӡ����
				cout.setf(ios::left);
		    	cout <<"����:"<< setiosflags(ios::left) << setw(19) << setfill(' ') << url;
		    	cout << " ";
				//��ӡIP
				cout.setf(ios::left);
		    	cout <<"ip��ַ:"<< setiosflags(ios::left) << setw(20) << setfill(' ') << DNS_table[find].IP << endl;
		    	
		    	//��ӡ���ݱ�����
				cout<<endl;
				cout<<"���ݱ�ԭʼ����:"<<endl;
				for(int i=0;i<BUF_SIZE;i++) printf("%x",recvbuf_temp[i]);
				cout <<endl;
			
			
				//��ӡ���պͷ������ݰ��ĵ�ַ���˿� 
				cout<<endl;
				cout <<"���͵�ַ: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << send_addr<<endl;
				cout <<"���Ͷ˿�: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << send_port<<endl;
				cout <<"���յ�ַ: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << rcv_addr<<endl;
				cout <<"���ն˿�: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << rcv_port<<endl;
		    	
			}
		}
	}
	
	
}


//����6:��ȡ��Ӧ������url 
void readurl(char* buf, char* dest)
{
    int len = strlen(buf);
    int i = 0, j = 0, k = 0;
    while (i < len)
    {
        if (buf[i] > 0 && buf[i] <= 63) //����Ǹ�����
        {
            for (j = buf[i], i++; j > 0; j--, i++, k++) //j�Ǽ����Ǽ���k��Ŀ��λ���±꣬i�Ǳ�������±�
                dest[k] = buf[i];
        }

        if (buf[i] != 0)    //���û��������dest��Ӹ�'.'
        {
            dest[k] = '.';
            k++;
        }
    }
    dest[k] = '\0';
}



//����7����ӡ�ⲿDNS��Ӧ���ĵ������Ϣ(���Լ���1&3)
void DisplayAnswer(int m,int level,char *recvbuf)
{
	
	char url_r[65];
    int length = -1;

    length = IDTransTable[m].offset;
    int nquery = ntohs(*((unsigned short*)(recvbuf + 4))), nresponse = ntohs(*((unsigned short*)(recvbuf + 6)));    //����������ش����
    char* p = recvbuf + 12; //����DNS��ͷ��ָ��
    ip_addr ip;
    int ip1, ip2, ip3, ip4;

        //��ȡÿ��������Ĳ�ѯurl
        for (int i = 0; i < nquery; ++i)
        {
            readurl(p, url_r);    //��ôдurl��ֻ���¼���һ�������url
            while (*p > 0)  	//��ȡ��ʶ��ǰ�ļ����������url
                p += (*p) + 1;
            p += 5; //����url�����Ϣ��ָ����һ������
        }

        if (nresponse > 0 && level >= 1)
            printf("��ѯ����: %s  ", url);
        //�����ظ�
        //����ο�DNS�ظ����ĸ�ʽ
        for (int i = 0; i < nresponse; ++i)
        {
            if ((unsigned char)*p == 0xc0) //��ָ�������
                p += 2;
            else
            {
                //���ݼ�������url
                while (*p > 0)
                    p += (*p) + 1;
                ++p;    //ָ����������
            }
            unsigned short resp_type = ntohs(*(unsigned short*)p);  //�ظ�����
            p += 2;
            unsigned short resp_class = ntohs(*(unsigned short*)p); //�ظ���
            p += 2;
            unsigned short high = ntohs(*(unsigned short*)p);   //����ʱ���λ
            p += 2;
            unsigned short low = ntohs(*(unsigned short*)p);    //����ʱ���λ
            p += 2;
            int ttl = (((int)high) << 16) | low;    //�ߵ�λ��ϳ�����ʱ��
            int datalen = ntohs(*(unsigned short*)p);   //�������ݳ���
            p += 2;
            
                
            
            if (resp_type == 1) //��A���ͣ��ظ�����url��ip
            {
                memset(ip.addr, 0, sizeof(ip.addr));
                //��ȡ4��ip����
                ip1 = (unsigned char)*p++;
                ip2 = (unsigned char)*p++;
                ip3 = (unsigned char)*p++;
                ip4 = (unsigned char)*p++;
				
                printf("ip��ַ: %d.%d.%d.%d\n", ip1, ip2, ip3, ip4);
                if (level == 3)
            	{
            	printf("type:%d  class:%d  ttl:%d\n", resp_type, resp_class, ttl);
				}
			
                break;
            }
            else p += datalen;  //ֱ������
        }

	
	if(level==3)
	{
		//��ӡ���պͷ������ݰ��ĵ�ַ���˿� 
		cout <<"���ͷ���ַ: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << send_addr<<endl;
		cout <<"���ͷ��˿�: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << send_port<<endl;
		cout <<"���շ���ַ: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << rcv_addr<<endl;
		cout <<"���շ��˿�: "<<setiosflags(ios::left) << setw(20) << setfill(' ') << rcv_port<<endl;
		cout<<"���ݱ�ԭʼ����:"<<endl;
		for(int i=0;i<BUF_SIZE;i++) printf("%x",recvbuf_temp[i]);
		cout <<endl; 
	}
	
} 






//������
int main(int argc, char** argv)
{
	WSADATA wsaData; 
    SOCKET  socketServer, socketLocal;				//����DNS���ⲿDNS�����׽���
    SOCKADDR_IN serverName, clientName, localName;	//����DNS���ⲿDNS����������������׽��ֵ�ַ
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
    
    //���Լ���1
	if (argc == 2) {
		strcpy(outerDns, DEF_DNS_ADDRESS);
		strcpy(tablePath, "example.txt");
	}
    //���Լ���2
	else if (argc == 4) {
		strcpy(outerDns, argv[2]);
		strcpy(tablePath, argv[3]);
	}
    //���Լ���3
	else if (argc == 3) {
		strcpy(outerDns, argv[2]);
		strcpy(tablePath, "example.txt");
	}

	num = GetTable(tablePath);						//��ȡ����������
    
	//����ϵͳ��ʱ��
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
	

	for (int i=0; i < AMOUNT; i++) {				//��ʼ��IDת����
		IDTransTable[i].oldID = 0;
		IDTransTable[i].done  = FALSE;
		IDTransTable[i].joinTime = 0; 
		IDTransTable[i].offset = 0; 
		memset(&(IDTransTable[i].client), 0, sizeof(SOCKADDR_IN));
		memset(&(IDTransTable[i].urlName), 0, LENGTH*sizeof(char));
	}

    WSAStartup(MAKEWORD(2,2), &wsaData);			//��ʼ��ws2_32.dll��̬���ӿ�

	//��������DNS���ⲿDNS�׽���
    socketServer = socket(AF_INET, SOCK_DGRAM, 0);
    unsigned long ul=1;
    int ret;
    ret=ioctlsocket(socketServer,FIONBIO,(unsigned long *)&ul);
	socketLocal = socket(AF_INET, SOCK_DGRAM, 0);

	//���ñ���DNS���ⲿDNS�����׽���
	localName.sin_family = AF_INET;//��ַ�� 
	localName.sin_port = htons(DNS_PORT);//16λ�˿ں� 
	localName.sin_addr.s_addr = inet_addr(LOCAL_ADDRESS);//32λIPv4��ַ 

	serverName.sin_family = AF_INET;
	serverName.sin_port = htons(DNS_PORT);
	serverName.sin_addr.s_addr = inet_addr(outerDns);

	//�󶨱���DNS��������ַ
	if (bind(socketLocal, (SOCKADDR*)&localName, sizeof(localName))) 
	{
		cout << "Binding Port 53 failed." << endl;
		exit(1);
	}
	else
		cout << "Binding Port 53 succeed." << endl;
   
	//����DNS�м̷������ľ������
	while (1) 
	{
		iLen_cli = sizeof(clientName);
        memset(recvbuf, '\0', BUF_SIZE);
        
        //ÿ19�봦��һ��IDת�����еĳ�ʱ���
		GetLocalTime(&sys);
	    int countTime=(((sys.wDay - Day) * 24 + sys.wHour - Hour) * 60 + sys.wMinute - Minute) * 60 + sys.wSecond - Second;
		int dealTime=countTime%20;
		if(dealTime==19)
		{
			for(int i=0;i<IDcount;i++)
			{
				if(IDTransTable[i].done==FALSE&&(countTime-IDTransTable[i].joinTime>=10))
				
				cout<<"query from client��"<< inet_ntoa(IDTransTable[i].client.sin_addr)<<"�� has run out of time"<<endl;
			} 
		}
        
        
        
        //�ж��Ƿ��յ��ⲿDNS�ķ���
        long int count=0;
        while(count<3)
		{
			//���������ⲿDNS����������Ӧ����
			iRecv = recvfrom(socketServer, recvbuf, sizeof(recvbuf), 0, (SOCKADDR*)&clientName, &iLen_cli);
			send_port=ntohs(clientName.sin_port);
			strcpy(send_addr,inet_ntoa(clientName.sin_addr));
			rcv_port=DNS_PORT;
			strcpy(rcv_addr,LOCAL_ADDRESS);
			if(iRecv>0)
			{
				cout<<endl;
				cout<<"�յ��ⲿdns�ش�"<<endl; 
				//IDת��
				unsigned short *pID = (unsigned short *)malloc(sizeof(unsigned short));
				memcpy(pID, recvbuf, sizeof(unsigned short));
				int m = ntohs(*pID);
				unsigned short oID = htons(IDTransTable[m].oldID);
				memcpy(recvbuf, &oID, sizeof(unsigned short));
				IDTransTable[m].done = TRUE;

				
				//��IDת�����л�ȡ����DNS�����ߵ���Ϣ
				clientName = IDTransTable[m].client;
					
				//��ӡid��Ϣ 
                DisplayAnswer(m, level,recvbuf);

				//��recvbufת���������ߴ�
				iSend = sendto(socketLocal, recvbuf, iRecv, 0, (SOCKADDR*)&clientName, sizeof(clientName));
				if (iSend == SOCKET_ERROR) {
					cout << "sendto Failed: " << WSAGetLastError() << endl;
					continue;
				}
				else if (iSend == 0)
					break;
            
				free(pID);	//�ͷŶ�̬������ڴ� 
			} 
				
			count++;
		}
		
        
        
		//����DNS����
		memset(recvbuf, '\0', BUF_SIZE);
		cout<<endl;
		cout<<endl;
		cout<<"�µĿͻ�DNS��ѯ"<<endl;
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
			GetUrl(recvbuf, iRecv);				//��ȡ����
			int find = IsFind(url, num);		//�������������в���


			//��������������û���ҵ�
			if (find == NOTFOUND) {
				//IDת��
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
                
                
				//��ӡ ʱ�� newID ���� ���� IP
				DisplayInfo(ntohs(nID), find,level);

				//��recvbufת����ָ�����ⲿDNS������
				cout<<endl;
				cout<<"send to outer dns server"<<endl;
				iSend = sendto(socketServer, recvbuf, iRecv, 0, (SOCKADDR*)&serverName, sizeof(serverName));
				if (iSend == SOCKET_ERROR) {
					cout << "sendto Failed: " << WSAGetLastError() << endl;
					continue;
				}
				
				else if (iSend == 0)
					break;
				
				free(pID);	//�ͷŶ�̬������ڴ�			
			}

			//���������������ҵ�
			else {	
				//��ȡ�����ĵ�ID
				unsigned short *pID = (unsigned short *)malloc(sizeof(unsigned short));
				memcpy(pID, recvbuf, sizeof(unsigned short));

				//ת��ID
				{
					GetLocalTime(&sys);
					Day1         = sys.wDay;
    				Hour1        = sys.wHour;
					Minute1      = sys.wMinute;
					Second1      = sys.wSecond;
					jTime=(((sys.wDay - Day1) * 24 + sys.wHour - Hour1) * 60 + sys.wMinute - Minute1) * 60 + sys.wSecond - Second1;
				}
				unsigned short nID = RegisterNewID(ntohs(*pID), clientName, TRUE,jTime);

				//��ӡ ʱ�� newID ���� ���� IP
				DisplayInfo(nID, find,level);

				//������Ӧ���ķ���
				memcpy(sendbuf, recvbuf, iRecv);						//����������
				unsigned short a = htons(0x8180);
				memcpy(&sendbuf[2], &a, sizeof(unsigned short));		//�޸ı�־��

				//�޸Ļش�����
				if (strcmp(DNS_table[find].IP.c_str(), "0.0.0.0") == 0)	
					a = htons(0x0000);	//���ι��ܣ��ش���Ϊ0
				else
					a = htons(0x0001);	//���������ܣ��ش���Ϊ1
				memcpy(&sendbuf[6], &a, sizeof(unsigned short));
				int curLen = 0;

				//����DNS��Ӧ����
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

				//�����ĺ���Ӧ���ֹ�ͬ���DNS��Ӧ���Ĵ���sendbuf
				memcpy(sendbuf+iRecv, answer, curLen);

				//����DNS��Ӧ����
				iSend = sendto(socketLocal, sendbuf, curLen, 0, (SOCKADDR*)&clientName, sizeof(clientName));
				if (iSend == SOCKET_ERROR) {
					cout << "sendto Failed: " << WSAGetLastError() << endl;
					continue;
				}
				else if (iSend == 0)
					break;
			
				free(pID);		//�ͷŶ�̬������ڴ�
			}
		}
	}

    closesocket(socketServer);	//�ر��׽���
	closesocket(socketLocal);
    WSACleanup();				//�ͷ�ws2_32.dll��̬���ӿ��ʼ��ʱ�������Դ

    return 0;
		
} 












    

