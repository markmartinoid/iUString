#pragma once
#ifndef INETCLIENT1_H
#define INETCLIENT1_H


#ifdef _WINDOWS_
#include <winsock2.h>
#include <ws2tcpip.h>
#endif//   _WINDOWS_
#include <iUString.h>
#include <iDynArray.h>
#include <iCodes.h>
#include <iNetCodes.h>
#include <iDns.h>

#ifndef NOT_USE_ZLIB1
#include <zlib.h>
#endif
//#define CALLBACK    __stdcall
//void CALLBACK  INET_ON_DATA_CALLBACK(void* data, unsigned int datasize)  
typedef void (CALLBACK* INET_ON_DATA_PROC)(void* data, unsigned int datasize);







unsigned short PortFromProtocol(const unsigned short Protocol) 
{ return Protocol;}


unsigned short Prefix_to_Protocol(const char* Prefix)
{
if(strstr(Prefix,INET_HTTP_PREFIX)!=NULL){return INET_HTTP;}
if(strstr(Prefix,INET_HTTP_PREFIX_)!=NULL){return INET_HTTP;}
if(strstr(Prefix,INET_HTTPS_PREFIX)!=NULL){return INET_HTTPS;}
if(strstr(Prefix,INET_HTTPS_PREFIX_)!=NULL){return INET_HTTPS;}
if(strstr(Prefix,INET_FTP_PREFIX)!=NULL){return INET_FTP;}
if(strstr(Prefix,INET_FTP_PREFIX_)!=NULL){return INET_FTP;}
if(strstr(Prefix,INET_MAILTO_PREFIX)!=NULL){return INET_MAIL;}
return 0;
}



const char* Protocol_to_Prefix(const unsigned short Protocol,const bool add_slashes=false)
{
switch (Protocol)
 {
 case INET_FTP:   if(add_slashes){return INET_FTP_PREFIX_;}else{return INET_FTP_PREFIX;}
 case INET_HTTP:  if(add_slashes){return INET_HTTP_PREFIX_;}else{return INET_HTTP_PREFIX;}
 case INET_HTTPS: if(add_slashes){return INET_HTTPS_PREFIX_;}else{return INET_HTTPS_PREFIX;}
 case INET_MAIL:  if(add_slashes){return INET_MAILTO_PREFIX_;}else{return INET_MAILTO_PREFIX;}
 default:break;
 }
return NULL;
}


