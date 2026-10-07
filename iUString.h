// Mark Martin 2007-2025 all rights reserved markmartinoid@gmail.com 

#pragma once
#ifndef _iString_H
#define _iString_H

#include <cstdlib>   // для malloc/realloc/free
#include <cstring>   // для memcpy/memmove/memset
#include <cwctype>   // для std::towupper и std::towlower (кроссплатформенный регистр)
#include <codecvt>   // для кроссплатформенной конвертации UTF-8 / UTF-16
#include <locale>    // для wstring_convert

// Заменяем специфичный для Windows ULONGLONG на стандартный тип C++
typedef unsigned long long i_uint64; 

#ifndef _StringType_DEFINED
#ifdef _WINDOWS_
    typedef wchar_t iwchar_t;
#else
    // Для совместимости, если Windows не определен
    typedef wchar_t iwchar_t; 
    // Вместо Windows-типов UINT используем стандартные
    typedef unsigned int UINT;
    #define CP_ACP 0
    #define CP_UTF8 65001
#endif

typedef unsigned short StringType;
#define _StringType_DEFINED
#endif



#ifdef _iSTR_USE_iMemMan 
#include "iMemMan.h"
#include "iDynArray.h"
#endif




#define iString_MAX_STRING_LENGTH  0x00400000


//string(string&& that)   // string&& is an rvalue reference to a string    This is so called RVO.
//http://msdn.microsoft.com/en-us/library/ms364057.aspx


//http://msdn.microsoft.com/en-us/library/dd374081.aspx
//little-endian UTF-16.
// wchar_t characters in memory are little-endian
template <typename StringType>void SwapEndianString(StringType *x,unsigned int Count)
{
if(sizeof(StringType)==2)
 {
 char c1;
 char c2;
 char* p_c1;
 char* p_c2;
 for(unsigned int i=0;i<Count;i++)
  {
  p_c1=(char*)&x[i];
  p_c2=p_c1+1;
  c1=*p_c1;
  c2=*p_c2;
  *p_c1=c2;
  *p_c2=c1;
  }
 }
}




template <typename StringType>bool iCharIsDigitOrDot(const StringType& chr)
{
if((chr<58)&&(chr>47)){return true;}
if((chr==44)||(chr==45)||(chr==46)){return true;}
return false;
}

template <typename StringType>bool iCharIsDigit(const StringType& chr)
{
if((chr<58)&&(chr>47)){return true;}

return false;
}






template <typename StringType>int iStrValue(const StringType* str,unsigned int str_max_index,const StringType* value_name,unsigned int value_name_LENGTH) 
{
unsigned int i_str=0;
int i_strSearch=0;
for(unsigned int i=0;i<=str_max_index;i++)
 {
 if(i_strSearch==value_name_LENGTH){break;}
 if(str[i_str]==value_name[i_strSearch]){i_strSearch++;}else{i_strSearch=0;}
 }
if(i_str>str_max_index){return 0;}
if(i_strSearch==value_name_LENGTH) {return 0;}
while((str[i_str]==' ')||(str[i_str]=='=')){i_str++;if(i_str>str_max_index){return 0;}}
i_strSearch=0; //now it is value max_index 
for(unsigned int i=i_str;i<=str_max_index;i++)
 {
 if((str[i]<58)&&(str[i]>47)){i_strSearch=i;}else{break;}
 }
int retval=0;
int power=1;
for(unsigned int i=i_strSearch;i<=i_str;i++)
 {
 retval+=power*(str[i]-48);
 power*=10;
 }
return retval;
}
//.46  -45   ,44    /47    0 48     9 57

template <typename StringType>unsigned int iStrAsUInt(const StringType* str,unsigned int str_max_index)
{
unsigned int retval=0;
unsigned int power=1;
unsigned int i=str_max_index;
do
{
 if(!(((str[i])<58)&&((str[i])>47))){return retval;}
 retval+=power*(str[i]-48);
 if(i==0){return retval;}
 power*=10;
 i--;
} while (true);
return retval;
}


