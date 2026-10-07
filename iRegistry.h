#pragma once

#ifndef _IREGISTRY_H
#define _IREGISTRY_H

#include <iUString.h>

class  iRegistry
{
 HKEY hkey;
 unsigned int key_created_or_opened;  //1read 2 write
public:
 iString<char> keyName; 

HKEY hkey_root;


//HKEY_LOCAL_MACHINE  If you really are writing an administrative app then put a manifest on it that has a requestedExecutionLevel of requireAdministrator.  


iRegistry(char* key_Name,HKEY rootkey=HKEY_CURRENT_USER)
{
hkey=HKEY_LOCAL_MACHINE ; //default must be predefined
hkey_root=rootkey ;
keyName=key_Name;
key_created_or_opened=0;
}


 ~iRegistry(void)
 {   
if(!keyISpredefined(hkey)){RegCloseKey(hkey);}

 }


bool keyISpredefined(HKEY key)
{
if((key==HKEY_CLASSES_ROOT)||(key==HKEY_CURRENT_USER)||(key==HKEY_LOCAL_MACHINE)||(key==HKEY_USERS)||(key==HKEY_PERFORMANCE_DATA)||(key==HKEY_PERFORMANCE_TEXT)||(key==HKEY_PERFORMANCE_NLSTEXT)||(key==HKEY_CURRENT_CONFIG)||(key==HKEY_DYN_DATA)||(key==HKEY_CURRENT_USER_LOCAL_SETTINGS)){return true;}
return false;
}


bool  CreateKey()
{
if(key_created_or_opened==2){return true;}
if(!keyISpredefined(hkey)){RegCloseKey(hkey);}
DWORD dw;

dw=RegCreateKeyExA(hkey_root,keyName.Buffer(),0, NULL,0,KEY_WRITE,NULL,&hkey,NULL) ;
if(ERROR_SUCCESS!=dw){return false;}
key_created_or_opened=true;
return true;
}


bool  OpenKey(unsigned int Desired)
{
if(key_created_or_opened>=Desired){return true;}
if(!keyISpredefined(hkey)){RegCloseKey(hkey);}
REGSAM samDesired=KEY_READ;
if(Desired==2){samDesired=KEY_WRITE;}

if(!keyISpredefined(hkey)){RegCloseKey(hkey);}
if(ERROR_SUCCESS!=RegOpenKeyExA (hkey_root,keyName.Buffer(),0,samDesired ,&hkey)){return false;}   
key_created_or_opened=true;
return true;
}



DWORD get(const char* SubKeyName,const char*  valueName, DWORD defaultValue)
{
if(! OpenKey(1)){return defaultValue;} 
DWORD  dwType;
DWORD  Data;
DWORD  DataSize=sizeof(Data);
if(ERROR_SUCCESS!=RegGetValueA(hkey,SubKeyName,valueName,RRF_RT_ANY|RRF_NOEXPAND,&dwType,&Data,&DataSize)){return defaultValue;} 
if(REG_DWORD!=dwType){return defaultValue;} 
return Data;
}
void set(const char* SubKeyName,const char*  valueName, DWORD Value)
{
if(! CreateKey()){return ;} 
DWORD  DataSize=sizeof(Value);
DWORD dw;
//dw=RegSetValueExA(hkey,valueName,0,REG_DWORD,(BYTE*)&Value,DataSize); 
dw=RegSetKeyValueA(hkey,SubKeyName,valueName,REG_DWORD,&Value,DataSize);
if(ERROR_SUCCESS!=dw){}       
}


//set length of iString before 
bool get(const char* SubKeyName,const char*  valueName, iString<char> &result,char* defaultValue)
{
if(! OpenKey(1)){return false;} 
DWORD  dwType;
DWORD dw;
DWORD  DataSize=0; //must include the size of the terminating null character or characters.
dw=RegGetValueA(hkey,SubKeyName,valueName,RRF_RT_ANY|RRF_NOEXPAND,&dwType,NULL,&DataSize);
if((dw== ERROR_MORE_DATA)||(dw==0))
 {
 result.SetLength(DataSize-1);     
 dw=RegGetValueA(hkey,SubKeyName,valueName,RRF_RT_ANY|RRF_NOEXPAND,&dwType,(void*)result.Buffer(),&DataSize);
 result.ResetLength();
 }
if(ERROR_SUCCESS!=dw){return false;} 
if(REG_SZ!=dwType){return false;} 
return true;
}

bool get(const wchar_t* SubKeyName,const wchar_t*  valueName, iString<wchar_t> &result,wchar_t* defaultValue)
{
if(! OpenKey(1)){return false;} 
DWORD  dwType;
DWORD dw;
DWORD  DataSize=0; //must include the size of the terminating null character or characters.
dw=RegGetValueW(hkey,SubKeyName,valueName,RRF_RT_ANY|RRF_NOEXPAND,&dwType,NULL,&DataSize);
if((dw== ERROR_MORE_DATA)||(dw==0))
 {
 result.SetLength(DataSize-1);     
 dw=RegGetValueW(hkey,SubKeyName,valueName,RRF_RT_ANY|RRF_NOEXPAND,&dwType,(void*)result.Buffer(),&DataSize);
 result.ResetLength();
 }
if(ERROR_SUCCESS!=dw){return false;} 
if(REG_SZ!=dwType){return false;} 
return true;
}




bool set(const char* SubKeyName,const char*  valueName, char* Value)
{
if(! CreateKey()){return false;} 
DWORD  DataSize=(unsigned int)strlen(Value)+sizeof(char);
DWORD dw; 
dw=RegSetKeyValueA(hkey,SubKeyName,valueName,REG_SZ,Value,DataSize);
return (ERROR_SUCCESS==dw);       
}





__int64 get64(const char* SubKeyName,const char*  valueName, __int64 defaultValue)
{
if(! OpenKey(1)){return defaultValue;} 
DWORD  dwType;
DWORD  Data;
DWORD  DataSize=sizeof(Data);
if(ERROR_SUCCESS!=RegGetValueA(hkey,SubKeyName,valueName,RRF_RT_ANY|RRF_NOEXPAND,&dwType,&Data,&DataSize)){return defaultValue;} 
if(REG_QWORD!=dwType){return defaultValue;} 
return Data;
}
void set64(char* SubKeyName,char*  valueName, __int64 Value)
{
if(! CreateKey()){return ;} 
DWORD  DataSize=sizeof(Value);
DWORD dw;
dw=RegSetKeyValueA(hkey,SubKeyName,valueName,REG_QWORD,&Value,DataSize);
if(ERROR_SUCCESS!=dw){}       
}


bool getBinary(const char* SubKeyName,const char*  valueName, char*Buffer ,DWORD  BufferSize )
{
if(! OpenKey(1)){return false;} 
DWORD  dwType;
if(ERROR_SUCCESS!=RegGetValueA(hkey,SubKeyName,valueName,RRF_RT_ANY|RRF_NOEXPAND,&dwType,&Buffer,&BufferSize)){return false;} 
if(REG_BINARY!=dwType){return false;} 
return true;
}




void setBinary(const char* SubKeyName,const char*  valueName, char* Value,DWORD  DataSize)
{
if(! CreateKey()){return ;} 
DWORD dw;
//dw=RegSetValueExA(hkey,valueName,0,REG_DWORD,(BYTE*)&Value,DataSize); 
dw=RegSetKeyValueA(hkey,SubKeyName,valueName,REG_BINARY,&Value,DataSize);
if(ERROR_SUCCESS!=dw){}       
}





};//class  iRegistry


#endif  //_IREGISTRY_H



