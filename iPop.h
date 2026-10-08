#pragma once

#ifndef _IPOP_H
#define _IPOP_H

#ifdef _WINDOWS_
#include <winsock2.h>
#include <ws2tcpip.h>
#endif//   _WINDOWS_
#include "iUString.h"
#include "iDynArray.h"
#include "iCodes.h"
#include "iNetCodes.h"
#include <iStringUtil.h>
#include <time.h>
#include <iDns.h>
#include <stdio.h>


const char* i_domain_from_email(const char* email_from)
{
int pos=iStrPos<char>(email_from,"@");
if(pos==-1){return NULL;}
return &email_from[pos+1];
}


void i_htmlspecialchars(iString<char> &str)
{
//��� �������������� ���� HTML ��������� ����������� htmlentities(). 
//"'" (��������� �������) ������������� � '&#039;' ������ � ������ ENT_QUOTES. 
str.Replace("&","'&amp;");
str.Replace("\"","'&quot;"); //� ������ ENT_NOQUOTES is not set. 
str.Replace("<","&lt;");
str.Replace(">","&gt;");
}
void i_htmlspecialchars(iString<wchar_t> &str)
{
//��� �������������� ���� HTML ��������� ����������� htmlentities(). 
//"'" (��������� �������) ������������� � '&#039;' ������ � ������ ENT_QUOTES. 
str.Replace(L"&",L"'&amp;");
str.Replace(L"\"",L"'&quot;"); //� ������ ENT_NOQUOTES is not set. 
str.Replace(L"<",L"&lt;");
str.Replace(L">",L"&gt;");
}





template <typename StringType> void i_htmlentities(iString<StringType> &str)
{
i_htmlspecialchars(str);

str.Replace("\r\n","</br>"); str.Replace("\n","</br>");str.Replace("\r","</br>");            ///&#13;
str.Replace("\x09","&nbsp;&nbsp;&nbsp;&nbsp;");    ///// http://cpp.comsci.us/etymology/literals.html
str.Replace("\x20","&nbsp;"); //32

str.Replace("\xAE","&reg;"); //174
str.Replace("\xB0","&deg;"); //176


unsigned int c_more_than_127Count=0;
for(unsigned int i=0;i<str.Length();i++)
 {
 if(str[i]>127){c_more_than_127Count++;}
 }
if(c_more_than_127Count==0){return};

str.SetLength(str.GetLength()+ c_more_than_127Count*5);
char buf[8];
iString<StringType> temp_str;temp_str.Allocate(7); 
for(unsigned int i=0;i<str.Length();i++)
 {
 if(str[i]>127)
  {
  temp_str.SetLength(2);
  temp_str[0]=38;//&
  temp_str[1]=35;//#
  temp_str+=iToString<StringType>(str[i],&buf[0],sizeof(buf));
  temp_str.SetLength(temp_str.GetLength()+1);
  temp_str[temp_str.GetLength()-1]=59;//;
  str.ReplaceCharAt(i,&buf[0]);
  }
 }
}



#define iBASE64_FLAG_NONE	0
#define iBASE64_FLAG_NOPAD	1
#define iBASE64_FLAG_NOCRLF  2

int iBase64EncodeGetRequiredLength(int nSrcLen, uint32_t dwFlags = iBASE64_FLAG_NONE)
{
//	__int64 nSrcLen4=static_cast<__int64>(nSrcLen)*4;  ATLENSURE(nSrcLen4 <= INT_MAX);
	
 int nSrcLen4=nSrcLen*4;
	int nRet = nSrcLen4/3;

	if ((dwFlags & iBASE64_FLAG_NOPAD) == 0)
		nRet += nSrcLen % 3;

	int nCRLFs = nRet / 76 + 1;
	int nOnLastLine = nRet % 76;

	if (nOnLastLine)
	{
		if (nOnLastLine % 4)
			nRet += 4-(nOnLastLine % 4);
	}

	nCRLFs *= 2;

	if ((dwFlags & iBASE64_FLAG_NOCRLF) == 0)
		nRet += nCRLFs;

	return nRet;
}

