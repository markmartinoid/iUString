#pragma once

#ifndef _IDNS_H
#define _IDNS_H




#ifdef _WINDOWS_
#include <winsock2.h>
#include <ws2tcpip.h>
#endif//   _WINDOWS_
#include "iUString.h"
#include "iDynArray.h"
#include "iCodes.h"
#include "iNetCodes.h"

#include <Iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")


#include <malloc.h>


struct iDNS_HEADER {
	unsigned	short id;		    // identification number
	
	unsigned	char rd     :1;		// recursion desired
	unsigned	char tc     :1;		// truncated message
	unsigned	char aa     :1;		// authoritive answer
	unsigned	char opcode :4;	    // purpose of message
	unsigned	char qr     :1;		// query/response flag
	
	unsigned	char rcode  :4;	    // response code
	unsigned	char cd     :1;	    // checking disabled
	unsigned	char ad     :1;	    // authenticated data
	unsigned	char z      :1;		// its z! reserved
	unsigned	char ra     :1;		// recursion available
	
	unsigned    short q_count;	    // number of question entries
	unsigned	short ans_count;	// number of answer entries
	unsigned	short auth_count;	// number of authority entries
	unsigned	short add_count;	// number of resource entries
};
 
struct iDNS_QUESTION {
	unsigned short qtype;
	unsigned short qclass;
};

//Type field of Query and Answer
#define iDNS_T_A		    1UL		/* host address */
#define iDNS_T_NS		2UL		/* authoritative server */
#define iDNS_T_CNAME		5UL		/* canonical name */
#define iDNS_T_SOA		6UL		/* start of authority zone */
#define iDNS_T_PTR		12UL		/* domain name pointer */
#define iDNS_T_MX		15UL		/* mail routing information */




struct DnsRecord{
char* ServerName;
sockaddr *a_addr;          
unsigned int a_addr_count;
unsigned int a_addr_usefull_index;

sockaddr *mx_addr;          
unsigned int mx_addr_count;
unsigned int mx_addr_usefull_index;


};



struct tempmx{iString<char> *mxHostName;unsigned short preference;};



int OnmxCompare(const tempmx &x1,const tempmx &x2)
{
return (x1.preference-x2.preference);
}




