#pragma once

#ifndef _ISKLONENIE_H
#define _ISKLONENIE_H


#include <iUString.h>
#include <iStringUtil.h>
#include <iUtils.h>

#include <shellapi.h>


bool f_woman_not_sklon(wchar_t c)
{

return ((c==L'о') || (c==L'и') || (c==L'х') || (c==L'ч') || (c==L'б') 
			|| (c==L'в') || (c==L'г') || (c==L'д') || (c==L'ж') || (c==L'з')
			|| (c==L'п') || (c==L'н') || (c==L'м') || (c==L'л') || (c==L'к')
			|| (c==L'р') || (c==L'с') || (c==L'т') || (c==L'ф') || (c==L'ц')
			|| (c==L'ь') || (c==L'щ') || (c==L'ш')); 


}

bool f_man_not_sklon(wchar_t c)
{
return ((c==L'о') || (c==L'и') || (c==L'х') || (c==L'я') || (c==L'а'));
}




void padeg_func1(iString<wchar_t> &s,unsigned int del,const wchar_t* add)
{
if(s.GetLength()==0)return;
s.SetLength(s.GetLength()-del);
s+=add;
}


void fioRP(iString<wchar_t> &fio,iString<wchar_t> &out)
{
iString<wchar_t> f1,i1,o1;


bool dot=false;
int mode=0;
for(unsigned int i=0;i<fio.GetLength();i++)
 {
 if((fio[i]==L' ')||(fio[i]==L'.'))
  { 
  if(fio[i]==L'.')dot=true;
  if(mode==0)
   {
   if((f1.GetLength()!=0)&&(i1.GetLength()==0))mode++;
   };
  if(mode==1)
   {
   if((i1.GetLength()!=0)&&(o1.GetLength()==0))mode++;
   };
  continue;
  };
 if(mode==0){f1+=fio[i];};
 if(mode==1){i1+=fio[i];};
 if(mode==2){o1+=fio[i];};
 }

wchar_t f_last=0;if(f1.GetLength()!=0)f_last=f1[f1.GetLength()-1];
wchar_t o_last=0;if(o1.GetLength()!=0)o_last=o1[o1.GetLength()-1];

wchar_t i_last=0;if(i1.GetLength()!=0)i_last=i1[i1.GetLength()-1];
wchar_t i_last2=0;if(i1.GetLength()>1)i_last2=i1[i1.GetLength()-2];


if(dot)
 {
 if(!(f_last==L'о') || (f_last==L'и'))
  {
  switch (f_last)
   {
   case L'я':{padeg_func1(f1,2,L"ой");break;}
   case L'а':{padeg_func1(f1,1,L"ой");break;}
   case L'й':{padeg_func1(f1,2,L"ого");break;}
   default: {padeg_func1(f1,0,L"а");break;}
   }
  }
 out=f1+L" "+i1+L"."+o1+L".";
 return;
 }

bool man=(o_last==L'ч');

if(man)
 {
 if(!f_man_not_sklon(f_last))
  {
  switch (f_last)
   {
   case L'й':{padeg_func1(f1,2,L"ого");break;}
   default: {padeg_func1(f1,0,L"а");break;}
   }
  }
 switch (i_last)
  {
  case L'й':{padeg_func1(i1,1,L"я");break;}
  case L'ь':{padeg_func1(i1,1,L"я");break;}
  default: {padeg_func1(i1,0,L"а");break;}
  }
 padeg_func1(o1,0,L"а");
 }else{
 if(!f_woman_not_sklon(f_last))
  {
  switch (f_last)
   {
   case L'я':{padeg_func1(f1,2,L"ой");break;}
   default: {padeg_func1(f1,0,L"ой");break;}
   }
  }
 switch (i_last)
  {
  case L'а':{if((i_last2==L'и')||(i_last2==L'г')){padeg_func1(i1,1,L"и");}else{padeg_func1(i1,1,L"ы");};break;}
  case L'я':{padeg_func1(i1,1,L"и");break;}
  case L'ь':{padeg_func1(i1,1,L"и");break;}
  default:break;
  }
 padeg_func1(o1,0,L"ы");
 }
out=f1+L" "+i1+L" "+o1+L" ";
}