template <typename StringType>ULONGLONG iStrAsUInt64(const StringType* str,unsigned int str_max_index)
{
ULONGLONG retval=0;
ULONGLONG power=1;
unsigned int i=str_max_index;
do
{
 if(!(((str[i])<58)&&((str[i])>47))){return retval;}
 retval+=power*(str[i]-48);
 if(i==0){return retval;}
 power*=10;
 i--;
} while (true);
return retval;
}






template <typename StringType>  unsigned int iStrLen(const StringType* str) 
{
unsigned int length=0;
if(str==NULL){return 0;}
for(unsigned int i=0;i<iString_MAX_STRING_LENGTH;i++)
 {
  if(str[i]==0){return (i);}
 }
return 0;//error  iString_MAX_STRING_LENGTH
}


template <typename StringType>bool iStrEqual_s(const StringType  *str,const StringType *strSearch,unsigned int compare_length)   
{
if((str==NULL)&&(strSearch==NULL)){return true;}
if((str==NULL)||(strSearch==NULL)){return false;}
for(unsigned int i=0;i<compare_length;i++)
 {
 if((str[i]==NULL)&&(strSearch[i]==NULL)){return true;}
 if((str[i]==NULL)||(strSearch[i]==NULL)){return false;}
 if(str[i]!=strSearch[i]){return false;}
 }
return true;
}


template <typename StringType>int iStrPos(const StringType  *str,const StringType *strSearch)   
{
int i_str=0;
int i_strSearch=0;
do
 {
 if(strSearch[i_strSearch]==0){return (i_str-i_strSearch);}
 if(str[i_str]==0){return -1;}
 if(str[i_str]==strSearch[i_strSearch]){i_strSearch++;}else{i_strSearch=0;}
 i_str++;
 } while (true);
return -1;
}

template <typename StringType>int iStrPos(const StringType  *str,const StringType *strSearch, int StartPos)   
{
if(StartPos<0){return -1;}
int i_str=StartPos;
int i_strSearch=0;
do
 {
 if(strSearch[i_strSearch]==0){return (i_str-i_strSearch);}
 if(str[i_str]==0){return -1;}
 if(str[i_str]==strSearch[i_strSearch]){i_strSearch++;}else{i_strSearch=0;}
 i_str++;
 } while (true);
return -1;
}



template <typename StringType>int iStrSearchCount(const StringType  *str,const StringType *strSearch)   
{
if(strSearch[0]==0)return 0;
int i_str=0;
int i_strSearch=0;
int Count=0;
do
 {
 if(strSearch[i_strSearch]==0)
  {i_strSearch=0;Count++;}
 if(str[i_str]==0)
  {return Count;}
 if(str[i_str]==strSearch[i_strSearch])
  {i_strSearch++;}
 else
  {i_strSearch=0;}
 i_str++;
 } while (true);
return Count;
}


template <typename StringType>int iStrPosLast(const StringType  *str,const StringType *strSearch,unsigned int strEndIndex,unsigned int strSearchLength)   
{
unsigned int i_str=strEndIndex;
int i_strSearch=strSearchLength-1;
do
 {
 if(str[i_str]==strSearch[i_strSearch]){i_strSearch--;}else{i_strSearch=strSearchLength-1;}
 if(i_strSearch==-1){return (i_str);}
 if(i_str==0){return -1;}
 i_str--;
 } while (true);
return -1;
}





