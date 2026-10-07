#pragma once

#ifndef _INETHTMLUTILS_H
#define _INETHTMLUTILS_H

#include <iUString.h>
#include "iDynArray.h"
#include "iNetCodes.h"

template <typename StringType>unsigned int iHTMLCharset(const StringType *str,unsigned int &pos,iString<StringType> &charset)
{
if(str.GetEmpty()){charset.SetEmpty();return 0;}
unsigned int Index;
const StringType constcharset[8]={' ','c','h','a','r','s','e','t'};
const unsigned int constcharsetMax=7;
pos=0;
Index=0;
while (pos<pos_end){if(str[pos]==constcharset[Index]){if(Index==constcharsetMax)break;Index++;}else{break;} pos++;} 
if(Index!=constcharsetMax){return 0;}
pos++;
while (pos<pos_end){if(str[pos]!=32){break;}else{pos++;}}//backspace
if((str[pos]==34)||(str[pos]==39))pos++;
while (pos<pos_end)
 {
 if((str[pos]==34)||(str[pos]==39)||(str[pos]==32) ||(str[pos]==62))break;
 charset.Append(str[pos],1);
 }

if (charset=="utf-8"){return CP_UTF8;}
if (charset=="UTF-8"){return CP_UTF8;}

return 0;
}





//check
template <typename StringType>bool iHTMLTagValue(const StringType *str,const StringType *TagType,const StringType *TagName,unsigned int &pos_str_begin,unsigned int &pos_str_end)   
{
if(str.GetEmpty())return false;	
unsigned int TagTypeMax=iStrLen(TagType)-1;
unsigned int TagNameMax=iStrLen(TagName)-1;
unsigned int pos_end=str.GetLength()-1;
pos_str_begin=0;
pos_str_end=0;
unsigned int Index;
const StringType name[5]={' ','n','a','m','e'};
const unsigned int nameMax=4;

unsigned int pos=0;
while (pos<pos_end)
 {
 if(pos_str_begin==0)
  {
  if(str[pos]!=60){pos++;continue;}
  pos++;
  while (pos<pos_end){if(str[pos]!=32){break;}else{pos++;}}//backspace
  if(pos==pos_end)break;
  Index=0;
  while (pos<pos_end){if(str[pos]==TagType[Index]){if(Index==TagTypeMax)break;Index++;}else{break;} pos++;} 
  if(Index!=TagTypeMax){pos++;continue;}
  pos++;
  while (pos<pos_end){if(str[pos]!=32){break;}else{pos++;}}//backspace
  if(pos==pos_end)break;

  Index=0;
  while (pos<pos_end){  if(str[pos]==name[Index]){if(Index==nameMax)break;  Index++;   }else{Index=0;}   pos++;} 
  if(pos==pos_end)break;
  if(Index!=nameMax){pos++;continue;}
  while (pos<pos_end){if(str[pos]!=32){break;}else{pos++;}}//backspace
  if(str[pos]!=61){pos++;continue;}
  while (pos<pos_end){if(str[pos]!=32){break;}else{pos++;}}//backspace

  Index=0;
  while (pos<pos_end){  if(str[pos]==TagName[Index]){if(Index==TagNameMax)break;  Index++;   }else{break}   pos++;} 
  if(pos==pos_end)break;
  if(Index!=nameMax){pos++;continue;}


  while (pos<pos_end){if(str[pos]==62){pos++;if(pos==pos_end)break; pos_str_begin=pos;   } 

  }else{//if(pos_str_begin==0)
  if(str[pos]!=62){continue;}else{if(pos!=0)pos_str_end=pos-1;}
  pos++;
  while (pos<pos_end){if(str[pos]!=32){break;}else{pos++;}}//backspace
  if(pos==pos_end){pos_str_end=0;break;}
  if(str[pos]!=47){pos_str_end=0;continue;}
  pos++;
  while (pos<pos_end){if(str[pos]!=32){break;}else{pos++;}}//backspace
  if(pos==pos_end){pos_str_end=0;break;}
  Index=0;
  while (pos<pos_end){if(str[pos]==TagType[Index]){if(Index==TagTypeMax)break; Index++;}else{break;}pos++;} 
  if(Index!=TagTypeMax){pos_str_end=0;continue;}
  pos++;
  while (pos<pos_end){if(str[pos]!=32){break;}else{pos++;}}//backspace
  if(pos==pos_end){pos_str_end=0;break;}
  if(str[pos]!=62){pos_str_end=0;continue;}
  }
 }

if(pos_str_end==0){pos_str_begin=0;return false;}

return true;
}









#endif  //_INETHTMLUTILS_H
