struct iPath{
unsigned short Protocol;
iString<char> Host;
unsigned short Port;
iString<char> Path;
iString<char> Query;
iString<char> UserName;
iString<char> Password;
iString<char> Fragment;

} ;
bool iGetiPath(const iString<char> & FullPath,iPath& ipath,unsigned short defaultProtocol=INET_HTTP,unsigned short defaultPort=0 )
{  
bool retval=true; 
iString<char> fullpath(FullPath);
int pos=fullpath.IndexFirst(":");
int protocol_divider_length_add=2;//slashes
if(pos==-1)
 {
 ipath.Protocol=defaultProtocol;
 }else{                            
 ipath.Protocol=Prefix_to_Protocol(fullpath.SubString(0,pos-1).Buffer());
 if(ipath.Protocol==0){ipath.Protocol=defaultProtocol;}
 if(ipath.Protocol==INET_MAIL){protocol_divider_length_add=0;}
 fullpath.Remove(0,pos+protocol_divider_length_add+1);
 }

ipath.UserName="";ipath.Password="";
int pos_a=fullpath.IndexFirst("@");
if(pos_a==0){fullpath.Remove(0,1); pos_a=-1;}
if(pos_a!=-1)
 {
 iString<char> name_password=fullpath.SubString(0,pos_a-1);
 pos=name_password.IndexFirst(":");
 if(pos>0)
  {
  ipath.UserName=name_password.SubString(0,pos-1);
  ipath.Password=name_password.SubString(pos+1,name_password.GetLength()-1);
  }else{
  ipath.UserName=name_password;
  }
 fullpath.Remove(0,pos_a+1);
 }


int pos_path;
int pos_query;
int pos_fragment;
int pos_port;

pos_port=fullpath.IndexFirst(":");
pos_path=fullpath.IndexFirst("/");
pos_query=fullpath.IndexFirst("?");
pos_fragment=fullpath.IndexFirst("#");

pos=fullpath.GetLength();

if((pos_port<pos)&&(pos_port!=-1)){pos=pos_port;}
if((pos_path<pos)&&(pos_path!=-1)){pos=pos_path;}
if((pos_query<pos)&&(pos_query!=-1)){pos=pos_query;}
if((pos_fragment<pos)&&(pos_fragment!=-1)){pos=pos_fragment;}
pos--;
int start=0;
if(pos!=-1)
 {
 ipath.Host=fullpath.SubString(0,pos);
 start=pos+1;
 }else{

 ipath.Host="";
 retval=false;
 }



if(pos_port==-1)
 {
 ipath.Port=defaultPort;
 }else{    
 pos=fullpath.GetLength();
 if((pos_path<pos)&&(pos_path!=-1)){pos=pos_path;}
 if((pos_query<pos)&&(pos_query!=-1)){pos=pos_query;}
 if((pos_fragment<pos)&&(pos_fragment!=-1)){pos=pos_fragment;}
 pos--;
 ipath.Port=fullpath.SubString(start,pos).AsInt(); 
 start=pos+1;
 if(ipath.Port==0){ipath.Port=defaultPort;}
 }
if(ipath.Port==0){ipath.Port=PortFromProtocol(ipath.Protocol);}        

if(pos_path==-1)
 {
 ipath.Path="/";
 }else{    
 pos=fullpath.GetLength();
 if((pos_query<pos)&&(pos_query!=-1)){pos=pos_query;}
 if((pos_fragment<pos)&&(pos_fragment!=-1)){pos=pos_fragment;}
 pos--;                       //start
 ipath.Path=fullpath.SubString(pos_path,pos); 
 start=pos+1;
 }


if(pos_query==-1)
 {
 ipath.Query="";
 }else{    
 pos=fullpath.GetLength();
 if((pos_fragment<pos)&&(pos_fragment!=-1)){pos=pos_fragment;}
 pos--;                        //start
 ipath.Query=fullpath.SubString(pos_query+1,pos); 
 start=pos+1;
 }


if(pos_fragment==-1)
 {
 ipath.Fragment="";
 }else{    
 pos=fullpath.GetLength();
 pos--;                       //start
 ipath.Fragment=fullpath.SubString(pos_fragment+1,pos); 
 }


return retval;
}









struct sock{
SOCKET  so;
bool active;
int ErrorCode;
};

/*
struct hostport{
iString<char> sPort;
unsigned int port;
iDynArray<sock> aSock;
iDynArray<ADDRINFOW> aAddrInfo;
unsigned int         aAddrInfo_index;
int ErrorCode;
//Acessible
};

struct host{
iString<char> Host;
iDynArray<hostport> aPort;
//Acessible
};
*/

 
class iHTTP_Header{
public:
unsigned int ResponceCode; 
unsigned int ContentEncoding; 
unsigned int ContentLength;
unsigned int Connection;
unsigned int KeepAlive_timeout;
unsigned int KeepAlive_max;
unsigned int ContentType;
unsigned int LastModified;
unsigned int CharSet;
iString<char> Location;
bool ContentLengthPresent;

void Clear()
{
ResponceCode=0;
ContentEncoding=0;
ContentLength=0;
Connection =0;
KeepAlive_timeout=0;
KeepAlive_max=0;
ContentType  =0;
LastModified =0;
CharSet=0;
Location.SetLength(0);
ContentLengthPresent=false;
}

};


//Last-Modified: Mon, 02 Sep 2013 00:48:02 GMT
//month        = "Jan" | "Feb" | "Mar" | "Apr"  | "May" | "Jun" | "Jul" | "Aug" | "Sep" | "Oct" | "Nov" | "Dec"
unsigned int iHTTPtime_ui(char* str,unsigned int str_max_index)
{
if(str_max_index!=28){return 0;}
unsigned int x;
unsigned int  retval=0;

//enable overvoflow
x=iStrAsUInt(&str[12],3)-1970;
if(x==0){return 0;}
if(x>2480){x=2480;}
retval=x<<23; //year 9 bit

switch (str[8])
{
case 'J':{if(str[9]=='a'){x=1;}else{if(str[10]=='n'){x=6;}else{x=7;} }break;}
case 'F':{x=2;break;}
case 'M':{if(str[10]=='r'){x=3;}else{x=5;}break;}
case 'A':{if(str[9]=='p'){x=4;}else{x=8;}break;}
case 'S':{x=9;break;}
case 'O':{x=10;break;}
case 'N':{x=11;break;}
case 'D':{x=12;break;}
default:return 0;
 break;
}
retval|=(x<<19);

x=iStrAsUInt(&str[5],1);
if((x==0)||(x>31)){return 0;}
retval|=x<<15;

x=iStrAsUInt(&str[17],1);
if((x==0)||(x>23)){return 0;}
retval|=(x<<8);

x=iStrAsUInt(&str[20],1);
if((x==0)||(x>59)){return 0;}
retval|=x;

return retval;
}


