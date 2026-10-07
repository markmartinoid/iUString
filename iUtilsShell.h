#pragma once

#ifndef _IUTILS_SHELL_H
#define _IUTILS_SHELL_H


#include <iUString.h>
#include <iStringUtil.h>
#include <iUtils.h>


//#include <Shlwapi.h>
//#include <shellapi.h>

#include <Shldisp.h>

//#include <iRegistry.h>


//filename must be double null-terminated.
bool iInstalFontW(const wchar_t* fontname,const wchar_t* filename,bool permanent)
{


if(! permanent)
{
if(0==AddFontResourceW(filename))return false;
}else{
iString<wchar_t>  newpath;
newpath.Allocate(MAX_PATH+1);
UINT x=GetWindowsDirectoryW((wchar_t*)newpath.Buffer(),newpath.GetAllocated());
newpath.ResetLength();
if((x>newpath.GetLength())||(x==0))return false;
newpath.ResetLength(); 

if(newpath[newpath.GetLength()-1]!='\\') 
	newpath.Append('\\'); 
newpath.Append(L"Fonts\\"); 


newpath.Append(ExtractFileName(filename));

if((newpath.GetLength()+1)<newpath.GetAllocated())
	((wchar_t*)newpath.Buffer())[newpath.GetLength()+1]=0;

//if(iFileExistsW(newpath.Buffer()))return false;//check to see if the filename already exists in that directory. If it does, the program should rename your .TTF file to some other name, perhaps by appending a number to the end of the basename.

if(!iFileExistsW(filename))return false;







if(0==CopyFileW(filename,newpath.Buffer(),FALSE))
 {
 SHFILEOPSTRUCTW fo;
 memset(&fo,0,sizeof(fo));
 fo.hwnd= GetActiveWindow();//GetForegroundWindow  
 fo.wFunc=FO_COPY;
 fo.pFrom=filename;//must be double null-terminated.
 fo.pTo=newpath.Buffer();//must be double null-terminated.
 fo.fFlags=FOF_FILESONLY		;
 fo.lpszProgressTitle=0;
 fo.hNameMappings=0;
 int i=SHFileOperationW(&fo);
 if (0!=i)return true;
 if (fo.fAnyOperationsAborted)return true;
 }


iString<char> c;
c.AppendAsChar(fontname);
iString<char> c2;
c2.AppendAsChar(ExtractFileName(newpath.Buffer()));
char * c0=0;

/*
iRegistry reg("Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts\\",HKEY_LOCAL_MACHINE);
bool b=reg.set(c0,c.Buffer(),(char *)c2.Buffer());
if(!b)
{return false;}
*/
//LSTATUS ST=SHSetValueW(HKEY_LOCAL_MACHINE,L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts\\",fontname,REG_SZ,newpath.Buffer(),newpath.GetLength()+1);




if(0==AddFontResourceW((wchar_t*)newpath.Buffer()))
{return false;}

}//if(! permanent)

return true;
}








#endif  //_IUTILS_SHELL_H
















