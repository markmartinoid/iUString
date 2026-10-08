///// // Mark Martin 2007-2025 all rights reserved markmartinoid@gmail.com 
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
ReUseMemory=reUseMemory;
if(ReUseMemory){aFreeBlock.Allocate(EstimatedBlockCountForReUseMemory);};  
ActiveChunkIndex=0;

if(PreallocationSize!=0){setMemSize(PreallocationSize);}
KGrow=kGrow;
}


 ~iMemMan(void)
 {
 }



void getmem(void*& buffer, size_t Size) // Передаем buffer по ссылке (&), чтобы вернуть адрес наружу
{
    if (ReUseMemory)
    {
        unsigned int count = aFreeBlock.GetCount();
        if (count != 0)
        {
            // Классический бинарный поиск лучшего подходящего блока (Best Fit)
            int low = 0;
            int high = (int)count - 1;
            int bestIdx = -1;

            while (low <= high)
            {
                int mid = low + (high - low) / 2;

                if (aFreeBlock[mid].Size >= Size)
                {
                    bestIdx = mid;     // Блок подходит, но ищем дальше влево, вдруг есть блок еще ближе к Size
                    high = mid - 1;
                }
                else
                {
                    low = mid + 1;     // Блок слишком мал, ищем в правой половине
                }
            }

            // Если подходящий свободный блок найден
            if (bestIdx != -1)
            {
                // Вычисляем физический адрес в памяти чанка (кроссплатформенный char*)
                char* chunkPtr = (char*)achunk[aFreeBlock[bestIdx].ChunkIndex].pointer;
                buffer = chunkPtr + aFreeBlock[bestIdx].Position;

                // Записываем заголовок менеджера памяти
                ((iMemManHeader*)buffer)->ChunkIndex = aFreeBlock[bestIdx].ChunkIndex;
                ((iMemManHeader*)buffer)->Size = aFreeBlock[bestIdx].Size;

                // Сдвигаем указатель buffer вперед, чтобы пользователь получил чистую память после заголовка
                buffer = (iMemManHeader*)buffer + 1;

                // Удаляем использованный блок из списка свободных с помощью вашего метода Delete
                aFreeBlock.Delete(bestIdx); 
                return;
            }
        }
    }

    // Если повторно использовать нечего, выделяем память из активных чанков
    for (unsigned int i = ActiveChunkIndex; i < achunk.GetCount(); i++)
    {
        if ((achunk[i].Size - achunk[i].FreePosition) >= (Size + sizeof(iMemManHeader)))
        {
            // Вычисляем адрес нового блока с учетом смещения
            char* chunkPtr = (char*)achunk[i].pointer;
            iMemManHeader* h = (iMemManHeader*)(chunkPtr + achunk[i].FreePosition);
            
            h->ChunkIndex = i;
            h->Size = Size;
            
            buffer = h + 1; // Возвращаем указатель на область ПОСЛЕ заголовка
            
            achunk[i].FreePosition += Size + sizeof(iMemManHeader);
            
            if (achunk[i].FreePosition == achunk[i].Size) 
            {
                ActiveChunkIndex++;
            }
            return;
        }
    }

    // Если свободного места не осталось ни в одном чанке, расширяем пул памяти
    size_t add = Size + sizeof(iMemManHeader);
    if ((size_t)((KGrow + 1.0) * TotalAllocated) > add) 
    {
        add = (size_t)((KGrow + 1.0) * TotalAllocated);
    }
    
    setMemSize(add);
    
    // Рекурсивная попытка выделить память в свежесозданном чанке
    getmem(buffer, Size);
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
    if (!buffer) return;

    // Извлекаем заголовок менеджера памяти, который находится прямо ПЕРЕД пользовательским буфером
    iMemManHeader* h = (iMemManHeader*)buffer - 1;

    if (ReUseMemory)
    {
        // Вычисляем физическую позицию освобождаемого блока внутри чанка
        char* chunkStart = (char*)achunk[h->ChunkIndex].pointer;
        char* blockStart = (char*)h;
        size_t position = blockStart - chunkStart;

        // Создаем запись о новом свободном блоке
        iFreeBlock newBlock;
        newBlock.ChunkIndex = h->ChunkIndex;
        newBlock.Size = h->Size;
        newBlock.Position = position;

        unsigned int count = aFreeBlock.GetCount();
        
        // Бинарный поиск правильной позиции для вставки (чтобы массив aFreeBlock оставался отсортирован по Size)
        int low = 0;
        int high = (int)count - 1;
        int insertIdx = count; // По умолчанию вставляем в конец

        while (low <= high)
        {
            int mid = low + (high - low) / 2;

            if (aFreeBlock[mid].Size >= newBlock.Size)
            {
                insertIdx = mid; // Нашли место, где блок >= нашего, но ищем дальше влево для точной позиции
                high = mid - 1;
            }
            else
            {
                low = mid + 1;
            }
        }

        // Вставляем блок в отсортированную позицию
        // Примечание: Проверьте, как в вашем iDynArray называется метод вставки. 
        // Обычно это Insert(insertIdx, newBlock). Если метода Insert нет, используйте код ниже:
        aFreeBlock.Insert(insertIdx, newBlock); 
        return;
    }

    // Если ReUseMemory отключен, память просто "утекает" до сброса всего менеджера,
    // так как линейный аллокатор не умеет двигать FreePosition назад для произвольных блоков.
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