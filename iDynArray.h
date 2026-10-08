#pragma once
#ifndef _IDYNARRAY_H
#define _IDYNARRAY_H

//creates the object on the heap(new) and one on the stack(without new).
//Without new, it depends on where you declare it. Within a function or method, it uses stack


#define  iDynArrayZEROINIT  0x00000001
#define  iDynArrayAUTOADD   0x00000002


int  _OnUShortCompare(unsigned short x1,unsigned short x2)  {return (x1-x2);}





///template<typename  el>
template<class  el>
class iDynArray
{
private:
   
    unsigned int Count; 
    unsigned int AllocatedCount; 


public:
  el* Array;
  DWORD Options;

  float kGrow;
  enum exception { MEMFAIL,OUTOFBOUND }; 
  typedef el& reference;
  typedef const el& const_reference;

  typedef int (* iDynArray_SORT_PROC)(const el &x1, const el &x2);
  iDynArray_SORT_PROC OnSortCompare;
  el Temp;
        
   
const el&  operator[]( unsigned int index) const 
{
if(index>=Count){if(Options&iDynArrayAUTOADD){SetCount(index+1);}else{throw OUTOFBOUND;}}// return NULL;
return (Array[index]);  
}

el& operator[](unsigned int index)
{
if(index>=Count){if(Options&iDynArrayAUTOADD){SetCount(index+1);}else{throw OUTOFBOUND;}}
return (Array[index]);  
}





el& operator = (const el &a)
{
if (this == &a) 	return *this;
Options=a.Options;
SetCount(a.Count); 
memcpy(Array, a.Array, sizeof(el)*a.Count);
kGrow=a.kGrow;
return *this;
}


iDynArray(const iDynArray &a)
{   
Array=NULL;
Options=a.Options;
Allocate(a.AllocatedCount);
memcpy(Array, a.Array, sizeof(el)*a.AllocatedCount);
Count = a.Count;
kGrow=a.kGrow;
OnSortCompare= NULL;
}

iDynArray(unsigned int AllocateCount=0, float kGrowth=0.3 )
{
Array=NULL;
Options=0;
Count = 0;
AllocatedCount = 0;
kGrow=kGrowth; 
Allocate(AllocateCount); 
OnSortCompare= NULL;
}



~iDynArray()
{
    if (Array)
    {
    	free(Array);
    	Array = NULL;
    }
}


//< 0 elem1 less than elem2 
//0 elem1 equivalent to elem2 
//> 0 elem1 greater than elem2
void Sort()
{
if(OnSortCompare== NULL){return;}
if(Count<2){return;}
bool sorted=false;
unsigned int right=Count-1;
unsigned int i;
while (!sorted)
 {
 sorted=true;
 for(i=0;i<right;i++)
  {
  if(OnSortCompare(Array[i],Array[i+1])>0)
   { 
   memcpy(&Temp,&Array[i],sizeof(el));
   memcpy(&Array[i],&Array[i+1],sizeof(el));
   memcpy(&Array[i+1],&Temp,sizeof(el));
   sorted=false;
   }
  }

 }
}

unsigned int GetCount()
{
    return Count; 
}



void SetCount(unsigned int NewCount)
{
if (NewCount>Count)  ///
 {
 Allocate(NewCount,false);
 }
Count = NewCount;
}


void Insert(const el &item, unsigned int index)
{
    if (index > Count)
    {
        if (Options & iDynArrayAUTOADD) { SetCount(index + 1); }
        else { throw OUTOFBOUND; }
    }
    else
    {
        if (index == Count) 
        { 
            SetCount(Count + 1); 
            memcpy(&Array[index], &item, sizeof(el)); 
            return; 
        }
        
        // Запоминаем старый размер для правильного смещения памяти
        unsigned int oldLength = Count - index; 
        SetCount(Count + 1);
        
        // Сдвигаем элементы вправо, используя сохраненное старое количество
        memmove(&Array[index + 1], &Array[index], sizeof(el) * oldLength);
    }
    
    // Копируем сам элемент на освободившееся место
    memcpy(&Array[index], &item, sizeof(el));
}


void Add(const el &item)
{
if(Count<=AllocatedCount)
 {
 Allocate((unsigned int)((Count+1)*(1+kGrow)));
 }
memcpy(&Array[Count],&item,sizeof(el)); 
Count++;
}

void Delete(unsigned int index)
{
if (index>=Count){return;}
if ((index==(Count-1))||(Count == 1)){Count--;return;}
memmove(&Array[index],&Array[index+1],sizeof(el)*(Count-1-index));
if(Options&iDynArrayZEROINIT) {memset(&Array[Count-1],0,sizeof(el));}
}




void Allocate(unsigned int NewCount,bool CanFree=true) 
{
 if(AllocatedCount == NewCount){return;}
 if(!CanFree){if(NewCount<AllocatedCount){return;}}
	el * new_ptr = (el *)realloc(Array, sizeof(el)*NewCount); 
 if (new_ptr == NULL){throw MEMFAIL; return;}
 Array=new_ptr;
 if((Options&iDynArrayZEROINIT)&&(NewCount>AllocatedCount)) {memset(&Array[AllocatedCount],0,(NewCount-AllocatedCount)*sizeof(el));}
	AllocatedCount = NewCount;
}

unsigned int GetAllocatedCount(){return AllocatedCount;}



el* getPointer()
{
    return Array;  
}




void freemem(void* buffer)
{
    if (!buffer) return;

    // Извлекаем заголовок менеджера памяти, который находится прямо ПЕРЕД пользовательским буфером
    iMemManHeader* h = (iMemManHeader*)buffer - 1;

    if (ReUseMemory)
    {
        // Вычисляем физическую позицию освобождаемого блока внутри чанка (кроссплатформенный char*)
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
        // Передаем параметры строго по сигнатуре вашего iDynArray: (объект, индекс)
        aFreeBlock.Insert(newBlock, insertIdx); 
        return;
    }
}





  

}



#endif // ifndef _IDYNARRAY_H