class iDNSResolver
{
public:
iDynArray<unsigned long> aDNSservers;
iDynArray<DnsRecord> aDNS;
bool WSA_started;
WSADATA wsa;
int DNS_Active;

ULONG addr_127_0_0_1;


unsigned int TimeoutSend;//ms  
unsigned int TimeoutReceive; 

iDNSResolver()
{
 WSA_started=false;  
 WSA_Start();
RetrieveDnsServers();
SetDNS_Max_Count(128);
 TimeoutSend=10000; 
 TimeoutReceive=10000;
 DNS_Active=0;

addr_127_0_0_1=inet_addr("127.0.0.1");///win 
}

bool WSA_Start(bool restart=false)
{
if(WSA_started)
 {
 if(restart){WSACleanup();}else{return true;} 
 } 
int iResult=WSAStartup(MAKEWORD(2, 2), &wsa);
if(iResult!=0){WSA_started=false;return false;}
return true;
}


void SetDNS_Max_Count(unsigned int count)
{
aDNS.Allocate(count);
}





unsigned int aDNS_index(iString<char>&Host,const ULONG iDNS_T)
{
if(Host.GetLength()==0){return UL_ERROR;}
ULONG index;
for (index=0;index<aDNS.GetCount();index++)
 {
 if(iStrEqual_s<char>(aDNS[index].ServerName,Host.Buffer(),Host.GetLength())) {return index;}
 }


if(aDNS.GetCount()>=aDNS.GetAllocatedCount())
 {                                           
 for (index=0;index<aDNS.GetCount();index++)
  {
   if(aDNS[index].a_addr!=NULL)
    {
    free(aDNS[index].a_addr);  aDNS[index].a_addr=NULL;
    free(aDNS[index].ServerName); aDNS[index].ServerName=NULL;
    } 
  } 
 aDNS.SetCount(0);
 }
index=UL_ERROR;
if(0!=ngethostbyname(Host,index,iDNS_T)){return UL_ERROR;} 
return index;
}







bool addHost( iString<char> &host, const ULONG addr4, ULONG &index,const ULONG iDNS_T)
{
if(host==NULL){return false;}
unsigned int i;
bool index_found=false;
for (index=0;index<aDNS.GetCount();index++)
 {
 if(host==aDNS[index].ServerName)
  {
   switch (iDNS_T)
   {
   case iDNS_T_A:
    {
    for (i=0;i<aDNS[index].a_addr_count;i++)
     {
     if(((sockaddr_in*)aDNS[index].a_addr)->sin_addr.S_un.S_addr  ==  addr4){return true;}
     }
    break;
    }
   case iDNS_T_MX:
    {
    for (i=0;i<aDNS[index].mx_addr_count;i++)
     {
     if(((sockaddr_in*)aDNS[index].mx_addr)->sin_addr.S_un.S_addr  ==  addr4){return true;}
     }
    break;
    }
   default:break;
   }
  index_found=true;
  break;
  }
 }
if(! index_found)
{index=aDNS.GetCount();aDNS.SetCount(index+1);
aDNS[index].a_addr_usefull_index=UL_ERROR;aDNS[index].a_addr_count=0;aDNS[index].a_addr=NULL;
aDNS[index].mx_addr_usefull_index=UL_ERROR;aDNS[index].mx_addr_count=0;aDNS[index].mx_addr=NULL;
aDNS[index].ServerName=(char*)malloc(host.GetLength()+sizeof(char)); 
memcpy(aDNS[index].ServerName,host.Buffer(),host.GetLength()+sizeof(char));
} 
   
switch (iDNS_T)
 {
  case iDNS_T_A:
  {
  void* ret=realloc(aDNS[index].a_addr,(aDNS[index].a_addr_count+1)*sizeof(sockaddr));
  if(ret==NULL){return false;}
  aDNS[index].a_addr_count++;
  aDNS[index].a_addr=(sockaddr*)ret;
  sockaddr_in* newaddr=(sockaddr_in*)&aDNS[index].a_addr[aDNS[index].a_addr_count-1];
  memset(newaddr,0,sizeof(sockaddr));
  newaddr->sin_family=AF_INET;
  newaddr->sin_addr.S_un.S_addr  =addr4;
  break;
  }
  case iDNS_T_MX:
  {
  void* ret=realloc(aDNS[index].mx_addr,(aDNS[index].mx_addr_count+1)*sizeof(sockaddr));
  if(ret==NULL){return false;}
  aDNS[index].mx_addr_count++;
  aDNS[index].mx_addr=(sockaddr*)ret;
  sockaddr_in* newaddr=(sockaddr_in*)&aDNS[index].mx_addr[aDNS[index].mx_addr_count-1];
  memset(newaddr,0,sizeof(sockaddr));
  newaddr->sin_family=AF_INET;
  newaddr->sin_addr.S_un.S_addr  =addr4;
  break;
  }
 }
return true;
}



void dnsrecv(SOCKET &s,char*buf,int &buf_size,int &ReceivedTotal,int maxrecvcall)
{
int iResult;
int recv_call_count=0;
do
 {
 iResult=recv (s,&buf[ReceivedTotal],buf_size-ReceivedTotal,0);
 if(iResult==0){break;}
 if (SOCKET_ERROR==iResult){break;  }
 ReceivedTotal+=iResult;
 recv_call_count++; 
 if(recv_call_count>=maxrecvcall){break;}
 } while(ReceivedTotal<buf_size);
}



int ngethostbyname( iString<char> &host,ULONG &index,const ULONG iDNS_T, iString<char> * forceHost=NULL) 
{
if(host==NULL){return IC_ERROR_PARAMETER_ERROR;}
unsigned long a=inet_addr(host.Buffer());
if(a!=INADDR_NONE)
 {
 if(!addHost(host,a,index,iDNS_T)){return IC_OUT_OF_MEMORY;}
 DNS_Active=1;
 return 0;
 }
if (aDNSservers.GetCount()==0){return IC_ERROR_NO_DNS_SERVERS;}

SOCKET s = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
if(s==INVALID_SOCKET )   {return IC_ERROR_NO_INIT;}
setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(char *) &TimeoutReceive,sizeof(TimeoutReceive));
setsockopt(s,SOL_SOCKET,SO_SNDTIMEO,(char *) &TimeoutSend,sizeof(TimeoutSend));



iDynArray<tempmx> a_mx;
if((iDNS_T==iDNS_T_MX)&&(forceHost==NULL)){a_mx.Allocate(8);}


sockaddr_in dest;
memset(&dest,0,sizeof(dest));
dest.sin_family=AF_INET;
dest.sin_port=htons(53);
dest.sin_addr.s_addr=aDNSservers[0];

sockaddr_in dest2;
memset(&dest2,0,sizeof(dest2));
dest2.sin_family=AF_INET;
dest2.sin_port=htons(53);
dest2.sin_addr.s_addr=aDNSservers[0];

int buf_size=65536;
char *buf=(char *)malloc(buf_size);
if( buf==NULL){return IC_OUT_OF_MEMORY;}
 int buf_position;


iDNS_HEADER *dns= (iDNS_HEADER *)buf;
u_short request_id= static_cast<u_short>(GetCurrentProcessId());
	dns->id = request_id;
	dns->qr = 0;      //This is a query
	dns->opcode = 0;  //This is a standard query
	dns->aa = 0;      //Not Authoritative
	dns->tc = 0;      //This message is not truncated
	dns->rd = 1;      //Recursion Desired
	dns->ra = 0;      //Recursion not available! hey we dont have it (lol)
	dns->z  = 0;
	dns->ad = 0;
	dns->cd = 0;
	dns->rcode = 0;
	dns->q_count = htons(1);   //we have only 1 question
	dns->ans_count  = 0;
	dns->auth_count = 0;
	dns->add_count  = 0;

buf_position=sizeof(iDNS_HEADER);

iString<char> str;
if(forceHost==NULL)
 {
 ToDnsNameFormat(host.Buffer(),str);
 }else{
 str=host;
 }
memcpy(&buf[buf_position],str.Buffer(),   str.GetLength()+1);
buf_position+=str.GetLength()+1;



iDNS_QUESTION* qinfo =(iDNS_QUESTION*)&buf[buf_position];

switch (iDNS_T)
 {
  case iDNS_T_A:
  {
  qinfo->qtype = htons(iDNS_T_A); 
  break;
  }
  case iDNS_T_MX:
  {
  if(forceHost!=NULL){qinfo->qtype = htons(iDNS_T_A);}else{qinfo->qtype = htons(iDNS_T_MX);} 
  break;
  }
  default:{return IC_ERROR_PARAMETER_ERROR;}
}

qinfo->qclass = htons(1); //its internet (lol)
buf_position+= sizeof(iDNS_QUESTION);

////	if (ioctlsocket(s, FIONBIO, &ioctl_opt) == SOCKET_ERROR) {return 0;  	}
		


/*
 HANDLE f=CreateFile(L".\\_file.txt",GENERIC_READ | GENERIC_WRITE,0,0, CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL , NULL);
 if(f!=INVALID_HANDLE_VALUE)
 {
 uint32_t NumberOfBytesWritten;
 WriteFile(f,(char*)&buf[0],buf_position,&NumberOfBytesWritten,NULL);
 CloseHandle (f);
 }
 return 0;
*/

// unsigned long ioctl_opt = 1;
// unsigned long ioctl_opt = 0;
//	if (ioctlsocket(s, FIONBIO, &ioctl_opt) == SOCKET_ERROR) {
//		return IC_ERROR_DNS;
//	}




if(connect(s,(sockaddr*)&dest,sizeof(dest))==SOCKET_ERROR ){closesocket(s);free(buf);return IC_ERROR_DNS;}
if(send(s,buf,buf_position,0)==SOCKET_ERROR ){closesocket(s);free(buf);return IC_ERROR_DNS;}


int ReceivedTotal=0;
dnsrecv(s,buf,buf_size,ReceivedTotal,1);

if(ReceivedTotal<sizeof(iDNS_HEADER)){free(buf);closesocket(s); return IC_ERROR_DNS; }   //uint32_t dw=WSAGetLastError();
 



/*
 HANDLE f=CreateFile(".\\_file.txt",GENERIC_READ | GENERIC_WRITE,0,0, CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL , NULL);
 if(f!=INVALID_HANDLE_VALUE)
 {
 uint32_t NumberOfBytesWritten;
 WriteFile(f,(char*)&buf[0],ReceivedTotal,&NumberOfBytesWritten,NULL);
 CloseHandle (f);
 }
 closesocket(s);return 0;
*/

/*
HANDLE f=CreateFile(".\\_file.txt",GENERIC_READ | GENERIC_WRITE,0,0, OPEN_EXISTING  ,FILE_ATTRIBUTE_NORMAL , NULL);
uint32_t NumberOfBytesWritten;
ReadFile (f, (char*)&buf[0],    286     , &NumberOfBytesWritten, NULL);
iResult= NumberOfBytesWritten;
*/



DNS_Active=4;
if(ReceivedTotal>buf_size){free(buf);closesocket(s);return IC_ERROR_DNS;}
dns= (iDNS_HEADER *)buf;
if(dns->id!=request_id){free(buf);closesocket(s);return IC_ERROR_DNS;}

buf_position=sizeof(iDNS_HEADER);
if (!iStrEqual_s<char>(str.Buffer(),&buf[buf_position],str.GetLength())){free(buf);closesocket(s);return IC_ERROR_DNS;}
buf_position+=str.GetLength();
buf[buf_position]=0;  // for recursive unpack
buf_position++;
buf_position+= sizeof(iDNS_QUESTION);

//RCODE     0               No error

u_short RDLENGTH;
u_short TYPE;


int retval=-1;

for(int i=0;i<ntohs(dns->ans_count);i++) 
 {
 if((buf_position+12)>=ReceivedTotal){dnsrecv(s,buf,buf_size,ReceivedTotal,2);     if((buf_position+12)>=ReceivedTotal)break;}
 // NAME
 if(buf[buf_position]&0xC0)
  {
  buf_position+=2;
  }else{
   while((buf[buf_position]!=0)&&(buf_position<ReceivedTotal)){buf_position++;}
  }
 if((buf_position+10)>=ReceivedTotal){dnsrecv(s,buf,buf_size,ReceivedTotal,2); if((buf_position+10)>=ReceivedTotal)break;}
 TYPE=ntohs(*((unsigned short*)&buf[buf_position]));
 buf_position+=8;
 RDLENGTH=ntohs(*((unsigned short*)&buf[buf_position]));
 buf_position+=2;
 if((RDLENGTH+buf_position)>ReceivedTotal){dnsrecv(s,buf,buf_size,ReceivedTotal,2);   if((RDLENGTH+buf_position)>ReceivedTotal)break;}//if((RDLENGTH+buf_position)>=ReceivedTotal){break;}
	if(TYPE==iDNS_T_A) 
  {
  if(RDLENGTH==4)
   {
   if(forceHost==NULL)
    {
    if(!addHost(host,*((ULONG*) &buf[buf_position]),index,iDNS_T)){free(buf);closesocket(s);return IC_OUT_OF_MEMORY;}
    } else{
		if(addr_127_0_0_1!=*((ULONG*) &buf[buf_position]))
		   {if(!addHost(*forceHost,*((ULONG*) &buf[buf_position]),index,iDNS_T)){free(buf);closesocket(s);return IC_OUT_OF_MEMORY;} }
    }
   retval=0;
   }
  }
	if((TYPE==iDNS_T_MX)&&(iDNS_T==iDNS_T_MX)&&(forceHost==NULL)) 
  {
  //buf[RDLENGTH+buf_position-1]=0;//??????????????????????
  TYPE=ntohs(*((unsigned short*)&buf[buf_position]));//preference
  buf_position+=2;
  a_mx.SetCount(a_mx.GetCount()+1);
  a_mx[a_mx.GetCount()-1].preference=TYPE;
  a_mx[a_mx.GetCount()-1].mxHostName=new iString<char>();    // \rgmail-smtp-inlgoogleA
  unsigned int RecursionLevel=0;
  Unpack(buf,buf_position,buf_position+RDLENGTH-1,*(a_mx[a_mx.GetCount()-1].mxHostName),RecursionLevel);
  }
  buf_position+=RDLENGTH;
 }


if((iDNS_T==iDNS_T_MX)&&(forceHost==NULL))
 {
 a_mx.OnSortCompare=OnmxCompare;
 a_mx.Sort();
 int retval2;
 retval=0;
 for(unsigned int i=0;i<a_mx.GetCount();i++)
  {
  retval2=ngethostbyname(*a_mx[i].mxHostName,index,iDNS_T,&host);
  if(retval2!=0){retval=retval2;}
  delete(a_mx[i].mxHostName);
  }

  if(index==UL_ERROR)ngethostbyname(host,index,iDNS_T_A,NULL);
  if(index==UL_ERROR){free(buf);closesocket(s);return retval;}
  if(aDNS[index].mx_addr_count==0)
   {
   for(unsigned int xx=0;xx<aDNS[index].a_addr_count;xx++) 
    {
    void* ret=realloc(aDNS[index].mx_addr,(aDNS[index].mx_addr_count+1)*sizeof(sockaddr));
    if(ret==NULL){free(buf);closesocket(s);return IC_OUT_OF_MEMORY;}
    aDNS[index].mx_addr_count++;
    aDNS[index].mx_addr=(sockaddr*)ret;
	memcpy(&aDNS[index].mx_addr[aDNS[index].mx_addr_count-1], &aDNS[index].a_addr[xx],sizeof(sockaddr));

    }
   }
  }






free(buf);
closesocket(s);
return retval;
}

                                              
void Unpack(char*buf,unsigned int buf_position,unsigned int max_index ,iString<char>&s,unsigned int &RecursionLevel)
{
s.Allocate(128);
s.SetLength(0);
RecursionLevel++;
if(RecursionLevel>10){return;}


for(unsigned int i=buf_position;i<=max_index;i++)
 {
 if((unsigned char)buf[i]>=0xC0)
  {
  if(i==max_index){return;}
  unsigned short pos=ntohs(*((unsigned short*)&buf[i]));
  pos=pos&0x3FFF;
  if(pos>=max_index){return;}
  iString<char> temp;
  Unpack(buf,pos,max_index,temp,RecursionLevel);
  s+=temp;   //   &buf[pos]
  return;  //?last bytes skip
  }
 if(buf[i]==0){break;}
 s.AppendChar(buf[i]); 

 }



//sizeof(iDNS_HEADER)
}