unsigned int iHtmlHeader(char* buffer,unsigned int buffer_size, iHTTP_Header& h)
{
if(&h==NULL){return 0;} 
h.Clear();
if(buffer_size<15){return 0;}
unsigned int start=0;
unsigned int buffer_max=buffer_size-1;
if(iStrEqual_s<char>(&buffer[start],"HTTP/1.1 ",9)){start=9;}else{return 0;} 
h.ResponceCode=iStrAsUInt< char>(&buffer[start],2);
while((buffer[start]!=iEOFn)&&(start<buffer_max))
 {start++;}
start++;
unsigned int end;
while(start<buffer_max)  
 {
 if(buffer[start]==iEOFr){start++;break;}
 end=start;
 while((buffer[end]!=iEOFr)&&(end<buffer_max)){end++;}
 if(end==buffer_max){break;}
 end--;

 if(h.Connection==0)
  {
  if((end-start)>=16){
  if(iStrEqual_s<char>(&buffer[start],"Connection: ",12))
   {
   start+=12;
   if((end-start)==9){if(iStrEqual_s<char>(&buffer[start],"Keep-Alive",10)){h.Connection=INET_CONNECTION_KEEP_ALIVE;start=end+3;continue;}}
   if((end-start)==4) {if(iStrEqual_s<char>(&buffer[start+1],"lose"     ,4)) {h.Connection=INET_CONNECTION_CLOSE;start=end+3;continue;}}  //close Close
   start=end+3;continue;
   }
  }}

 if(h.ContentEncoding==0)
  {
  if((end-start)>=21){
  if(iStrEqual_s<char>(&buffer[start],"Content-Encoding: ",18))
   {
   start+=18;
   if((end-start)==6){if(iStrEqual_s<char>(&buffer[start],"chunked",7)){h.ContentEncoding=INET_CONTENT_ENCODING_CHUNKED;start=end+3;continue;}}
   if((end-start)==6){if(iStrEqual_s<char>(&buffer[start],"deflate",7)){h.ContentEncoding=INET_CONTENT_ENCODING_DEFLATE;start=end+3;continue;}}
   if((end-start)==3){if(iStrEqual_s<char>(&buffer[start],"gzip"   ,4)){h.ContentEncoding=INET_CONTENT_ENCODING_GZIP;start=end+3;continue;}}
  // if((end-start)==4){if(iStrEqual_s<char>(&buffer[start],"sdch",   4)){h.ContentEncoding=INET_CONTENT_ENCODING_SDCH;start=end+3;continue;}}
   if((end-start)>1){h.ContentEncoding=INET_CONTENT_ENCODING_OTHER;}else{h.ContentEncoding=INET_CONTENT_ENCODING_NONE;}
   start=end+3;continue;
   }
  }}

 if(h.ContentLength==0)
  {
  if((end-start)>=17){
  if(iStrEqual_s<char>(&buffer[start],"Content-Length: ",16))
   {
   h.ContentLengthPresent=true;
   start+=16;
   h.ContentLength=iStrAsUInt(&buffer[start],end-start);
   start=end+3;continue;
   }
  }}

 if(h.Location.GetLength()==0)
  {
  if((end-start)>10){
  if(iStrEqual_s<char>(&buffer[start],"Location: ",10))
   {
   start+=10;
   h.Location.SetLength(end-start+2);
   memcpy((void*)h.Location.Buffer(),&buffer[start],h.Location.GetLength());
   start=end+3;continue;
   }
  }}



 if(h.ContentType==0)
  {
  if((end-start)>=14){
  if(iStrEqual_s<char>(&buffer[start],"Content-Type: ",14))
   {
   start+=14;
   if((end-start)==7){if(iStrEqual_s<char>(&buffer[start],"text/xml",8)){h.ContentType=INET_CONTENT_TYPE_TEXTXML;start=end+3;continue;}}
   if((end-start)==19){if(iStrEqual_s<char>(&buffer[start],"multipart/byteranges",20)){h.ContentType=INET_CONTENT_TYPE_MULTIPART_BINARY;start=end+3;continue;}}
   if((end-start)>=9)
    {
    if(iStrEqual_s<char>(&buffer[start],"text/html",9))
     {
     h.ContentType=INET_CONTENT_TYPE_TEXTHTML;
     start+=11;
     if((end-start)>=7){if(iStrEqual_s<char>(&buffer[start],"charset=",8))
      {
      start+=8;
      if((end-start)==4)
       {
       if(iStrEqual_s<char>(&buffer[start],CP_UTF8_,5)){h.CharSet=CP_UTF8;start=end+3;continue;}
       if(iStrEqual_s<char>(&buffer[start],CP_UTF7_,5)){h.CharSet=CP_UTF7;start=end+3;continue;}
     
       }//4
      if((end-start)==11)
       {
       if(iStrEqual_s<char>(&buffer[start],CP_W1250_,12)){h.CharSet=CP_W1250;start=end+3;continue;}
       if(iStrEqual_s<char>(&buffer[start],CP_W1252_,12)){h.CharSet=CP_W1252;start=end+3;continue;}
       if(iStrEqual_s<char>(&buffer[start],CP_W1251_,12)){h.CharSet=CP_W1251;start=end+3;continue;}
       }//11

      }}
     }
    }
   h.ContentType=INET_CONTENT_TYPE_OTHER;
   start=end+3;continue;
   }
  }}


  if((h.KeepAlive_timeout==0)||(h.KeepAlive_max==0))
  {
  if((end-start)>=17){
  if(iStrEqual_s<char>(&buffer[start],"Keep-Alive: ",12))
   {
   start+=12;
   h.KeepAlive_timeout=iStrValue(&buffer[start],end,"timeout",7);
   h.KeepAlive_max=iStrValue(&buffer[start],end,"max",3);
   start=end+3;continue;
   }
  }}



 if(h.LastModified==0)
  {
  if((end-start)==43){
  if(iStrEqual_s<char>(&buffer[start],"Last-Modified: ",15))
   {
   start+=15;
   h.LastModified=iHTTPtime_ui(&buffer[start],end-start);
   start=end+3;continue;
   }
  }}



 start=end+3;continue;
 }
if(buffer[start]==iEOFn){start++;}else{return 0;} 
return start;
}
/*Accept-Ranges: bytes       Date:       Vary: Accept-Encoding,User-Agent
lOCATION
HTTP/1.1 206 Partial content
Content-type: multipart/byteranges; boundary=THIS_STRING_SEPARATE>              
 C programs typically use the zlib library 
 time_t t;struct tm gm, loc;struct timeval tv;     mktime      
*/
  
    