BOOL iBase64Encode(const BYTE *pbSrcData,int nSrcLen, char* szDest,int *pnDestLen,	uint32_t dwFlags = iBASE64_FLAG_NONE) 
{
	static const char s_chBase64EncodingTable[64] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
		'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',	'h',
		'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
		'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

	if (!pbSrcData || !szDest || !pnDestLen)
	{
		return FALSE;
	}

	if(*pnDestLen < iBase64EncodeGetRequiredLength(nSrcLen, dwFlags))
	{
		return FALSE;
	}

	int nWritten( 0 );
	int nLen1( (nSrcLen/3)*4 );
	int nLen2( nLen1/76 );
	int nLen3( 19 );

	for (int i=0; i<=nLen2; i++)
	{
		if (i==nLen2)
			nLen3 = (nLen1%76)/4;

		for (int j=0; j<nLen3; j++)
		{
			uint32_t dwCurr(0);
			for (int n=0; n<3; n++)
			{
				dwCurr |= *pbSrcData++;
				dwCurr <<= 8;
			}
			for (int k=0; k<4; k++)
			{
				BYTE b = (BYTE)(dwCurr>>26);
				*szDest++ = s_chBase64EncodingTable[b];
				dwCurr <<= 6;
			}
		}
		nWritten+= nLen3*4;

		if ((dwFlags & iBASE64_FLAG_NOCRLF)==0)
		{
			*szDest++ = '\r';
			*szDest++ = '\n';
			nWritten+= 2;
		}
	}

	if (nWritten && (dwFlags & iBASE64_FLAG_NOCRLF)==0)
	{
		szDest-= 2;
		nWritten -= 2;
	}

	nLen2 = (nSrcLen%3) ? (nSrcLen%3 + 1) : 0;
	if (nLen2)
	{
		uint32_t dwCurr(0);
		for (int n=0; n<3; n++)
		{
			if (n<(nSrcLen%3))
				dwCurr |= *pbSrcData++;
			dwCurr <<= 8;
		}
		for (int k=0; k<nLen2; k++)
		{
			BYTE b = (BYTE)(dwCurr>>26);
			*szDest++ = s_chBase64EncodingTable[b];
			dwCurr <<= 6;
		}
		nWritten+= nLen2;
		if ((dwFlags & iBASE64_FLAG_NOPAD)==0)
		{
			nLen3 = nLen2 ? 4-nLen2 : 0;
			for (int j=0; j<nLen3; j++)
			{
				*szDest++ = '=';
			}
			nWritten+= nLen3;
		}
	}

	*pnDestLen = nWritten;
	return TRUE;
}


template <typename StringType> bool  mime_header_encode(iString<StringType> &str,iString<char> &dest, const char* DestCharSet="utf-8",bool ReplaceHTMLSpecialChars=false) 
{
//stripslashes
if (ReplaceHTMLSpecialChars){i_htmlspecialchars(str);}
iString<char> temp;
if(DestCharSet=="utf-8")
 {
 temp.AppendAsUTF8((StringType*)str.Buffer());
 }else{
 temp.AppendAsChar((StringType*)str.Buffer());
 }

// iBASE64_FLAG_NONE	0   iBASE64_FLAG_NOPAD	1  iBASE64_FLAG_NOCRLF

uint32_t flags=iBASE64_FLAG_NOPAD|iBASE64_FLAG_NOCRLF; //iBASE64_FLAG_NOPAD

int DestLength=iBase64EncodeGetRequiredLength(temp.GetLength()*sizeof(char), flags);
dest.Allocate(DestLength+8+iStrLen<char>(DestCharSet),false);
if(dest.GetAllocated()<(unsigned int)DestLength+8+iStrLen<char>(DestCharSet)){return false;}
dest="=?";
dest+= DestCharSet;
dest+="?B?";
if(!iBase64Encode((BYTE *)temp.Buffer(),temp.GetLength(),(char*) (dest.Buffer()+ dest.GetLength())   ,&DestLength, flags)){return false;} 
dest.SetLength(dest.GetLength()+DestLength,false);
dest+="?=";
return true;
}

 
bool chunk_split(char* data,unsigned long dataLength,char* out_data,unsigned long &out_data_Length,unsigned int chunklen = 76,char* end = "\r\n") 
{
// chunk_split will also add the break _after_ the last occurence.
unsigned long whole_split_count=dataLength/chunklen;
unsigned long last_bytes=dataLength- (whole_split_count*chunklen);
unsigned long endLength=iStrLen<char>(end); 
 


unsigned long xLength=whole_split_count*(chunklen+endLength);
if(last_bytes>0){xLength+=(last_bytes+endLength);}
if(out_data_Length==0){out_data_Length=xLength;return false;}
if(out_data_Length<xLength){return false;}

unsigned long out_pos=0;
unsigned long in_pos=0;
for(unsigned int i=0;i<whole_split_count;i++)
 {
 memcpy(&out_data[out_pos],&data[in_pos],chunklen);
 out_pos+=chunklen;
 in_pos+=chunklen;
 memcpy(&out_data[out_pos],end,endLength);
 out_pos+=endLength;
 }
if(last_bytes>0)
 {
 memcpy(&out_data[out_pos],&data[in_pos],last_bytes);
 out_pos+=last_bytes;
 memcpy(&out_data[out_pos],end,endLength);
 out_pos+=endLength;
 }
out_data_Length=out_pos;
return true;
}