void FromDnsNameFormat(iString<char>& s) {
	//convert 3www6google3com0 to www.google.com
//www6google3com0



if(s.GetLength()<2)return;
unsigned int index=(unsigned char)s[0];
unsigned int new_index;

s.Remove(0,1);
unsigned int max_index=s.GetLength()-1;
while (index<=max_index)
{
new_index=s[index];
if(new_index==0) break;
s[index]='.';
index+=new_index;
index++;

}

}


void ToDnsNameFormat(const char* hostName,iString<char>& s) {
	//convert www.google.com to 3www6google3com   
s.SetLength(0);
int char_count;
int total=0; 
int hostName_length=(unsigned int)strlen(hostName);
do{
 char_count=iStrPos<char>(&hostName[total],".");
 if(char_count==-1){char_count=hostName_length-total;}
 s.Append((char)char_count,1);
 s.Append(hostName[total],char_count);
 total+=char_count+1;
 }while (total<hostName_length);

}


void RetrieveDnsServersFromRegistry() {
 HKEY hkey=0;
	iString<char> name;
 name.Allocate(256);
	char *path="SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces";
	
 iString<char>  fullpath;
 fullpath.Allocate(256);



	HKEY inter;
	unsigned long count,i,charCount,err;
 if(ERROR_SUCCESS!=RegOpenKeyExA(HKEY_LOCAL_MACHINE , path , 0 , KEY_READ , &hkey )){return;}
	if(ERROR_SUCCESS!=RegQueryInfoKeyA(hkey, 0 , 0 , 0 , &count , 0 , 0 , 0 , 0 , 0 , 0 , 0 )){return;}
	for(i=0;i<count;i++) 
  {
  charCount=name.GetAllocated()-1;
		if(ERROR_SUCCESS!=RegEnumKeyExA(hkey , i ,(char*) name.Buffer() , &charCount , 0 , 0 , 0 , 0 )){continue;}
  name.SetLength(charCount,false);
		
  fullpath=path;
  if(fullpath[fullpath.GetLength()-1]!='\\'){fullpath+="\\";}
  fullpath+=name;

  inter=0;
  if(ERROR_SUCCESS!=RegOpenKeyExA(HKEY_LOCAL_MACHINE , fullpath.Buffer() , 0 , KEY_READ , &inter )){continue;}
				

  charCount=name.GetAllocated()-1;
		err=RegQueryValueExA(inter , "NameServer" , 0 , 0 ,(LPBYTE) name.Buffer() ,	&charCount );
  name.SetLength(charCount,false);
		name.ResetLength(); 

		if((err==ERROR_SUCCESS) && (name.GetLength()>6)) 
  {
  
  int end1_index=name.IndexFirst(",");
  if(end1_index!=-1)name[end1_index]=0;
  if(name[name.GetLength()-1]==32){name[name.GetLength()-1]=0;}
  
  unsigned long addr=inet_addr(name.Buffer());
  if(INADDR_NONE!=addr){ aDNSservers.Add(addr);}

  if((end1_index!=-1)&&((name.GetLength()-end1_index)>6))
   {
   unsigned long begin2_index=end1_index+1;
   if(name[begin2_index+1]==32){begin2_index++;}
   addr=inet_addr(&name[begin2_index]);
   if(INADDR_NONE!=addr){ aDNSservers.Add(addr);}
   }
  }
		RegCloseKey(inter);
	}
	RegCloseKey(hkey);


}


