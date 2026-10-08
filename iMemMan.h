#pragma once

#ifndef _IMEMMAN_H
#define _IMEMMAN_H

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

// Подключаем ваш динамический массив
#include "iDynArray.h"

// Константа ошибки выделения памяти
#define MEMFAIL 1
#define OUTOFBOUND 2

// Кросплатформенный заголовок блока памяти (размер равен 8 байтам — идеально для выравнивания)
struct iMemManHeader
{
    uint32_t ChunkIndex;
    uint32_t Size;
};

// Структура для отслеживания свободных блоков (Best Fit)
struct iFreeBlock
{
    uint32_t ChunkIndex;
    size_t Size;
    size_t Position;
};

// Структура физического чанка памяти
struct iMemManChunk
{
    char* pointer; // Изменено с PCHAR на кроссплатформенный char*
    size_t Size;
    size_t FreePosition;
};

class iMemMan
{
private:
    iDynArray<iMemManChunk> achunk;
    iDynArray<iFreeBlock> aFreeBlock;
    
    unsigned int ActiveChunkIndex;
    size_t TotalAllocated;
    double KGrow;
    bool ReUseMemory;
    unsigned int EstimatedBlockCountForReUseMemory;

public:
    // Конструктор с исправленным порядком инициализации ReUseMemory
    iMemMan(bool reUseMemory = true, double kGrow = 0.5, unsigned int estimatedBlockCountForReUseMemory = 1024)
    {
        KGrow = kGrow;
        ActiveChunkIndex = 0;
        TotalAllocated = 0;
        EstimatedBlockCountForReUseMemory = estimatedBlockCountForReUseMemory;
        ReUseMemory = reUseMemory; // Инициализируем ДО проверки условия ниже

        if (ReUseMemory)
        {
            aFreeBlock.Allocate(EstimatedBlockCountForReUseMemory);
        }
    }

    // Деструктор: гарантированно освобождает всю выделенную через malloc память чанков
    ~iMemMan()
    {
        for (unsigned int i = 0; i < achunk.GetCount(); i++)
        {
            if (achunk[i].pointer != NULL)
            {
                free(achunk[i].pointer);
                achunk[i].pointer = NULL;
            }
        }
    }

    // Выделение памяти (Best Fit бинарный поиск + линейный аллокатор)
    void getmem(void*& buffer, size_t Size)
    {
        if (ReUseMemory)
        {
            unsigned int count = aFreeBlock.GetCount();
            if (count != 0)
            {
                int low = 0;
                int high = (int)count - 1;
                int bestIdx = -1;

                // Классический рабочий бинарный поиск подходящего блока
                while (low <= high)
                {
                    int mid = low + (high - low) / 2;

                    if (aFreeBlock[mid].Size >= Size)
                    {
                        bestIdx = mid; 
                        high = mid - 1; // Ищем дальше влево, вдруг есть блок еще ближе к нужному Size
                    }
                    else
                    {
                        low = mid + 1;
                    }
                }

                if (bestIdx != -1)
                {
                    char* chunkPtr = achunk[aFreeBlock[bestIdx].ChunkIndex].pointer;
                    buffer = chunkPtr + aFreeBlock[bestIdx].Position;

                    ((iMemManHeader*)buffer)->ChunkIndex = aFreeBlock[bestIdx].ChunkIndex;
                    ((iMemManHeader*)buffer)->Size = (uint32_t)aFreeBlock[bestIdx].Size;

                    buffer = (iMemManHeader*)buffer + 1;

                    // Удаляем использованный блок с помощью вашего метода Delete
                    aFreeBlock.Delete(bestIdx); 
                    return;
                }
            }
        }

        // Выделение из активных чанков пула
        for (unsigned int i = ActiveChunkIndex; i < achunk.GetCount(); i++)
        {
            if ((achunk[i].Size - achunk[i].FreePosition) >= (Size + sizeof(iMemManHeader)))
            {
                char* chunkPtr = achunk[i].pointer;
                iMemManHeader* h = (iMemManHeader*)(chunkPtr + achunk[i].FreePosition);
                
                h->ChunkIndex = i;
                h->Size = (uint32_t)Size;
                
                buffer = h + 1;
                
                achunk[i].FreePosition += Size + sizeof(iMemManHeader);
                
                // Исправлена опечатка '=' на корректное сравнение '=='
                if (achunk[i].FreePosition == achunk[i].Size)
                {
                    ActiveChunkIndex++;
                }
                return;
            }
        }

        // Расширение пула, если памяти не хватило
        size_t add = Size + sizeof(iMemManHeader);
        if ((size_t)((KGrow + 1.0) * TotalAllocated) > add) 
        {
            add = (size_t)((KGrow + 1.0) * TotalAllocated);
        }
        
        setMemSize(add);
        getmem(buffer, Size); // Рекурсивный повтор в новом чанке
    }

