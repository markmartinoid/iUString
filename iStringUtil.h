#pragma once
#ifndef _iStringUtil_H
#define _iStringUtil_H

#include <math.h>

#include <iDynArray.h>

#ifdef _WINDOWS_
#define iPATH_SEPARATOR '\\'

#elif
#define iPATH_SEPARATOR '/'

#endif




/*
string to_utf8(const wchar_t* buffer, int len)
{
	int nChars = ::WideCharToMultiByte(
		CP_UTF8,
		0,
		buffer,
		len,
		NULL,
		0,
		NULL,
		NULL);
	if (nChars == 0) return "";

	string newbuffer;
	newbuffer.resize(nChars) ;
	::WideCharToMultiByte(
		CP_UTF8,
		0,
		buffer,
		len,
		const_cast< char* >(newbuffer.c_str()),
		nChars,
		NULL,
		NULL); 

	return newbuffer;
}
 */

// split string
template <typename StringType> void iStrToDynArrayPtr(StringType*s,const StringType Separator,   iDynArray<StringType*> &a)
{
unsigned int length=iStrLen<StringType>(s);
a.SetCount(0); 
if(length==0)return;
if((s[0]!=Separator))a.Add(s);
length--;
for(unsigned int i=0;i<length;i++)
 {
 if(s[i]==Separator)
  {
  s[i]=0; 
  if(s[i+1]!=Separator)a.Add(&(s[i+1]));
  }
 }
}



template <typename StringType> StringType* ExtractFileName(StringType*filename)
{
unsigned int length=iStrLen<StringType>(filename);
unsigned int Last=0xffffffff;
for(unsigned int i=0;i<length;i++)
 {
 if((filename[i]==0x2f)||(filename[i]==0x5c)){Last=i;}
 }
Last++;
if(Last<length){return &filename[Last];}
return 0;
}


//bufferCount including null character
template <typename StringType> StringType* iToString(const DWORD i, StringType*buffer,const DWORD  bufferCount)
{
if(bufferCount<2){return NULL;}
memset(buffer,0,bufferCount*sizeof(StringType));
if(i==0){buffer[0]=48;return buffer;}
DWORD x=i;
DWORD Chars=1;//last null
DWORD modulo;
while(x!=0)
{
 modulo=x-(DWORD)((x/10)*10);
 x=x/10;
 
 Chars++;
 if(Chars>bufferCount){return buffer;}
 buffer[bufferCount-Chars]=(StringType)(48+modulo);
 }
return &buffer[bufferCount-Chars];
}

//bufferCount including null character
template <typename StringType,typename IntType> StringType* iToStringScale(const IntType i, StringType*buffer,const DWORD  bufferCount,const IntType Scale,const StringType*charset)
{
if(bufferCount<2){return NULL;}
memset(buffer,0,bufferCount*sizeof(StringType));
if(i==0){buffer[0]=charset[0];   return buffer;}
IntType x=i;
DWORD Chars=1;//last null
IntType modulo;
while(x!=0)
{
 modulo=x-(IntType)((x/Scale)*Scale);
 if(modulo>=Scale)return NULL;
 x=x/Scale;
 
 Chars++;
 if(Chars>bufferCount){return NULL;}
 buffer[bufferCount-Chars]=charset[modulo];
 }
return &buffer[bufferCount-Chars];
}



/*
template <typename StringType> int StringToInt(const StringType *buffer, StringType*buffer,const DWORD  bufferCount)
{
if(buffer==NULL)return 0;
bool negative;
if (buffer[0]==45){negative=true;}else{negative=false;}

for(i:=0;i<12;i++)
 {
 if(negative) if(i==0) continue;
 if((buffer[i]<


 }


}
*/



           
void int_to_hexstring(DWORD value, char result[9])
{
static char const HEXDIGITS[0x10] = {'0', '1', '2', '3', '4', '5', '6', '7','8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int i;
    result[8] = '\0';

    for(i=7; i>=0; i-- ) {

        int d  = value & 0xf;
        result[i] = HEXDIGITS[d];
        value >>= 4;
    }
    for(;i>=0;i--){ result[i] = '0'; }
}


 
void int_to_hexstring_p(DWORD value, char* result)
{
static char const HEXDIGITS[0x10] = {'0', '1', '2', '3', '4', '5', '6', '7','8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int i;
    result[8] = '\0';

    for(i=7; i>=0; i-- ) {

        int d  = value & 0xf;
        result[i] = HEXDIGITS[d];
        value >>= 4;
    }
    for(;i>=0;i--){ result[i] = '0'; }
}




void uint64_to_hexstring(ULONGLONG value, char result[17])
{
static char const HEXDIGITS[0x10] = {'0', '1', '2', '3', '4', '5', '6', '7','8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int i;
    result[16] = '\0';

    for(i=15; i>=0; i-- ) {

        int d  = value & 0xf;
        result[i] = HEXDIGITS[d];
        value >>= 4;
    }
    for(;i>=0;i--){ result[i] = '0'; }
}






int hexalpha_to_int(const unsigned char c)
{

if((c >= '0') && (c <= '9')) {return (c- '0');}
   
unsigned char hexalpha[] = "aAbBcCdDeEfF\0";
int i;
unsigned char  answer = 0;

  for(i = 0; answer == 0 && hexalpha[i] != '\0'; i++)
  {
    if(hexalpha[i] == c)
    {
      answer = 10 + (i / 2);
    }
  }
  return answer;
}

int hexalpha_to_intW(const wchar_t c)
{

if((c >= L'0') && (c <= L'9')) {return (c- L'0');}
   
wchar_t hexalpha[] = L"aAbBcCdDeEfF\0";
int i;
unsigned char  answer = 0;

  for(i = 0; answer == 0 && hexalpha[i] != L'\0'; i++)
  {
    if(hexalpha[i] == c)
    {
      answer = 10 + (i / 2);
    }
  }
  return answer;
}



template <typename StringType> DWORD hexstring_to_int(const  StringType* str, DWORD length)
{
DWORD  result=0;
DWORD power=0;
for(int i=length-1;i>=0;i--)
{
result|=hexalpha_to_int(str[i])<<power;
power+=4;
}
return  result;
}


DWORD hexstring_to_intW(const  wchar_t* str,DWORD length )
{
DWORD  result=0;
DWORD power=0;
for(int i=length-1;i>=0;i--)
{
result|=hexalpha_to_intW(str[i])<<power;
power+=4;
}
return  result;
}



ULONGLONG hexalpha_to_uint64(const unsigned char c)
{

if((c >= '0') && (c <= '9')) {return (c- '0');}
   
unsigned char hexalpha[] = "aAbBcCdDeEfF\0";
int i;
unsigned char  answer = 0;

  for(i = 0; answer == 0 && hexalpha[i] != '\0'; i++)
  {
    if(hexalpha[i] == c)
    {
      answer = 10 + (i / 2);
    }
  }
  return answer;
}




ULONGLONG hexstring_to_uint64(const char* str,DWORD length )
{
ULONGLONG  result=0;
ULONGLONG power=0;
for(int i=length-1;i>=0;i--)
{
result|=hexalpha_to_uint64(str[i])<<power;
power+=4;
}
return  result;
}





#endif//  _iStringUtil_H

