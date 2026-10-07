#pragma once
#ifndef _IMEMMAN_H
#define _IMEMMAN_H
/// limits.h  #define iMAXDWORD  0xFFFFFFFF;

//////////////////////////////////////////////////////////////////////////////////
//iMemMan gMemMan;
/*
void* operator new (size_t size) 
  {
  return 0;
  }

void* operator new[ ] (size_t size)
  {
  return 0;
  }

void operator delete (void* pointerToDelete)
  {
  return;
  }

void operator delete[ ] (void* arrayToDelete)
  {
  return;
  }   

 */


//////////////////////////////////////////////////////////////////////////////////

#ifdef _WINDOWS_
#include <minwindef.h>
#endif//   _WINDOWS_


#include "iDynArray.h"
#include <array>

struct iMemManChunk
{
void* pointer;
size_t Size;
size_t FreePosition;

};
struct iMemManBlock
{
 DWORD ChunkIndex;
 DWORD Position;
 DWORD Size;
};

struct iMemManHeader
{
 DWORD ChunkIndex;
 DWORD Size;
};



class iMemMan
{
private:
iDynArray<iMemManChunk> achunk;
unsigned int ActiveChunkIndex;//can>=achunk.count
bool ReUseMemory;
size_t TotalAllocated;
iDynArray<iMemManBlock> aFreeBlock;
public:
float KGrow;
enum exception { MEMFAIL };

 iMemMan(size_t PreallocationSize,float kGrow=0.3,DWORD EstimatedChunkCount=4,bool reUseMemory=true,DWORD EstimatedBlockCountForReUseMemory=32)
{
TotalAllocated=0;
if(EstimatedChunkCount==0){EstimatedChunkCount=1;}
achunk.Allocate(EstimatedChunkCount);   
if(ReUseMemory){aFreeBlock.Allocate(EstimatedBlockCountForReUseMemory);};  
ActiveChunkIndex=0;
ReUseMemory=reUseMemory;
if(PreallocationSize!=0){setMemSize(PreallocationSize);}
KGrow=kGrow;
}


 ~iMemMan(void)
 {
 }



void getmem(void* buffer,size_t Size)
{

if(ReUseMemory)
{
unsigned int xPos=aFreeBlock.GetCount();
if(xPos!=0)
{
xPos<<=1;
unsigned int Step=xPos;
if(Step>1){Step<<=1;}
while(Step>0)
 {
 if(Size>aFreeBlock[xPos].Size)
  { 
  xPos+=Step;
  }else{
  if(Step==1){break;}
  if(Size==aFreeBlock[xPos].Size){break;}
  xPos-=Step;
  }
 Step<<=1;
 if(xPos>=aFreeBlock.GetCount())
  {xPos=aFreeBlock.GetCount()-1;}
 };
if(aFreeBlock[xPos].Size>=Size)
 {  
 buffer=(PCHAR)achunk[aFreeBlock[xPos].ChunkIndex].pointer+aFreeBlock[xPos].Position;
 ((iMemManHeader*)buffer)->ChunkIndex=aFreeBlock[xPos].ChunkIndex;
 ((iMemManHeader*)buffer)->Size=aFreeBlock[xPos].Size;
 buffer=(iMemManHeader*)buffer+1;
 return;
 }
}//if(xPos!=0)   aFreeBlock.GetCount()>0
}//if ReUseMemory

for(unsigned int i=ActiveChunkIndex;i<achunk.GetCount();i++)
{
 if((achunk[i].Size-achunk[i].FreePosition)>=(Size+sizeof(iMemManHeader)))
 {
 iMemManHeader* h=(iMemManHeader*)achunk[i].pointer+achunk[i].FreePosition;
 h->ChunkIndex=i;
 h->Size=Size;
 buffer=h+1;      
 achunk[i].FreePosition+=Size+sizeof(iMemManHeader);
 if(achunk[i].FreePosition=achunk[i].Size){ActiveChunkIndex++;}
 return;
 }
}

//add mem
size_t add=Size+sizeof(iMemManHeader); 
if((size_t)((KGrow+1.0)*TotalAllocated)>add) {add=(size_t)(KGrow+1.0)*TotalAllocated;}
setMemSize(add);

for(unsigned int i=ActiveChunkIndex;i<achunk.GetCount();i++)
{
 if((achunk[i].Size-achunk[i].FreePosition)>=(Size+sizeof(iMemManHeader)))
 {
 iMemManHeader* h=(iMemManHeader*)achunk[i].pointer+achunk[i].FreePosition;
 h->ChunkIndex=i;
 h->Size=Size;
 buffer=h+1;      
 achunk[i].FreePosition+=Size+sizeof(iMemManHeader);
 if(achunk[i].FreePosition=achunk[i].Size){ActiveChunkIndex++;}
 return;
 }
}

throw MEMFAIL;
}

void freemem(void* buffer)
{
if(!ReUseMemory){return;}

iMemManHeader* h=(iMemManHeader*)buffer-1;

unsigned int xPos=aFreeBlock.GetCount();
xPos<<=1;
unsigned int Step=xPos;
if(Step>1){Step<<=1;}
while(Step>0)
 {
 if(h->Size>aFreeBlock[xPos].Size)
  { 
  xPos+=Step;
  }else{
  if(Step==1){break;}
  if(h->Size==aFreeBlock[xPos].Size){break;}
  xPos-=Step;
  }
 Step<<=1;
 if(xPos>=aFreeBlock.GetCount())
  {xPos=aFreeBlock.GetCount()-1;}
 };
iMemManBlock b; b.ChunkIndex=h->ChunkIndex;b.Size=h->Size;b.Position=((PCHAR)h-(PCHAR)achunk[h->ChunkIndex].pointer);
aFreeBlock.Insert(b,xPos);
}


void setMemSize(size_t newSize,bool AllowRelease=false)
{
if(newSize>TotalAllocated)
{
 size_t TotalAdd=newSize-TotalAllocated;
 iMemManChunk chunk;
 int Attempt=1;


 Attempt=0; 
 chunk.Size=TotalAdd;
 while ((TotalAdd>0)&&(chunk.Size>0)&&(Attempt<1024)) 
  {
  chunk.pointer=(PCHAR)malloc(chunk.Size);
  if(chunk.pointer==NULL)
   {
   chunk.Size=(size_t)(chunk.Size/1.2);Attempt++;
   }else{
   TotalAdd-=chunk.Size;TotalAllocated+=chunk.Size;chunk.FreePosition=0;achunk.Add(chunk);
   }
  };
 if(TotalAdd!=0){throw MEMFAIL;}
 }else{ // if(newSize>TotalAllocated)
 if(newSize==TotalAllocated){return ;}
 if(AllowRelease)
  {
  size_t TotalFree=TotalAllocated-newSize;
  unsigned int remove_chunks=0;
  for(unsigned int i=achunk.GetCount()-1;i>=0;i--)
   {
    if(achunk[i].Size>TotalFree){break;}
    if(achunk[i].pointer!=NULL){free(((void*)(achunk[i].pointer)));achunk[i].pointer=NULL;TotalFree-=achunk[i].Size;TotalAllocated-=achunk[i].Size;remove_chunks++;}
   }
  if(remove_chunks!=0){achunk.SetCount(achunk.GetCount()-remove_chunks);}
  }//if(AllowRelease)
 }//else if(newSize>TotalAllocated)


}


size_t getMemSize()
{
return TotalAllocated;
}






};    //class iMemMan

#endif //_IMEMMAN_H 