void RetrieveDnsServers() {

FIXED_INFO *pFixedInfo;
ULONG ulOutBufLen;
uint32_t dwRetVal;
IP_ADDR_STRING *pIPAddr;

pFixedInfo = (FIXED_INFO *) malloc(sizeof (FIXED_INFO));
if (pFixedInfo == NULL) 
 {
// printf("Error allocating memory needed to call GetNetworkParams\n");
 return ;
 }
ulOutBufLen = sizeof (FIXED_INFO);

if (GetNetworkParams(pFixedInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
 {
 free(pFixedInfo);
 pFixedInfo = (FIXED_INFO *) malloc(ulOutBufLen);
 if (pFixedInfo == NULL) 
  {
  //printf("Error allocating memory needed to call GetNetworkParams\n");
  return ;
  }
 }

unsigned long addr;
if (dwRetVal = GetNetworkParams(pFixedInfo, &ulOutBufLen) == NO_ERROR) 
 {
 addr=inet_addr(pFixedInfo->DnsServerList.IpAddress.String);
 if(INADDR_NONE!=addr)
  { 
  aDNSservers.Add(addr);
  }

 pIPAddr = pFixedInfo->DnsServerList.Next;
 while (pIPAddr) 
  {
  addr=inet_addr(pIPAddr->IpAddress.String);
  if(INADDR_NONE!=addr){ aDNSservers.Add(addr);}
  pIPAddr = pIPAddr->Next;
  }
 }

if(pFixedInfo!=NULL){free(pFixedInfo);}

}



};// class iDNS


#define SOCKADDR_SetPort(sa,Port)    sa.sa_data[0]= (Port & 0xFF00) >> 8;  sa.sa_data[1] = (Port & 0x00FF);


unsigned int  iConnect(iDNSResolver &DNSResolver,const ULONG iDNS_T, iString<char> &Host,const unsigned short Port,  SOCKET& so,const unsigned int TimeoutSend=10000,const unsigned int TimeoutReceive=10000)  
{                    

unsigned int ErrorCode=0;

unsigned int dns=DNSResolver.aDNS_index(Host,iDNS_T);
if(dns==UL_ERROR){return IC_ERROR_DNS;} 
 
so=socket(AF_INET,SOCK_STREAM ,IPPROTO_TCP ); 
if(so==INVALID_SOCKET){ return WSAGetLastError();} 
setsockopt(so,SOL_SOCKET,SO_RCVTIMEO,(char *) &TimeoutReceive,sizeof(TimeoutReceive));
setsockopt(so,SOL_SOCKET,SO_SNDTIMEO,(char *) &TimeoutSend,sizeof(TimeoutSend));


switch (iDNS_T)
{
 case iDNS_T_A:
  {
        //         WSAEADDRINUSE
  if(DNSResolver.aDNS[dns].a_addr_usefull_index!=UL_ERROR)
   {           
   SOCKADDR_SetPort(DNSResolver.aDNS[dns].a_addr[DNSResolver.aDNS[dns].a_addr_usefull_index],Port);
   ErrorCode=connect(so,&DNSResolver.aDNS[dns].a_addr[DNSResolver.aDNS[dns].a_addr_usefull_index],sizeof(sockaddr)); 
   if(ErrorCode==0){return 0;} 
   DNSResolver.aDNS[dns].a_addr_usefull_index=UL_ERROR; 
   }
  for(unsigned int i=0;i<DNSResolver.aDNS[dns].a_addr_count;i++)  
   {
   SOCKADDR_SetPort(DNSResolver.aDNS[dns].a_addr[i],Port);
   ErrorCode=connect(so,&DNSResolver.aDNS[dns].a_addr[i],sizeof(sockaddr)); 
   if(ErrorCode==0){DNSResolver.aDNS[dns].a_addr_usefull_index=i;return 0;} 
   }
  break;
  }
 case iDNS_T_MX:
  {
  if(DNSResolver.aDNS[dns].mx_addr_usefull_index!=UL_ERROR)
   {           
   SOCKADDR_SetPort(DNSResolver.aDNS[dns].mx_addr[DNSResolver.aDNS[dns].mx_addr_usefull_index],Port);
   ErrorCode=connect(so,&DNSResolver.aDNS[dns].mx_addr[DNSResolver.aDNS[dns].mx_addr_usefull_index],sizeof(sockaddr)); 
   if(ErrorCode==0){return 0;} 
   DNSResolver.aDNS[dns].mx_addr_usefull_index=UL_ERROR; 
   }
  for(unsigned int i=0;i<DNSResolver.aDNS[dns].mx_addr_count;i++)  
   {
   SOCKADDR_SetPort(DNSResolver.aDNS[dns].mx_addr[i],Port);
   ErrorCode=connect(so,&DNSResolver.aDNS[dns].mx_addr[i],sizeof(sockaddr)); 
   if(ErrorCode==0){DNSResolver.aDNS[dns].mx_addr_usefull_index=i;return 0;} 
   }
  break;
  }
 default:{ closesocket(so);return IC_ERROR_PARAMETER_ERROR;}
 }

if(ErrorCode==SOCKET_ERROR){closesocket(so); return  IC_ERROR_CONNECT;}
closesocket(so);
 return (ErrorCode==0? IC_ERROR_CONNECT:ErrorCode);
}








#endif  //_IUTILS_H
















