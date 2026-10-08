#pragma once

#ifndef _IUTILS_H
#define _IUTILS_H


#ifdef _WINDOWS_
#ifdef _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

 /*
 void  ShowMessageW(uint32_t dw)
{
WCHAR buffer[256];
FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM,0,dw,0,&buffer[0],256,0);
MessageBoxW(0 ,&buffer[0],L"",0);
}
 */


#include <iUString.h>
#include <iStringUtil.h>



bool iFileCheckSumW(const wchar_t* FileName, uint32_t *lpSum)
{

if(lpSum==0)return false;
HANDLE h=CreateFileW(FileName,GENERIC_READ,FILE_SHARE_READ ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
if (h==INVALID_HANDLE_VALUE)return false;

uint32_t FileSizeHigh=0;
uint32_t Size= GetFileSize(h,&FileSizeHigh);
Size=Size>>2;
Size=Size<<2;

 
HANDLE memHeap=GetProcessHeap();
uint32_t* buffer=(uint32_t*)HeapAlloc(memHeap, HEAP_ZERO_MEMORY, Size);
if (buffer==NULL) {CloseHandle(h);return false;}

uint32_t dw=0;
if (0==ReadFile (h,buffer,Size,&dw,NULL)){HeapFree(memHeap,0,buffer);CloseHandle(h);return false;} 

*lpSum=0;
Size=Size>>2;
for(uint32_t i=0;i<Size;i++) 
	(*lpSum)+=buffer[i];


HeapFree(memHeap,0,buffer);
CloseHandle(h);
return true;
}

//retvalSizeBytes incl. null
template <typename StringType>  bool iChangeFileExt(const StringType* str,const StringType* NewExt,StringType* retval,const uint32_t retvalSizeCharacters) 
{
unsigned int len= iStrLen<StringType>(str);
unsigned int NewExt_pos=len;
for(unsigned int i=(len-1);i>0;i--)
 {
 if((str[i]=='\\')||(str[i]=='/')) 
  {NewExt_pos=i+1;break;}
 if(str[i]=='.')
  {NewExt_pos=i;break;}
 }
unsigned int NewExt_len=iStrLen<StringType>(NewExt); 
bool retvalue=true;
if((NewExt_pos<retvalSizeCharacters)&&(NewExt_pos>0)) 
 { memcpy(retval,str,NewExt_pos*sizeof(StringType)); retval[NewExt_pos]=0;}else{retvalue=false;}
if(((NewExt_pos+NewExt_len)<retvalSizeCharacters)&&(NewExt_len>0)) {memcpy(&(retval[NewExt_pos]),NewExt,NewExt_len*sizeof(StringType));retval[NewExt_pos+NewExt_len]=0;}else{retvalue=false;}
return retvalue;
}


//retvalSizeBytes incl. null
template <typename StringType>  bool iExtractFilePath(const StringType* str,StringType* retval,const uint32_t retvalSizeCharacters) 
{
unsigned int len= iStrLen<StringType>(str);
for(unsigned int i=(len-1);i>0;i--)
 {
 if(str[i]==iPATH_SEPARATOR) 
  {
   if((i+1)>retvalSizeCharacters)return false;
   memcpy(retval,str,(i+1)*sizeof(StringType));
   retval[i+1]=0;
   return true;
  }
 }
if((len+2)>retvalSizeCharacters)return false;
memcpy(retval,str,len*sizeof(StringType));
retval[len]=iPATH_SEPARATOR;
retval[len+1]=0;
return true;
}





int CALLBACK iIsFontInstalledW_CALLBACK(CONST LOGFONTW *lpelf,CONST TEXTMETRICW *lpntm,uint32_t FontType,LPARAM lParam)	
{
*((bool*)lParam)=true;

if(lParam) {return 0;}else{return 1;}  
}
   

bool iIsFontInstalledW(const wchar_t* fontname)
{
bool bFontInstalled=0;
HDC dc=CreateDCW(L"DISPLAY",NULL,NULL,NULL);
if(dc==NULL) return false;
EnumFontFamiliesW(dc,fontname,iIsFontInstalledW_CALLBACK,(LPARAM)&bFontInstalled);
DeleteDC(dc);
return bFontInstalled;
}

bool iFileExistsW(const wchar_t* filename)
{
WIN32_FIND_DATAW f; 
HANDLE h=FindFirstFileW(filename,&f);
if(INVALID_HANDLE_VALUE==h)return false;
FindClose(h);
return true;
}


bool iDirectoryExistsW(const wchar_t* filename)
{
uint32_t  Code = GetFileAttributesW(filename);
return (Code != 0xFFFFFFFF) && (FILE_ATTRIBUTE_DIRECTORY & Code);
}

















#endif  //_IUTILS_H
