template <class StringType> class  iString
{
StringType* data;
unsigned int Allocated;
unsigned int Length;
unsigned int var_uint;
public:
//bool CanIncreaseMemory;
enum exception { MEMFAIL,OUTOFBOUND,TOBIGSIE }; 

 iString(unsigned int length=0)
 { 
  data=NULL;Length=0;Allocated=0;
  if(length!=0){Allocate(length+1);}else{data=NULL;Length=0;Allocated=0;}
 }

 iString(const iString<StringType> &str)
{ 
data=NULL;Length=0;Allocated=0;
if(&str== NULL){return;};
SetLength(str.Length);
memcpy(data,str.data,Length*sizeof(StringType));
}

//  move constructor
 iString(iString<StringType>&& str)
{ 
if(&str== NULL){data=NULL;Length=0;Allocated=0;return;};
data=str.data;Length=str.Length;Allocated=str.Allocated;
str.data=NULL;str.Length=0;str.Allocated=0;
}


 iString(const iString<StringType> *str)
{ 
data=NULL;Length=0;Allocated=0;
if(&str== NULL){return;};
SetLength(str->Length);
memcpy(data,str->data,Length*sizeof(StringType));
}

 iString(const StringType &str)
{   
data=NULL;Length=0;Allocated=0;
if(&str== NULL){return;};
SetLength(iStrLen<StringType>(&str));
memcpy(data,&str,Length*sizeof(StringType));
}


 iString(const StringType str[])  
{     
data=NULL;Length=0;Allocated=0;
if(&str== NULL){return;};
SetLength(iStrLen<StringType>(&str[0]));               
memcpy(data,&str[0],Length*sizeof(StringType));  ///!!  &str[0]
}



 ~iString(void)
 {   Allocate(0,true);
 }


void SetEmpty()
{
if(Allocated!=0) 
 {
 data[0]=0;
 }
}


bool GetEmpty()
{
if(Allocated!=0) 
 {
 return data[0]==0;
 }else{
 return true;
 }
}


void SetLength(const unsigned int NewLength,const bool CanFree=true)
{
if(NewLength>iString_MAX_STRING_LENGTH){throw TOBIGSIE;}
Allocate(NewLength+1,CanFree);
Length=NewLength;
data[Length]=0;
}

//total including null character
void Allocate(const unsigned int NewAllocated,const bool CanFree=true) 
{
if(Allocated == NewAllocated){return;}
if(!CanFree){if(NewAllocated<Allocated){return;}}
if(NewAllocated==0)
 {
  if((data!=NULL)&&(Allocated!=0)){free(data);data=NULL;}
 }else{
 StringType* new_ptr = (StringType*)realloc(data, sizeof(StringType)*NewAllocated); 
 if (new_ptr == NULL) 
  {Allocated=0;Length=0; throw MEMFAIL;return; }
 data=new_ptr;  
 if(Allocated==0){data[0]=0;}
 }
Allocated = NewAllocated;
}

const StringType* Buffer()
{
return data;
}

//  unsigned int minLength = min(buffer1->length,buffer2->length);
int Compare(const StringType* buffer2,const unsigned int minLength) const
{
if(data == buffer2)
 {
 return 0;
 }
if(data && buffer2)
 {
 int res;
 for(unsigned int i = 0; i < minLength; i++)
  {
  res = int(data[i]) - int(buffer2[i]);
  if(res != 0)
   {return res;}
  }
 }else{ 
 if(data){return 1;}
 if(buffer2){return -1;}               
 }                   
return 0;                
}

int Compare(const iString<StringType> & str2) const
{
int x=Length-str2.Length;
if(x!=0){return x;}
return Compare(str2.data,Length);
}


int Compare(const StringType* buffer2 ) const
{
int x=Length-iStrLen<StringType>(buffer2);
if(x!=0){return x;}
return Compare(buffer2,Length);
}


bool operator == (const iString<StringType>& other) const { return Compare(other) == 0; }
bool operator != (const iString<StringType>& other) const { return Compare(other) != 0; }
bool operator <= (const iString<StringType>& other) const { return Compare(other) <= 0; }
bool operator <  (const iString<StringType>& other) const { return Compare(other) <  0; }
bool operator >= (const iString<StringType>& other) const { return Compare(other) >= 0; }
bool operator >  (const iString<StringType>& other) const { return Compare(other) >  0; }

bool operator == (const StringType* str) const { return Compare(str) == 0; }
bool operator != (const StringType* str) const { return Compare(str) != 0; }
bool operator <= (const StringType* str) const { return Compare(str) <= 0; }
bool operator <  (const StringType* str) const { return Compare(str) <  0; }
bool operator >= (const StringType* str) const { return Compare(str) >= 0; }
bool operator >  (const StringType* str) const { return Compare(str) >  0; }

//move assignment operator
iString<StringType>& operator=(iString&& _Right) 
{	
if (this != &_Right)
 {
 data=_Right.data;Length=_Right.Length;Allocated=_Right.Allocated;
 _Right.data=NULL;_Right.Length=0;_Right.Allocated=0;
 }
return (*this);
}

iString<StringType>& operator=(const iString& _Right) 
{	
if (this != &_Right)
 {
 if(&_Right== NULL){SetLength(0,false);return(*this);};
 SetLength(_Right.Length,false);
 memcpy(data,_Right.data,Length*sizeof(StringType));
 }
return (*this);
}

iString<StringType>& operator=(const StringType& _Right) 
{	
if(&_Right== NULL){SetLength(0,false);return(*this);};
SetLength(iStrLen<StringType>(&_Right),false);
memcpy(data,&_Right,Length*sizeof(StringType));
return (*this);
}


iString<StringType>& operator=(const StringType* _Right) //new
{	
if(_Right== NULL){SetLength(0,false);return(*this);};
SetLength(iStrLen<StringType>(_Right),false);
memcpy(data,_Right,Length*sizeof(StringType));
return (*this);
}





iString<StringType>& Append(const StringType &_Right,const unsigned int  _length)
{
if((_Right!=NULL)&&(_length!=0))
 {
 var_uint=Length;
 SetLength(Length+_length,false);
 memcpy(&data[var_uint],&_Right,_length*sizeof(StringType));
 }
return *this;
}
iString<StringType>& Append(const StringType* _Right,const unsigned int  _length)
{
if((_Right!=NULL)&&(_length!=0))
 {
 var_uint=Length;
 SetLength(Length+_length,false);
 memcpy(&(data[var_uint]),_Right,_length*sizeof(StringType));
 }
return *this;
}




iString<StringType>& operator += (const StringType& _Right) {return Append(_Right);}
iString<StringType>& Append(const StringType& str)
{
if(str!=NULL)
 {
 Append(str, 1);//02.06.2014 Append(str, iStrLen<StringType>(&str))
 }
return *this;
}

iString<StringType>& operator += (const StringType* _Right) {return Append(_Right);}
iString<StringType>& Append(const StringType* str)
{
if(str!=NULL)
 {
 Append(str, iStrLen<StringType>(str));//27.07.14  Append(*str, iStrLen<StringType>(str));
 }
return *this;
}

iString<StringType>&  operator += (const iString& _Right) {return Append(_Right); }         
iString<StringType>&  Append(const iString& str)    
{
if(&str!=NULL)
 {
 Append(str.data, str.Length);//27.07.14 Append(*str.data, str.Length);
 }
return (*this);
}

iString<StringType>&  operator += (const iString* _Right) {return Append(_Right); }         
iString<StringType>&  Append(const iString* str)    
{
if(str!=NULL)
 {
 Append(str->data, str->Length);
 }
return (*this);
}




//_length can without null
iString<char>&   AppendAsUTF8(const wchar_t *str,const unsigned int  _length)
{
if(_length==0){return (*this);}
int nChars = WideCharToMultiByte(CP_UTF8,0,str,_length,NULL,0,NULL,NULL);
if (nChars == 0) {return (*this);}
Allocate(Length+(nChars+1)*sizeof(char)); 
WideCharToMultiByte(CP_UTF8,0,str,_length,&data[Length],nChars,NULL,NULL);
if(data[Length+nChars]!=0){data[Length+nChars]=0;}

ResetLength();
return (*this);		
}		
		
iString<char>&   AppendAsUTF8(const wchar_t *str) 
{
AppendAsUTF8(str,iStrLen<wchar_t>(str));
return (*this);
}		
		
		
		 





iString<StringType>& AppendChar(const StringType str)
{
if(str!=NULL)
 {
 Append(str, 1);
 }
return *this;
}

            
            
iString<StringType> Appended(const StringType &_Right,const unsigned int  length)    const
{             
iString<StringType>  s(this);
s.Append(_Right,length);           
return s;           
}            
iString<StringType> Appended(const StringType *_Right,const unsigned int  length)    const
{             
iString<StringType>  s(this); 
s.Append(_Right,length);           
return (s);           
}            

friend iString<StringType> operator + (const iString& str1,const iString& str2)
{
if(str2.Length==0)return str1.Appended((StringType *)NULL,0);
return str1.Appended(*str2.data,str2.Length);
}

friend iString<StringType> operator + (const iString& str1,const StringType* str2)
{
if(str2==NULL){return str1;}
return str1.Appended(str2,iStrLen<StringType>(str2));// !! --(*str2,
}

friend iString<StringType> operator + (const iString& str,const StringType c)
{
return str.Appended(c,1);
}





StringType& operator[](const unsigned int  index)
{
if(index>=Length){throw OUTOFBOUND;}
return data[index];
}
StringType& operator[](const unsigned int  index) const
{
if(index>=Length){throw OUTOFBOUND;}
return data[index];
}






void ClearMem()
{
memset(data,0,sizeof(StringType)*Allocated);
}

    
const iString<StringType>& Remove(const unsigned int start, unsigned int length)
{
if(start>=Length){return *this;} 
if((start+length)> Length){length=Length-start;} 
if((start+length)==Length){SetLength(Length-length,false);return *this;}
memmove(&data[start],&data[start+length], (Length-(start+length))*sizeof(StringType));
SetLength(Length-length,false);
return *this;
}


const iString<StringType>& Insert( const StringType* str,const unsigned int length,const unsigned int position )
{
if(position>=Length){return *this;} 
if(length==0){return *this;} 
SetLength(Length+length,false);
memmove(&data[position+length],&data[position],(Length-length-position)*sizeof(StringType));
memcpy(&data[position],str,length*sizeof(StringType));
return *this;
}


const iString<StringType>& Replace(const StringType* SearchPattern,const StringType* ReplacePattern)
{
if(SearchPattern==NULL){return *this;}
unsigned int SearchPatternLength= iStrLen<StringType>(SearchPattern);
unsigned int ReplacePatternLength=iStrLen<StringType>(ReplacePattern);
int pos=0;
while ((pos=iStrPos<StringType>(data,SearchPattern,pos))!=-1)  
 {
 if(SearchPatternLength==ReplacePatternLength)
  {
  memcpy(&data[pos],ReplacePattern,SearchPatternLength*sizeof(StringType));
  pos++;
  }else{
  int difference=ReplacePatternLength-SearchPatternLength;
  if(difference>0)SetLength(Length+difference,false);
  memmove(&data[pos+ReplacePatternLength],&data[pos+SearchPatternLength],(Length-difference-pos-1)*sizeof(StringType));
  memcpy(&data[pos],ReplacePattern,ReplacePatternLength*sizeof(StringType));
  if(difference<0)SetLength(Length+difference,false);
  pos++;
  pos+=difference;
  }
 }
return (*this);
}


const iString<StringType>& ReplaceCharAt(unsigned int pos,const StringType* ReplacePattern)
{
if(ReplacePattern==NULL){return *this;}
if(pos>=Length){return *this;}
unsigned int ReplacePatternLength= iStrLen<StringType>(ReplacePattern);
 if(ReplacePatternLength==1)
  {
  memcpy(&data[pos],ReplacePattern,ReplacePatternLength*sizeof(StringType));
  }else{
  unsigned int difference=ReplacePatternLength-1;
  SetLength(Length+difference,false);
  memmove(&data[pos+ReplacePatternLength],&data[pos+1],(Length-difference-pos-1)*sizeof(StringType));
  memcpy(&data[pos],ReplacePattern,ReplacePatternLength*sizeof(StringType));
  }
return *this;
}






const iString<StringType>& TrimLeft( StringType* charactersToRemove,const unsigned int RemoveLength=1)
{
if(charactersToRemove==NULL){return *this;} 
int pos;
while((pos=IndexFirst(charactersToRemove))>=0)
{
 Remove(pos,RemoveLength);
}
return *this;
}

unsigned int GetLength()
{return Length;}

unsigned int GetAllocated()
{return Allocated;}

iString<StringType> SubString(const int start,const int end)  const
{
iString<StringType> s;    
if((end>start)&&(start>=0)&&(end>=0)&&(end<(int)Length))
 {
 s.SetLength(end-start+1);
 memcpy(&s.data[0],&data[start],s.Length*sizeof(StringType));
 }       
return s;
}

 iString<StringType> Left(const int end)  const
{
return SubString(0,end);
}
 iString<StringType> Right(const int start)  const
{
return SubString(start,Length-1);
}

                                                             // , unsigned int numberOfSearchChars=0 
int IndexFirst ( const iString<StringType> *searchChars,const unsigned int start=0) const
{
return  IndexFirst (searchChars->data,start); 
}


int IndexFirst (const StringType *searchChars,const unsigned int start=0 )  const  //  unsigned int numberOfSearchChars=0
{
if(searchChars==NULL){return -1;}  
if(start>=Length){return -1;} 
//if((start+numberOfSearchChars)>=Length){return -1;} 
return iStrPos<StringType>(&data[start]  ,searchChars);
}



int IndexLast (const StringType *searchChars,const unsigned int start=0 )  const  //  unsigned int numberOfSearchChars=0
{
if(searchChars==NULL){return -1;}  
if(start>=Length){return -1;} 
unsigned int search_len=iStrLen<StringType>(searchChars);
return iStrPosLast<StringType>(&data[start]  ,searchChars,Length-1,search_len);
}




void ResetLength()
{
unsigned int i=iStrLen<StringType>(data);
if(i>=Allocated){i=0;}
SetLength(i,false);
}


void SwapEndian()
{
if(sizeof(StringType)==2)
 {
 char c1;
 char c2;
 char* p_c1;
 char* p_c2;
 for(unsigned int i=0;i<Length;i++)
  {
  p_c1=(char*)&data[i];
  p_c2=p_c1+1;
  c1=*p_c1;
  c2=*p_c2;
  *p_c1=c2;
  *p_c2=c1;
  }
 }
}




void  AppendAsChar(const wchar_t *str,const unsigned int  _length,UINT CodePage)  
{
if(_length==0){return ;}
Allocate(Length+(_length+2)*sizeof(char));        
WideCharToMultiByte(CodePage, 0, str, -1,&data[Length] , _length+1 , NULL, NULL);
ResetLength();
}

void AppendAsChar(const wchar_t *str,UINT CodePage) 
{
AppendAsChar(str,iStrLen<wchar_t>(str),CodePage);
}

void AppendAsChar(const wchar_t *str) 
{
AppendAsChar(str,CP_ACP);
}






void  AppendAsChar(const char *str,const unsigned int  _length)  
{
Append(str,_length);
}
void AppendAsChar(const char *str) 
{
AppendAsChar(str,iStrLen<char>(str));
}



void  AppendAsWChar(const char *str,const unsigned int  _length,UINT SourceCodePage=CP_ACP)  
{
if(_length==0){return ;}
Allocate(Length+(_length+2)*sizeof(wchar_t));  
MultiByteToWideChar(SourceCodePage, 0, str, -1,&data[Length] , _length+1 );
ResetLength();
}
void  AppendAsWChar(const char *str)
{
AppendAsWChar(str,iStrLen<char>(str));
}  

void  AppendAsWChar(const wchar_t *str,const unsigned int  _length)  
{
Append(str,_length);
}
void  AppendAsWChar(const wchar_t *str)
{
Append(str,iStrLen<wchar_t>(str));
}  




int AsInt()   
{
return iStrAsUInt<StringType>(data,Length-1);
}

ULONGLONG AsUInt64()   
{
return iStrAsUInt64<StringType>(data,Length-1);
}



bool isStringOfDigits()   
{
if(Length==0)return false;
for(unsigned int i=0;i<Length;i++)
 {
 if (! iCharIsDigit<StringType>(data[i])) return false;
 }
return true;
}




 iString<StringType>& UpperCase();
 iString<StringType>& LowerCase();

};

 //ultoa  Reference  Data Conversion
////////////////////////////////////////////////  UNICODE
/*
isdigit
iswalpha
iswalnum
iswpunct
iswspace
*/


  




template<class iwchar_t> iString<iwchar_t>& iString<iwchar_t>::UpperCase() 
{
CharUpperBuffW(data,Length);
return *this;
}
template<class iwchar_t> iString<iwchar_t>& iString<iwchar_t>::LowerCase()
{
CharLowerBuffW(data,Length);
return *this;
}




#endif//  _iString_H


/*
iString<StringType>& operator=(const int& _Right) 
{	
if(&_Right== NULL){SetLength(0,false);return(*this);};

StringType buffer[256]; 
if(0!=_itow_s(_Right,&buffer[0],255,10)) {SetLength(0,false);return(*this);};
SetLength(iStrLen<StringType>(&buffer[0]),false);
memcpy(data,&buffer[0],Length*sizeof(StringType));
return (*this);
}
*/