#pragma warning( push )
#pragma warning(disable : 4996)

// 0-    1 data - str   2 data - file(filepath))
template <typename StringType> bool iMakeMailHeader(iString<char> &header,  const StringType* name_from,const char* email_from,const StringType* name_to,const char*email_to,const StringType* subject,iString<StringType>& message_txt,iString<StringType>& message_html,const char* data,unsigned long dataLength,const StringType* filename,unsigned long mode, const char* CharSet="utf-8")
{

//#ifdef _WINDOWS_
//message_txt.SwapEndian();
//message_html.SwapEndian();
//#endif


struct tm   newTime;    
time_t      szClock;
time( &szClock );                  
gmtime_s(&newTime ,&szClock );  //localtime_r 

iString<char> str_time;str_time.Allocate(64);


uint32_t dwClock= (uint32_t)szClock;
iString<char>   boundary;
boundary.SetLength(34);
boundary="--"; 
int_to_hexstring_p(dwClock,(char*)str_time.Buffer());
boundary+=str_time.Buffer();
boundary+=str_time.Buffer();
boundary+=str_time.Buffer();
boundary+=str_time.Buffer();

str_time.SetLength(0,false);
strftime((char*)str_time.Buffer(),str_time.GetAllocated(),"%a, %d %b %Y %H:%M:%S", &newTime ); // $.date("D, j M Y G:i:s")." +0700\r\n";
str_time.ResetLength();
str_time+=" +0000 (UTC)";




header.SetLength(0,false);
header.Allocate(4096,false);
iString<StringType> src; src.Allocate(128);
iString<char> dest;      dest.Allocate(512);


/*
header+="Subject: ";
dest.AppendAsChar(subject); ///////////////////////////////////////////
header+=dest;
header+= iEOF;
*/

header+="Date: "; header+= str_time; header+= iEOF;


header+="From: ";
src=name_from;
if(!mime_header_encode<StringType>(src,dest,CharSet,false)){return false;}
header+=dest;
header+=" <";
header+=email_from;
header+=">";
header+= iEOF;


#ifndef IPOP_XMAILER
header+="X-Mailer: The Bat! (v3.99.3) Professional";
#endif // !IPOP_XMAILER
#ifdef IPOP_XMAILER
header+=IPOP_XMAILER;
#endif // IPOP_XMAILER




header+= iEOF;

header+= "Reply-To: ";
header+=dest;//$name_from
header+=" <";
header+=email_from;
header+=">";
header+= iEOF;

header+="X-Priority: 3 (Normal)";
header+= iEOF;

header+="Message-ID: <1";//.
uint32_t dwticks=GetTickCount();
str_time.Allocate(128,false);
str_time.ClearMem();
header+=iToString(dwticks,(char*)str_time.Buffer(),str_time.GetAllocated());//GetLength()
str_time.ClearMem();
header+=iToString(dwClock,(char*)str_time.Buffer(),str_time.GetAllocated());//GetLength()
dwticks=dwticks*dwClock*3;
str_time.ClearMem();
header+=iToString(dwticks,(char*)str_time.Buffer(),str_time.GetAllocated());//GetLength()
header+="@";
header+=i_domain_from_email(email_from);
header+=">";
header+= iEOF;


header+="To: "; 
src=name_to;
if(!mime_header_encode<StringType>(src,dest,CharSet,false)){return false;}
header+=dest;
header+=" <";
header+=email_to;
header+=">";
header+= iEOF;


header+="Subject: "; 
src=subject;
if(!mime_header_encode<StringType>(src,dest,CharSet,false)){return false;}   
header+=dest;
header+= iEOF;


header+="MIME-Version: 1.0";
header+= iEOF;




if((message_txt.GetLength()!=0)&&(message_html.GetLength()!=0)) 
 {
 header+="Content-Type: multipart/alternative; boundary=\"";
 header+=boundary;
 header+="\"";
 header+= iEOF; 
 header+= iEOF;
 } else {
 header+="Content-Type: multipart/mixed; boundary=\"";
 header+=boundary;
 header+="\"";
 header+= iEOF; 
 header+= iEOF;  
 }




header+= "--";
header+=boundary;
header+= iEOF;

header+= "Content-Type: text/plain; charset=\"";
header+= CharSet;
header+= "\"";
header+= iEOF;
header+= "Content-Transfer-Encoding: 8bit";
header+= iEOF;
header+=iEOF;
if(CharSet=="utf-8"){header.AppendAsUTF8((StringType*)message_txt.Buffer());}else{header.AppendAsChar((StringType*)message_txt.Buffer());}




if(message_html.GetLength()!=0)
 {
 header+=iEOF;
 header+=iEOF;
 header+= "--";
 header+=boundary;
 header+=iEOF;
 header+="Content-Type: text/html; charset=\"";
 header+= CharSet;
 header+="\"";
 header+=iEOF;

 header+="Content-Transfer-Encoding: 8bit";
 header+=iEOF;
 header+=iEOF;
 if(CharSet=="utf-8"){header.AppendAsUTF8((StringType*)message_html.Buffer());}else{header.AppendAsChar((StringType*)message_html.Buffer());}
 }



src=ExtractFileName(filename);
if(!mime_header_encode<StringType>(src,dest,CharSet,false)){return false;}   

if (mode==1)
 {	
 header+=iEOF;
 header+=iEOF;
 header+= "--";
 header+=boundary;
 header+=iEOF;

 header+="Content-Type: application/octet-stream; name=";
 header+="\"";
 header+=dest;//ExtractFileName(filename);
 header+="\"";
 header+=iEOF;

 header+="Content-Disposition: attachment; filename=";
 header+="\"";
 header+=dest;//ExtractFileName(filename);
 header+="\"";
 header+=iEOF; //; size='.strlen($data).';

 header+="Content-Transfer-Encoding: base64";
 header+=iEOF; 

 header+=iEOF;


 int dataBufferLength_Base64=iBase64EncodeGetRequiredLength(dataLength,iBASE64_FLAG_NONE)+128;
 char * buf_Base64=(char *)malloc(dataBufferLength_Base64);
 if(buf_Base64==NULL){return false;}
 if(!iBase64Encode((BYTE*)data,dataLength,buf_Base64,&dataBufferLength_Base64,	 iBASE64_FLAG_NONE)){free(buf_Base64);return false;}

 unsigned long dataBufferLength_chunk_split=0;
 chunk_split(NULL,dataBufferLength_Base64,NULL,dataBufferLength_chunk_split,76U,"\r\n");
 header.Allocate(header.GetLength()+dataBufferLength_chunk_split+64);
 if(!chunk_split(buf_Base64,dataBufferLength_Base64,&header[header.GetLength()],dataBufferLength_chunk_split)){free(buf_Base64);return false;}
 free(buf_Base64);
 header.SetLength(header.GetLength()+dataBufferLength_chunk_split);
 }
if (mode==2)
 {	
 iString<char> s;
 s.AppendAsChar(filename);
 FILE *f = fopen(s.Buffer(),"rb");	//rb,"r");   windows binary mode
 if(f==NULL){return false;}
  

 header+=iEOF;
 header+=iEOF;
 header+= "--";
 header+=boundary;
 header+=iEOF;

 header+="Content-Type: application/octet-stream; name=";
 header+="\"";
 header+=dest;//ExtractFileName(filename);
 header+="\"";
 header+=iEOF;

 fseek(f, 0L, SEEK_END);
 long f_size= ftell(f);
 header+="Content-Disposition: attachment; filename=";
 header+="\"";
 header+=dest;//ExtractFileName(filename);
 header+="\"";
 header+="; size=";
 str_time.ClearMem();
 header+=iToString(f_size,(char*)str_time.Buffer(),str_time.GetAllocated());//GetLength
 header+=";";
 header+=iEOF; 

 header+="Content-Transfer-Encoding: base64";
 header+=iEOF; 

 header+=iEOF;

 


 
 


 int dataBufferLength_Base64=iBase64EncodeGetRequiredLength(f_size,iBASE64_FLAG_NONE)+128;
 char * buf_file=(char *)malloc(f_size);
 if(buf_file==NULL){return false;}
 fseek(f,0,SEEK_SET);
 size_t readresult=fread(buf_file,1,f_size,f);
 fclose(f);
 if(readresult!=f_size){free(buf_file);return false;}
 header.Allocate(header.GetLength()+dataBufferLength_Base64,false);
 if(!iBase64Encode((BYTE*)buf_file,f_size,(char*)&header.Buffer()[header.GetLength()],&dataBufferLength_Base64,	 iBASE64_FLAG_NONE)){free(buf_file);return false;}
 header.SetLength(header.GetLength()+dataBufferLength_Base64);
 free(buf_file);

 /*
 if(!iBase64Encode((BYTE*)buf_file,f_size,buf_Base64,&dataBufferLength_Base64,	 iBASE64_FLAG_NONE)){free(buf_Base64);free(buf_file);return false;}
 free(buf_file);

 unsigned long dataBufferLength_chunk_split=0;
 chunk_split(NULL,dataBufferLength_Base64,NULL,dataBufferLength_chunk_split,76U,"\r\n");
 header.Allocate(header.GetLength()+dataBufferLength_chunk_split+64,false);
 unsigned int uuu=header.GetLength();
 char*cccc=(char*)&header.Buffer()[uuu] ;
 if(!chunk_split(buf_Base64,dataBufferLength_Base64,cccc,dataBufferLength_chunk_split)){free(buf_Base64);return false;}
 free(buf_Base64);
 header.SetLength(header.GetLength()+dataBufferLength_chunk_split);
 */
 }
 
header+=iEOF; 
header+=iEOF; 
header+="--";
header+=boundary;
header+="--";

header+=iEOF; 
header+=".";
header+=iEOF; 
return true;
}