    // Возврат памяти в пул свободных блоков
    void freemem(void* buffer)
    {
        if (!buffer) return;

        iMemManHeader* h = (iMemManHeader*)buffer - 1;

        if (ReUseMemory)
        {
            char* chunkStart = achunk[h->ChunkIndex].pointer;
            char* blockStart = (char*)h;
            size_t position = blockStart - chunkStart;

            iFreeBlock newBlock;
            newBlock.ChunkIndex = h->ChunkIndex;
            newBlock.Size = h->Size;
            newBlock.Position = position;

            unsigned int count = aFreeBlock.GetCount();
            int low = 0;
            int high = (int)count - 1;
            int insertIdx = count; 

            // Бинарный поиск позиции для сохранения сортировки по Size
            while (low <= high)
            {
                int mid = low + (high - low) / 2;

                if (aFreeBlock[mid].Size >= newBlock.Size)
                {
                    insertIdx = mid;
                    high = mid - 1;
                }
                else
                {
                    low = mid + 1;
                }
            }

            // Вставка строго по сигнатуре вашего iDynArray: (объект, индекс)
            aFreeBlock.Insert(newBlock, insertIdx); 
            return;
        }
    }

    // Управление физическим размером пула памяти
    void setMemSize(size_t newSize, bool AllowRelease = false)
    {
        if (newSize > TotalAllocated)
        {
            size_t TotalAdd = newSize - TotalAllocated;
            iMemManChunk chunk;
            int Attempt = 0;

            chunk.Size = TotalAdd;
            while ((TotalAdd > 0) && (chunk.Size > 0) && (Attempt < 1024)) 
            {
                chunk.pointer = (char*)malloc(chunk.Size);
                if (chunk.pointer == NULL)
                {
                    chunk.Size = (size_t)(chunk.Size / 1.2);
                    Attempt++;
                }
                else
                {
                    TotalAdd -= chunk.Size;
                    TotalAllocated += chunk.Size;
                    chunk.FreePosition = 0;
                    achunk.Add(chunk);
                }
            }

            if (TotalAdd != 0)
            {
                throw MEMFAIL;
            }
        }
        else 
        { 
            if (newSize == TotalAllocated) { return; }
            
            if (AllowRelease)
            {
                size_t TotalFree = TotalAllocated - newSize;
                int lastIndex = (int)achunk.GetCount() - 1;
                
                // Исправлен бесконечный цикл за счет использования знакового типа int
                for (int i = lastIndex; i >= 0; i--)
                {
                    if (achunk[i].Size > TotalFree) { break; }
                    
                    if (achunk[i].pointer != NULL)
                    {
                        free(achunk[i].pointer);
                        achunk[i].pointer = NULL;
                        
                        TotalFree -= achunk[i].Size;
                        TotalAllocated -= achunk[i].Size;
                        
                        // Безопасное поочередное удаление чанков с конца массива
                        achunk.Delete(i); 
                    }
                }
                
                if (ActiveChunkIndex >= achunk.GetCount())
                {
                    ActiveChunkIndex = (achunk.GetCount() > 0) ? achunk.GetCount() - 1 : 0;
                }
            }
        }
    }
};

#endif // _IMEMMAN_H
