#pragma once
#ifndef _IDYNARRAY_H
#define _IDYNARRAY_H
// ©2007-2015 by Mark Martin markmartinoid@gmail.com / bug fixes by GOOGLE AI
//creates the object on the heap(new) and one on the stack(without new).
//Without new, it depends on where you declare it. Within a function or method, it uses stack
// el Temp member variable and replacing it with a stack-allocated variable (local_temp) inside the loop, preventing potential data corruption and memory overhead by a GOOGLE AI advice

#define  iDynArrayZEROINIT  0x00000001
#define  iDynArrayAUTOADD   0x00000002


inline int  _OnUShortCompare(unsigned short x1,unsigned short x2)  {return (x1-x2);}





///template<typename  el>
template<class  el>
class iDynArray
{
private:
   
    unsigned int Count; 
    unsigned int AllocatedCount; 


public:
  el* Array;
  uint32_t Options;

  float kGrow;
  enum exception { MEMFAIL,OUTOFBOUND, OUTOFMEMORY_INT32 }; 
  typedef el& reference;
  typedef const el& const_reference;

  typedef int (* iDynArray_SORT_PROC)(const el &x1, const el &x2);
  iDynArray_SORT_PROC OnSortCompare;
  // el Temp; removes}d coz unsave multithreaded
        
   
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




// This goes inside your class template <class el> class iDynArray
iDynArray& operator = (const iDynArray &a)
{
    // 1. Check for self-assignment (e.g., array1 = array1)
    if (this == &a) return *this;

    // 2. Copy the configuration fields
    Options = a.Options;
    kGrow = a.kGrow;

    // 3. Resize our internal buffer to hold the new items
    SetCount(a.Count); 

    // 4. Copy the raw memory bytes safely
    if (a.Count > 0 && a.Array != NULL)
    {
        memcpy(Array, a.Array, sizeof(el) * a.Count);
    }

    // 5. Return a reference to this array object
    return *this;
}





iDynArray(const iDynArray &a, const bool CompactMem=false)
{   
Array=NULL;
Options=a.Options;

if(CompactMem){
Allocate(a. Count); 
memcpy(Array, a. Array, sizeof(el)*a. Count);
}else{
Allocate(a.AllocatedCount);
memcpy(Array, a.Array, sizeof(el)*a.AllocatedCount);
}

Count = a.Count;
kGrow=a.kGrow;
OnSortCompare= NULL;
}


iDynArray(unsigned int AllocateCount = 0, float kGrowth = 0.3f)
{
    Array = NULL;
    Options = 0;
    Count = 0;
    OnSortCompare = NULL;
if (AllocateCount<16)AllocateCount=16;

    if (kGrowth < 0.05f) kGrowth = 0.3f;
    if (kGrowth > 128.00f) kGrowth = 128.00f;//that class for large bases
    kGrow = kGrowth;

if (AllocateCount > UINT_MAX / sizeof(el)) throw exception { MEMFAIL,OUTOFMEMORY_INT32 };

    if (AllocateCount > 0)
    {
        Allocate(AllocateCount);
    }
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
   el local_temp; // SafeThreading Created on the stack only when swapping 
    memcpy(&local_temp, &Array[i], sizeof(el));
    memcpy(&Array[i], &Array[i+ 1], sizeof(el));
    memcpy(&Array[i+ 1], &local_temp, sizeof(el));
    sorted= false;
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
    
    // Создаем локальную копию элемента на стеке ДО изменения размера массива.
    // Если realloc сместит Array в памяти, наша копия на стеке останется невредимой.
    el local_item;
    memcpy(&local_item, &item, sizeof(el));

    if (index == Count) 
    { 
        SetCount(Count + 1); 
        memcpy(&Array[index], &local_item, sizeof(el)); 
        return; 
    }
    
    // Запоминаем количество элементов для сдвига до изменения Count
    unsigned int oldLength = Count - index; 
    SetCount(Count + 1);
    
    // Сдвигаем элементы вправо
    memmove(&Array[index + 1], &Array[index], sizeof(el) * oldLength);
    
    // Копируем сохраненный элемент на освободившееся место
    memcpy(&Array[index], &local_item, sizeof(el));
}


// we assuming use preemptive Allocated Count in constructor
void Add(const el &item)
{
    el local_item;
    memcpy(&local_item, &item, sizeof(el));

    if (Count >= AllocatedCount)
    {
#ifdef IDYNARRAY_SMALL_ARRAYS
    unsigned int newCapacity = (AllocatedCount == 0) ? 16 : (unsigned int)(AllocatedCount * (1.0f + kGrow));
    if (newCapacity <= Count) newCapacity = Count + 1;
    Allocate(newCapacity);
 #else   
        Allocate((unsigned int)((Count + 1) * (1.0f + kGrow)));
#endif        
    }

    memcpy(&Array[Count], &local_item, sizeof(el));
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




  

};



#endif // ifndef _IDYNARRAY_H