#pragma warning( pop ) 


#define SMTP_GET_CODE_BUFFER_SIZE  512U

unsigned int smtp_get_responce_code(SOCKET &so,char* buffer )
{
int BufferReceived;
//int ReceivedTotal=0;
unsigned int recv_call_count=0;

// do
// {
 BufferReceived = recv(so, &buffer[0],SMTP_GET_CODE_BUFFER_SIZE , 0); 
if(BufferReceived<0)BufferReceived=0;
// recv_call_count++; 
// if(recv_call_count==16){return IC_TO_MANY_RECV_CALL;}
 if(BufferReceived==0){buffer[0]=0;return 0;}
 if (SOCKET_ERROR==BufferReceived){return IC_ERROR_SOCKET; }
// ReceivedTotal+=BufferReceived;
// if (SMTP_GET_CODE_BUFFER_SIZE<=ReceivedTotal){return IC_BUFFER_TO_SMALL;}
// } while (BufferReceived!=0);
if(BufferReceived<3){return IC_SMTP_NO_MESSAGE;}

BufferReceived++;
if(BufferReceived>SMTP_GET_CODE_BUFFER_SIZE)BufferReceived=SMTP_GET_CODE_BUFFER_SIZE;
buffer[BufferReceived-1]=0;

return iStrAsUInt<char>(buffer,2);
}