class iNetClient1
{

iDNSResolver* DNSResolver;


char * Buffer;
unsigned int  BufferSize; unsigned int  BufferMax;
#ifndef NOT_USE_ZLIB1
z_stream zlib_stream;
unsigned int   zlib_init_count;
#endif
public:

unsigned int TimeoutSend;//ms  
unsigned int TimeoutReceive; 




 iNetClient1(iDNSResolver* pDNSResolver,unsigned int buffer_size=4096)
 {

 TimeoutSend=10000; 
 TimeoutReceive=10000;
 BufferSize=buffer_size;BufferMax=BufferSize-1;
 Buffer=(char*)malloc(BufferSize);
 #ifndef NOT_USE_ZLIB1
 zlib_init_count=0;
 #endif
 DNSResolver=pDNSResolver;
 }

 ~iNetClient1(void)
 {
 if(Buffer!=NULL){free(Buffer);}
///////////////// WSACleanup();
#ifndef NOT_USE_ZLIB1
 if(zlib_init_count!=0){inflateEnd(&zlib_stream);}
#endif
 }




#ifndef NOT_USE_ZLIB1
bool InitZlib()
{
if(zlib_init_count==1){return true;}    
zlib_stream.zalloc = Z_NULL;
zlib_stream.zfree = Z_NULL;
zlib_stream.opaque = Z_NULL;
zlib_stream.avail_in = 0;
zlib_stream.next_in= Z_NULL;
//if(inflateInit(&zlib_stream)!=Z_OK){zlib_init_count=0xFFFFFFF;}else{zlib_init_count=1;} 
if(inflateInit2(&zlib_stream,32+MAX_WBITS)!=Z_OK){zlib_init_count=0xFFFFFFF;return false;}else{zlib_init_count=1;return true;} 
// Add 32 to windowBits to enable zlib and gzip decoding with automatic header detection, or add 16 to decode only the gzip format (the zlib format will return a Z_DATA_ERROR). If a gzip stream is being decoded, strm->adler is a crc32 instead of an adler32.
return true;
}
#endif 


 


bool HeaderGP(iString<char> &hdr,iPath &Path,iString<char> &Referer,unsigned int HeaderType=INET_HEADER_GET,unsigned int BrowserType=INET_BROVSER_CHROME_537 )
{
hdr.Allocate(512,false);
if(HeaderType==INET_HEADER_GET){hdr="GET ";}else{hdr="POST ";}
hdr+=Path.Path;

if(Path.Query.GetLength()!=0)
 {
 hdr+="?";
 hdr+=Path.Query;
 }

//if(Path.Fragment.GetLength()!=0)
// {
// hdr+="#";
// hdr+=Path.Fragment;
// }


hdr+=" HTTP/1.1";
hdr+=iEOF;
hdr+="Host: ";
hdr+=Path.Host;  
hdr+=iEOF;

#ifndef iSIMPLEHEADERS
switch (BrowserType)
 {
 case INET_BROVSER_CHROME_537:
  {
  hdr+= "Connection: close";//Connection: keep-alive  
  hdr+=iEOF;
  hdr+= "User-Agent: Mozilla/5.0 (Windows NT 6.0) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.64 Safari/537.31"; 	
  hdr+=iEOF;
  hdr+= "Accept:  text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";	
  hdr+=iEOF;
  if(Referer.GetLength()!=0){ hdr+= "Referer: ";  hdr+=Referer;  hdr+=iEOF;}
  #ifndef NOT_USE_ZLIB1
  hdr+= "Accept-Encoding: gzip,deflate"; // //Accept-Encoding: gzip,deflate,sdch
  #endif
  #ifdef NOT_USE_ZLIB1
  hdr+= "Accept-Encoding: deflate"; 
  #endif
  hdr+=iEOF;
  hdr+= "Accept-Language: en-US;q=0.6,en;q=0.4"; 
  hdr+=iEOF;
  hdr+= "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3"; 
  hdr+=iEOF;
  //Cookie: PREF=ID=7eb1ac44abdcbaf5:U=1cd37b92582c9822:FF=0:TM=1326140021:LM=1353866512:S=1ITab7BjHa_hn4vY; NID=66=L6-mQq0s6eMKZ-AzARdb3JHeRO6H3MKGt_r3aTePmfL3AHSZboEgP4Tzbs6FPukek1kxdpPy03mHgFP-D-DVM6oWwwOXwUUWuC1Tr8_Ql7_63fPDhCPThGom88zc_Xxu	
  break;
  }
 case INET_BROVSER_IE_7:
  {
  hdr+= "Accept: */*";	
  hdr+=iEOF;
  if(Referer.GetLength()!=0){ hdr+= "Referer: ";  hdr+=Referer;  hdr+=iEOF;}
  hdr+= "Accept-Language: en";
  hdr+=iEOF;
  hdr+= "UA-CPU: x86";  
  hdr+=iEOF;
  #ifndef NOT_USE_ZLIB1
  hdr+= "Accept-Encoding: deflate,gzip";
  #endif
  #ifdef NOT_USE_ZLIB1
  hdr+= "Accept-Encoding: deflate"; 
  #endif
  hdr+=iEOF;
  hdr+= "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0; SLCC1; .NET CLR 2.0.50727; .NET CLR 3.0.04506)"; 
  hdr+=iEOF;
  hdr+= "Connection: close";
  hdr+=iEOF;
  //Cookie: PREF=ID=7373ba6f51e2e498:U=59021f0a67810a05:FF=0:NW=1:TM=1326139913:LM=1326139934:S=ykx5u4ZcVRNdtsKP 
  break;
  }
 case INET_BROVSER_FIREFOX_3:
  {
#endif
  hdr+= "Cache-Control: no";
  hdr+=iEOF;
  //hdr+= 'Accept-Ranges: bytes'."\r\n";
  //if ($range != 0) $query .= 'Range: bytes='.$range.'-'."\r\n"; // -500
  if(Referer.GetLength()!=0){ hdr+= "Referer: ";  hdr+=Referer;  hdr+=iEOF;}
  hdr+= "User-Agent: Mozilla/5.0 Firefox/3.6.12"; 
  hdr+=iEOF;
  hdr+= "Accept: */*"; //text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
  hdr+=iEOF;
  hdr+= "Accept-Language: en-us,en;q=0.5";
  hdr+=iEOF;
  #ifndef NOT_USE_ZLIB1
  hdr+= "Accept-Encoding: deflate,gzip";
  #endif
  #ifdef NOT_USE_ZLIB1
  hdr+= "Accept-Encoding: deflate"; 
  #endif
  hdr+=iEOF;
  hdr+= "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7";
  hdr+=iEOF;
  hdr+= "Connection: close";
  hdr+=iEOF;
#ifndef iSIMPLEHEADERS
  break;
  }
 default:
 return false;
 break;
}
#endif

if(HeaderType==INET_HEADER_POST)// && !empty($postdata_str)) 
 {  /*
 $postdata_str = substr($postdata_str, 0, -1); 
 $query .= 'Content-Type: application/x-www-form-urlencoded' . $EOF; 
 $query .= 'Content-Length: '. strlen($postdata_str) . $EOF . $EOF; 
 $query .= $postdata_str; */
 }else{
  hdr+=iEOF;
 }

return true;
}




//can not null-terminated                   1                      2                3                         4                            5                         6                            7                                      8                     
unsigned int http_get(const iString<char> &path, iString<char> &Referer,char* out_buffer,unsigned int &out_buffer_length,iHTTP_Header&  Header, INET_ON_DATA_PROC* callback,unsigned short defaultProtocol=INET_HTTP,unsigned short defaultPort=0,char* RequestText=NULL )
{
unsigned int ErrorCode;
SOCKET so;
iPath Path;
if(!iGetiPath(path,Path,defaultProtocol,defaultPort)){return IC_ERROR_PATH;}

iString<char> hdr; 
if (RequestText!=NULL)
 {
 hdr=RequestText;
 }else{
 if(!HeaderGP(hdr,Path,Referer,INET_HEADER_GET,INET_BROVSER_CHROME_537)){return IC_ERROR_HEADER;}
 }
////memcpy(buffer,hdr.Buffer(),hdr.GetLength());  buffer_length=hdr.GetLength();   return 0;




ErrorCode=iConnect(*DNSResolver,iDNS_T_A, Path.Host,Path.Port,so,TimeoutSend,TimeoutReceive);  
if(ErrorCode!=0){return ErrorCode;} 



if(SOCKET_ERROR==send(so,hdr.Buffer(),hdr.GetLength(),0))
 {
 closesocket(so); 
 return  WSAGetLastError();
 }

//if(SOCKET_ERROR==shutdown(so, SD_SEND))closesocket(so);return  WSAGetLastError();  // shutdown the connection since no more data will be sent
 

unsigned int ReceivedTotal=0; //have is the amount of data returned from deflate().



//iHTTP_Header  Header;
bool header_received=false;
unsigned int BufferIndex=0;

   unsigned int ChunkSize;
   char chunk[INET_CHUNK_CHAR_MAX+1];chunk[INET_CHUNK_CHAR_MAX]=0;
   unsigned int Chunk_CharCount=0;
   bool wait_ChunkSize=true;

unsigned int data_length;
unsigned int BufferReceived;


unsigned int  recv_call_count=0;
unsigned int  max_recv_call_count;
if(out_buffer_length<BufferSize){max_recv_call_count=3;}else{max_recv_call_count  =out_buffer_length*2/BufferSize;}
unsigned int  redirect_count=0;
unsigned int  BufferReceivedTotal=0; //packed if encoded  or unpacked  

do
 {                                        // out_buffer_length-Received
 if(header_received){if(BufferReceivedTotal>=Header.ContentLength)break;    }
 BufferReceived = recv(so, &Buffer[0],BufferSize , 0); 
 recv_call_count++; if(recv_call_count==max_recv_call_count){break;}
 if(BufferReceived==0){break;}
 if (SOCKET_ERROR==BufferReceived){ErrorCode=IC_ERROR_SOCKET;break;  }
 if (out_buffer_length<=ReceivedTotal){ErrorCode=IC_BUFFER_TO_SMALL;break; }
 if(header_received){BufferIndex=0;}else
  {
   BufferIndex=iHtmlHeader(Buffer,BufferSize ,Header);
   if(BufferIndex>=BufferSize){ErrorCode=IC_ERROR_INDEX_OUT_OF_BOUNDS;break;}
   if(Header.ResponceCode==INET_RESPONCECODE_301_REDIRECT)
    {
    redirect_count++;if(redirect_count==INET_MAX_REDIRECT_COUNT){ErrorCode=IC_TO_MANY_REDIRECTS;break;}
    if(!iGetiPath(Header.Location,Path,defaultProtocol,defaultPort)){ErrorCode=IC_ERROR_PATH;break;}
    if(!HeaderGP(hdr,Path,Referer,INET_HEADER_GET,INET_BROVSER_CHROME_537)){ErrorCode= IC_ERROR_HEADER;break;}
    
    if(Header.Connection==INET_CONNECTION_KEEP_ALIVE)
     {
      while(BufferReceived==BufferSize){BufferReceived = recv(so, &Buffer[0],BufferSize , 0);recv_call_count++;if(recv_call_count==max_recv_call_count){break;}}
     }else{
      ErrorCode=iConnect(*DNSResolver,iDNS_T_A, Path.Host,Path.Port,so,TimeoutSend,TimeoutReceive); 
      if(ErrorCode!=0){break;}
     }
    if(SOCKET_ERROR==send(so,hdr.Buffer(),hdr.GetLength(),0)){ErrorCode=IC_ERROR_SOCKET;break;}
    continue; 
    }
   header_received=true;
  } 
 BufferReceivedTotal+=(BufferReceived-BufferIndex);
 if(Header.ContentEncoding<=INET_CONTENT_ENCODING_OTHER)
  {   
  if((BufferReceived-BufferIndex+ReceivedTotal)>out_buffer_length){data_length=out_buffer_length-ReceivedTotal;}else{data_length=BufferReceived-BufferIndex;}
  memcpy(&out_buffer[ReceivedTotal],  &Buffer[BufferIndex],data_length);
  ReceivedTotal+=(BufferReceived-BufferIndex);  
  continue;
  }//INET_CONTENT_ENCODING_OTHER

 if(Header.ContentEncoding==INET_CONTENT_ENCODING_CHUNKED)
  {
  while((BufferIndex!=BufferReceived)&&(out_buffer_length!=ReceivedTotal))
   {
   while(wait_ChunkSize)
    {
    if(Chunk_CharCount==0){if((Buffer[BufferIndex]==iEOFr)||(Buffer[BufferIndex]==iEOFn)){if(BufferIndex==BufferReceived){break;}else{BufferIndex++;}}}
    if(Buffer[BufferIndex]==iEOFr){ChunkSize=iStrAsUInt<char>(&chunk[0],Chunk_CharCount-1); Chunk_CharCount=0;  wait_ChunkSize=false;BufferIndex+=4;break;}
    chunk[ChunkSize]=Buffer[BufferIndex];
    Chunk_CharCount++;
    BufferIndex++;
    }
   if(BufferIndex==BufferReceived){continue;}

   if(BufferReceived-BufferIndex<=ChunkSize){data_length=ChunkSize;}else{data_length=BufferReceived-BufferIndex;}
   if((data_length+ReceivedTotal)>out_buffer_length){data_length=out_buffer_length-ReceivedTotal;ErrorCode=IC_BUFFER_TO_SMALL;}
   ChunkSize-=data_length;
   if(ChunkSize==0){wait_ChunkSize=true;}
   if(data_length!=0){memcpy(&out_buffer[ReceivedTotal],  &Buffer[BufferIndex],data_length);}
   ReceivedTotal+=data_length;
   BufferIndex+=data_length;
   }   
  continue;
  }//INET_CONTENT_ENCODING_CHUNKED

 #ifndef NOT_USE_ZLIB1
 if((Header.ContentEncoding==INET_CONTENT_ENCODING_DEFLATE)||(Header.ContentEncoding==INET_CONTENT_ENCODING_GZIP))
  {
  if(!InitZlib()){ErrorCode=IC_ERROR_ZLIB;break;}
  zlib_stream.avail_in=BufferReceived-BufferIndex;
  zlib_stream.next_in =(Bytef*)&Buffer[BufferIndex];
  zlib_stream.avail_out = out_buffer_length-ReceivedTotal;
  if(zlib_stream.avail_out>0)
  {
   if(ReceivedTotal<out_buffer_length)
    {zlib_stream.next_out = (Bytef*)&out_buffer[ReceivedTotal];
    }else{ErrorCode=IC_BUFFER_TO_SMALL;break;}
  }else{
   out_buffer_length=ReceivedTotal;
   ErrorCode=  IC_BUFFER_TO_SMALL;
   break;
  }

  iResult=inflate(&zlib_stream, Z_NO_FLUSH);
  ReceivedTotal=zlib_stream.total_out;
  if((iResult==Z_NEED_DICT)||(iResult==Z_DATA_ERROR)||(iResult==Z_MEM_ERROR)||(iResult==Z_VERSION_ERROR) ||(iResult==Z_STREAM_ERROR )){out_buffer_length=ReceivedTotal;inflateReset (&zlib_stream);ErrorCode= IC_ERROR_ZLIB;break;}
  if(iResult==Z_STREAM_END){break;}   
  continue;
  }//INET_CONTENT_ENCODING_GZIP
 #endif



 } while((BufferReceived>0)&&(ReceivedTotal<out_buffer_length));

 #ifndef NOT_USE_ZLIB1
if(Header.ContentEncoding==INET_CONTENT_ENCODING_DEFLATE){inflateReset (&zlib_stream);}
#endif
 

           






out_buffer_length=ReceivedTotal;
// DisconnectEx  allows the socket handle to be reused
closesocket(so);  // if(so!=INVALID_SOCKET){closesocket(so);so=INVALID_SOCKET;}  
//!!!never issue closesocket on s concurrently with another Winsock function call.
return ErrorCode;
}

};