void fioDP(iString<wchar_t> &fio,iString<wchar_t> &out)
{
iString<wchar_t> f1,i1,o1;


bool dot=false;
int mode=0;
for(unsigned int i=0;i<fio.GetLength();i++)
 {
 if((fio[i]==L' ')||(fio[i]==L'.'))
  { 
  if(fio[i]==L'.')dot=true;
  if(mode==0)
   {
   if((f1.GetLength()!=0)&&(i1.GetLength()==0))mode++;
   };
  if(mode==1)
   {
   if((i1.GetLength()!=0)&&(o1.GetLength()==0))mode++;
   };
  continue;
  };
 if(mode==0){f1+=fio[i];};
 if(mode==1){i1+=fio[i];};
 if(mode==2){o1+=fio[i];};
 }

wchar_t f_last=0;if(f1.GetLength()!=0)f_last=f1[f1.GetLength()-1];
wchar_t o_last=0;if(o1.GetLength()!=0)o_last=o1[o1.GetLength()-1];

wchar_t i_last=0;if(i1.GetLength()!=0)i_last=i1[i1.GetLength()-1];
wchar_t i_last2=0;if(i1.GetLength()>1)i_last2=i1[i1.GetLength()-2];


if(dot)
 {
 if(!(f_last==L'о') || (f_last==L'и'))
  {
  switch (f_last)
   {
   case L'я':{padeg_func1(f1,2,L"ой");break;}
   case L'а':{padeg_func1(f1,1,L"ой");break;}
   case L'й':{padeg_func1(f1,2,L"ому");break;}
   default: {padeg_func1(f1,0,L"у");break;}
   }
  }
 out=f1+L" "+i1+L"."+o1+L".";
 return;
 }

bool man=(o_last==L'ч');

if(man)
 {
 if(!f_man_not_sklon(f_last))
  {
  switch (f_last)
   {
   case L'й':{padeg_func1(f1,2,L"ому");break;}
   default: {padeg_func1(f1,0,L"у");break;}
   }
  }
 switch (i_last)
  {
  case L'й':{padeg_func1(i1,1,L"ю");break;}
  case L'ь':{padeg_func1(i1,1,L"ю");break;}
  default: {padeg_func1(i1,0,L"у");break;}
  }
 padeg_func1(o1,0,L"у");
 }else{
 if(!f_woman_not_sklon(f_last))
  {
  switch (f_last)
   {
   case L'я':{padeg_func1(f1,2,L"ой");break;}
   default: {padeg_func1(f1,0,L"ой");break;}
   }
  }
 switch (i_last)
  {
  case L'а':{if(i_last2==L'и'){padeg_func1(i1,1,L"и");}else{padeg_func1(i1,1,L"е");};break;}
  case L'я':{if(i_last2==L'и'){padeg_func1(i1,1,L"и");}else{padeg_func1(i1,1,L"е");};break;}
  case L'ь':{padeg_func1(i1,1,L"и");break;}
  default:break;
  }
 padeg_func1(o1,0,L"е");
 }
out=f1+L" "+i1+L" "+o1+L" ";
}


bool Soglasnaya(wchar_t c)
{
return (
  (c==L'б')||(c==L'в')||(c==L'г')||(c==L'д')||(c==L'ж')
||(c==L'з')||(c==L'й')||(c==L'к')||(c==L'л')||(c==L'м')
||(c==L'н')||(c==L'п')||(c==L'р')||(c==L'с')||(c==L'т')
||(c==L'ф')||(c==L'х')||(c==L'ц')||(c==L'ч')||(c==L'ш')
||(c==L'щ')
||(c==L'Б')||(c==L'В')||(c==L'Г')||(c==L'Д')||(c==L'Ж')
||(c==L'З')||(c==L'Й')||(c==L'К')||(c==L'Л')||(c==L'М')
||(c==L'Н')||(c==L'П')||(c==L'Р')||(c==L'С')||(c==L'Т')
||(c==L'Ф')||(c==L'Х')||(c==L'Ц')||(c==L'Ч')||(c==L'Ш')
||(c==L'Щ'));
}



void WordRP(iString<wchar_t> &str)
{
iString<wchar_t> last3,last2;
if(str.GetLength()<3){return;}
last2=&str[str.GetLength()-2];
//last3=&str[str.GetLength()-3];
wchar_t last=str[str.GetLength()-1]; 
if(last==L'.')return;

wchar_t*add=NULL;
unsigned int del=0;

if(last2==L"ий"){del=2;add=L"ого";}
if(last2==L"ец"){del=2;add=L"ца";}
if(last2==L"ой"){del=2;add=L"ого";}
if(last2==L"ый"){del=2;add=L"ого";}
if(last2==L"да"){del=0;add=NULL;}

if((del!=0)||(add!=NULL))
 {
 padeg_func1(str,del,add);
 return;
 }

if(last==L'а'){del=1;add=L"ы";}
if(last==L'й'){del=1;add=L"я";}
if(last==L'ь'){del=1;add=L"я";}
if(last==L'а'){del=1;add=L"ы";}

if((del!=0)||(add!=NULL))
 {
 padeg_func1(str,del,add);
 return;
 }


if(Soglasnaya(last)){del=0;add=L"а";}
if((del!=0)||(add!=NULL))
 {
 padeg_func1(str,del,add);
 return;
 }
}


void WordDP(iString<wchar_t> &str)
{
iString<wchar_t> last3,last2;
if(str.GetLength()<3){return;}
last2=&str[str.GetLength()-2];
//last3=&str[str.GetLength()-3];
wchar_t last=str[str.GetLength()-1]; 
if(last==L'.')return;

wchar_t*add=NULL;
unsigned int del=0;

if(last2==L"ий"){del=2;add=L"ему";}
if(last2==L"ец"){del=2;add=L"цу";}
if(last2==L"ой"){del=2;add=L"ому";}
if(last2==L"ый"){del=2;add=L"ому";}
if(last2==L"да"){del=2;add=L"де";}

if((del!=0)||(add!=NULL))
 {
 padeg_func1(str,del,add);
 return;
 }


if(last==L'а'){del=1;add=L"е";}
if(last==L'й'){del=1;add=L"ю";}
if(last==L'ь'){del=1;add=L"ю";}


if((del!=0)||(add!=NULL))
 {
 padeg_func1(str,del,add);
 return;
 }


if(Soglasnaya(last)){del=0;add=L"у";}
if((del!=0)||(add!=NULL))
 {
 padeg_func1(str,del,add);
 return;
 }
}
	 



	
	



#endif  //_ISKLONENIE_H
















