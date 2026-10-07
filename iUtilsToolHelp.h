#pragma once

#ifndef _IUTILS_TOOLHELP_H
#define _IUTILS_TOOLHELP_H



#include <TlHelp32.h>
#include <iUString.h>


#include <tchar.h>

int GetProcessModuleENTRY (DWORD dwPID,DWORD dwModuleID,LPMODULEENTRY32 lpMe32)
{
bool bFound=false;
 
memset(lpMe32,0,sizeof(MODULEENTRY32));
HANDLE  hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
if(hModuleSnap==INVALID_HANDLE_VALUE){return -1;}
lpMe32->dwSize = sizeof(MODULEENTRY32);

if (Module32First(hModuleSnap, lpMe32)==TRUE)
 {
 do
  {
	  if (lpMe32->th32ProcessID=dwPID) {CloseHandle (hModuleSnap); return 1;} 
  } while  (Module32Next(hModuleSnap, lpMe32)==TRUE);
 }
CloseHandle (hModuleSnap);
return 0;
}


int GetProcessModuleENTRYW (DWORD dwPID,DWORD dwModuleID,LPMODULEENTRY32W lpMe32)
{
bool bFound=false;
 
memset(lpMe32,0,sizeof(MODULEENTRY32));
HANDLE  hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
if(hModuleSnap==INVALID_HANDLE_VALUE){return -1;}
lpMe32->dwSize = sizeof(MODULEENTRY32);

if (Module32FirstW(hModuleSnap, lpMe32)==TRUE)
 {
 do
  {
	  if (lpMe32->th32ProcessID=dwPID) {CloseHandle (hModuleSnap); return 1;} 
  } while  (Module32NextW(hModuleSnap, lpMe32)==TRUE);
 }
CloseHandle (hModuleSnap);
return 0;
}






 int process_is_running(const TCHAR* Name,bool isFullPath)
{ 
PROCESSENTRY32 p;
MODULEENTRY32 me32;
HANDLE snapshot_HANDLE;


snapshot_HANDLE=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
if(snapshot_HANDLE==INVALID_HANDLE_VALUE){return -1;} 

memset(&p,0,sizeof(PROCESSENTRY32));
p.dwSize=sizeof(PROCESSENTRY32);
if (Process32First(snapshot_HANDLE,&p)==FALSE){ CloseHandle(snapshot_HANDLE);return -1;}  


do
 {
 if (isFullPath)
  {
  if (GetProcessModuleENTRY(p.th32ProcessID,p.th32ModuleID,&me32)<1) continue; 
  if(_stricoll(Name,me32.szExePath)==0){CloseHandle(snapshot_HANDLE);return 1;}
  }else{
  if(_stricoll(Name,p.szExeFile)==0){CloseHandle(snapshot_HANDLE);return 1;}
 }
 }while (Process32Next(snapshot_HANDLE, &p)==TRUE);
CloseHandle(snapshot_HANDLE);
return 0;
}




 int process_is_runningW(const wchar_t* Name,bool isFullPath)
{ 
PROCESSENTRY32W p;
MODULEENTRY32W me32;
HANDLE snapshot_HANDLE;


snapshot_HANDLE=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
if(snapshot_HANDLE==INVALID_HANDLE_VALUE){return -1;} 

memset(&p,0,sizeof(PROCESSENTRY32));
p.dwSize=sizeof(PROCESSENTRY32);
if (Process32FirstW(snapshot_HANDLE,&p)==FALSE){ CloseHandle(snapshot_HANDLE);return -1;}  


do
 {
 if (isFullPath)
  {
  if (GetProcessModuleENTRYW(p.th32ProcessID,p.th32ModuleID,&me32)<1) continue; 
  if(_wcsicoll(Name,me32.szExePath)==0){CloseHandle(snapshot_HANDLE);return 1;}
  }else{
  if(_wcsicoll(Name,p.szExeFile)==0){CloseHandle(snapshot_HANDLE);return 1;}
 }
 }while (Process32NextW(snapshot_HANDLE, &p)==TRUE);
CloseHandle(snapshot_HANDLE);
return 0;
}







#endif  //_IUTILS_TOOLHELP_H
