#endif //INETCLIENT1_H


/*
getsockopt
ioctlsocket Controls the I/O mode of a socket.
    FIONBIO  nonblocking mode 
    FIONREAD returns the amount of data that can be read in a single call to the recv 

setsockopt
  SO_RCVBUF SO_SNDBUF SO_REUSEADDR
  SO_RCVTIMEO SO_SNDTIMEO      DWORD Sets the timeout, in milliseconds, for blocking receive calls 
  SO_RCVLOWAT SO_SNDLOWAT sets the minimum number of bytes to process for socket output operations.

WSAIoctl
  FIONBIO  FIONREAD 
  SIO_ENABLE_CIRCULAR_QUEUEING oldest message in the queue should be eliminated 
  SIO_FLUSH Discards current contents of the sending queue 
        server SIO_IDEAL_SEND_BACKLOG_QUERY 
  SIO_KEEPALIVE_VALS
  SIO_RCVALL Enables a socket to receive all IPv4 or IPv6 packets passing throuigh a network interface


WSAConnect
WSAGetLastError
WSAPoll Determines status of one or more sockets. 
WSAAsyncGetHostByAddr
 shutdown function disables sends or receives on a socket


htonl Converts a u_long from host to TCP/IP network byte order  
htons Converts a u_short from host to TCP/IP network byte order  
ntohl Converts a u_long from TCP/IP network order to host byte order 
ntohs Converts a u_short from TCP/IP network byte order to host byte order 

inet_ntoa Converts an (IPv4) Internet network address into a string in Internet standard dotted format. 
inet_ntop. converts an IPv4 or IPv6 Internet network address into a string in Internet standard format. The ANSI version of this function is inet_ntop. 
inet_pton. text presentation form into its numeric binary 

Winsock RIO Windows Server 2012
TransmitFile
TransmitPackets function transmits in-memory data or file data over a connected socket
*/