unsigned int smtp_put_string(SOCKET &so,const char* cstr,bool add_eof=true)
{
iString<char> str;
str=cstr;
if(add_eof) { str+=iEOF;}
if(SOCKET_ERROR==send(so,str.Buffer(),str.GetLength(),0))
 {
 return  WSAGetLastError();
 }
return 0;
}



unsigned int  MailSmtp_a(iDNSResolver& DNSResolver,const char* email_to, iString<char> & header, iString<char> &SmtpMail_Server,unsigned short SmtpMail_Port, const char* SmtpMail_Username,const  char* SmtpMail_Password,iString<char>*log=NULL,iString<char>*error_str=NULL)
{

if(error_str!=NULL) error_str->SetLength(0); 

char buf16[16];
unsigned int ui;
SOCKET so;
ui=iConnect(DNSResolver,iDNS_T_MX,SmtpMail_Server,SmtpMail_Port,so);  
if(ui!=0){return ui;} 



char* buffer=(char*)malloc(SMTP_GET_CODE_BUFFER_SIZE);
if(buffer==NULL){closesocket(so);return IC_OUT_OF_MEMORY;}

ui=smtp_get_responce_code(so,buffer);
if((ui!=220)&&(ui!=250)&&(ui!=334)&&(ui!=200)){if(error_str!=NULL) *error_str+=buffer;   closesocket(so);free(buffer);return ui;}//(ui!=0)&&


if(log!=NULL){*log+="Connected: "; *log+=iToString<char>(ui,&buf16[0],sizeof(buf16));*log+=iEOF;}

ui=smtp_put_string(so,"EHLO lhost_tn"); if(ui!=0){closesocket(so);free(buffer);return ui;}


ui=smtp_get_responce_code(so,buffer);
if(log!=NULL){*log+="EHLO response: "; *log+=iToString<char>(ui,&buf16[0],sizeof(buf16));*log+=iEOF;}
if((ui!=250)&&(ui!=334)&&(ui!=200))
 {
 if(ui==550)
 {
 ui=smtp_put_string(so,"HELO lhost_tn"); if(ui!=0){closesocket(so);free(buffer);return ui;}
 ui=smtp_get_responce_code(so,buffer);
 if(log!=NULL){*log+="HELO response: "; *log+=iToString<char>(ui,&buf16[0],sizeof(buf16));*log+=iEOF;}
 if((ui!=250)&&(ui!=334)&&(ui!=200)){if(error_str!=NULL) *error_str+=buffer;   closesocket(so);free(buffer);return ui;}
 }else{
	if(error_str!=NULL) *error_str+=buffer;   closesocket(so);free(buffer);return ui;
 }
}


ui=smtp_put_string(so,"AUTH LOGIN"); if(ui!=0){if(error_str!=NULL) *error_str+=buffer;  closesocket(so);free(buffer);return ui;}
ui=smtp_get_responce_code(so,buffer);
if(log!=NULL){*log+="authrequest: "; *log+=iToString<char>(ui,&buf16[0],sizeof(buf16));*log+=iEOF;}
if((ui==334)||(ui==250)&&(ui!=200))
 {
 int DestLen=SMTP_GET_CODE_BUFFER_SIZE-1;
 if(!iBase64Encode((BYTE*)SmtpMail_Username,iStrLen<char>(SmtpMail_Username), buffer,&DestLen,iBASE64_FLAG_NOPAD|iBASE64_FLAG_NOCRLF)){closesocket(so);free(buffer);return IC_ERROR;}
 buffer[DestLen]=0;
 ui=smtp_put_string(so,buffer); if(ui!=0){closesocket(so);free(buffer);return ui;} 
 ui=smtp_get_responce_code(so,buffer);
 if(log!=NULL){*log+="authmhSmtpMail_username: "; *log+=iToString<char>(ui,&buf16[0],sizeof(buf16));*log+=iEOF;}
 if((ui==334)||(ui==200))  //mail.com=0 (accound as removed)
  {
  DestLen=SMTP_GET_CODE_BUFFER_SIZE-1;
  if(!iBase64Encode((BYTE*)SmtpMail_Password,iStrLen<char>(SmtpMail_Password), buffer,&DestLen,iBASE64_FLAG_NOPAD|iBASE64_FLAG_NOCRLF)){closesocket(so);free(buffer);return IC_ERROR;}
  buffer[DestLen]=0;
  ui=smtp_put_string(so,buffer); if(ui!=0){closesocket(so);free(buffer);return ui;} 
  ui=smtp_get_responce_code(so,buffer);
  if(log!=NULL){*log+="authmhSmtpMail_password: "; *log+=iToString<char>(ui,&buf16[0],sizeof(buf16));*log+=iEOF;}
  if((ui!=334)&&(ui!=235)&&(ui!=200)){if(error_str!=NULL) *error_str+=buffer; closesocket(so);free(buffer);return IC_ERROR_AUTHENTICATION;} 
  }
 }

iString<char> s;
s="MAIL FROM: ";
s+="<";
s+=SmtpMail_Username;
s+=">";
ui=smtp_put_string(so,s.Buffer()); if(ui!=0){closesocket(so);free(buffer);return ui;}
ui=smtp_get_responce_code(so,buffer);
if(log!=NULL){*log+="mailmhSmtpMail_fromresponse: "; *log+=iToString<char>(ui,&buf16[0],sizeof(buf16));*log+=iEOF;}
if((ui!=250)&&(ui!=334)&&(ui!=200)){if(error_str!=NULL) *error_str+=buffer;  closesocket(so);free(buffer);return ui;}

s="RCPT TO: ";
s+="<";
s+=email_to;
s+=">";
//s+=iEOF;
ui=smtp_put_string(so,s.Buffer()); if(ui!=0){closesocket(so);free(buffer);return ui;}
ui=smtp_get_responce_code(so,buffer);
if(log!=NULL){*log+="mailtoresponse: "; *log+=iToString<char>(ui,&buf16[0],sizeof(buf16));*log+=iEOF;}
if((ui!=250)&&(ui!=251)&&(ui!=334)&&(ui!=200)){if(error_str!=NULL) *error_str+=buffer;  closesocket(so);free(buffer);return ui;}


ui=smtp_put_string(so,"DATA"); if(ui!=0){closesocket(so);free(buffer);return ui;}
ui=smtp_get_responce_code(so,buffer);
if(log!=NULL){*log+="data1response: "; *log+=iToString<char>(ui,&buf16[0],sizeof(buf16));*log+=iEOF;}
if((ui!=354)&&(ui!=334)&&(ui!=200)){if(error_str!=NULL) *error_str+=buffer;  closesocket(so);free(buffer);return ui;}

if(SOCKET_ERROR==send(so,header.Buffer(),header.GetLength(),0)){closesocket(so);free(buffer);return IC_ERROR_SEND;} 
ui=smtp_get_responce_code(so,buffer);
if(log!=NULL){*log+="c: "; *log+=iToString<char>(ui,&buf16[0],sizeof(buf16));*log+=iEOF;}
if((ui!=250)&&(ui!=334)&&(ui!=200)){if(error_str!=NULL) *error_str+=buffer;closesocket(so);free(buffer);return ui;}

ui=smtp_put_string(so,"QUIT"); if((ui!=0)&&(ui!=221)&&(ui!=200))  {closesocket(so);free(buffer);return ui;}
ui=smtp_get_responce_code(so,buffer);
if(log!=NULL){*log+="quitresponse: "; *log+=iToString<char>(ui,&buf16[0],sizeof(buf16));*log+=iEOF;}

closesocket(so);
free(buffer);
return 0;
}
//SEND 


// 0-    1 data - str   2 data - file(filepath))
template <typename StringType> unsigned int doMailSmtp(iDNSResolver& DNSResolver,const StringType* name_from,const char* email_from,const StringType* name_to,const char* email_to,const StringType* subject,iString<StringType>& message_txt,iString<StringType>& message_html,const char* data,unsigned long dataLength,const StringType* filename,unsigned long mode,iString<char> &SmtpMail_Server,unsigned short SmtpMail_Port, const char* SmtpMail_Username, const char* SmtpMail_Password, const char* DestCharSet="utf-8",iString<char>*log=NULL,iString<char>*error_str=NULL) 
{
iString<char> header;

if(!iMakeMailHeader<StringType>(header, name_from, email_from, name_to,email_to, subject, message_txt, message_html, data, dataLength, filename, mode, DestCharSet)){return IC_ERROR_MAKE_SMTP_HEADER;}



return  MailSmtp_a(DNSResolver,email_to, header, SmtpMail_Server,SmtpMail_Port, SmtpMail_Username, SmtpMail_Password,log,error_str);



}



#endif  //_IPOP_H
















