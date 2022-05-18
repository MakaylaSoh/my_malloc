/* Makayla Soh 10/22/2021
*  File name: MAKAYLA_SOH_PROGRAM_2.cpp
*  Description: create functions malloc() and free() using sbrk() and brk()
*/

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <unistd.h>
#include <iostream>
#include <time.h>

using namespace std;

#define PAGESIZE 4096
typedef unsigned char BYTE;

void* startofheap = NULL;
 
typedef struct chunkhead
{
    unsigned int info; //0 is free, 1 is filled
    unsigned int size; //size of space (including chunkhead)
    chunkhead *next, *prev;
} chunkhead;


/*
* Function: mymalloc
* Parameter: interger representing size of bytes wanted
* Returns: the address in memory where there is free space 
*/
void *mymalloc(int size)
{
    //size calculations
    int pages = (int)((size + sizeof(chunkhead)) / PAGESIZE) + 1;
    int realSize = pages * PAGESIZE; //total size needed

    //traverse heap
    chunkhead *p = (chunkhead*)startofheap;
    chunkhead *bestFit = NULL;
    chunkhead *lastEl = NULL;
    while(p != NULL)
    {
        //check chunk size and avaliability
        if(p -> size >= realSize && p -> info == 0)
        {
            if(bestFit == NULL || p -> size < bestFit -> size)
                bestFit = p;
        }

        lastEl = p;
        p = p -> next;
    }

    //add onto heap (no chunks meet requirements)
    if(bestFit == NULL)
    {
        chunkhead *newEl = (chunkhead*)sbrk(realSize);
        newEl -> info = 1;
        newEl -> size = realSize;
        newEl -> prev = lastEl;
        newEl -> next = NULL;
        if(lastEl != NULL) lastEl -> next = newEl;
        else startofheap = newEl; //start of heap
        return (void *)((BYTE*)newEl + sizeof(chunkhead));
    }

    //fitting chunk exists
    bestFit -> info = 1;
    bestFit -> size = realSize;

    //split
    if(bestFit -> size > realSize)
    {
        chunkhead *newEl = (chunkhead *)((BYTE *)bestFit + realSize);
        newEl -> info = 0;
        newEl -> size = bestFit -> size - realSize;
        newEl -> prev = bestFit;
        newEl -> next = bestFit -> next;
        if(bestFit -> next != 0)
            (bestFit -> next) -> prev = newEl;
        bestFit -> next = newEl;        
    }

    return (void *)((BYTE*)bestFit + sizeof(chunkhead));
}


/*
* Function: myfree
* Parameter: BYTE pointer to the chunk being freed
* Description: no returns; chunk at pointer should be freed and merged with other free space
*/
void myfree(BYTE *a)
{
    //change info to free
    chunkhead *aHead = (chunkhead *)(a - sizeof(chunkhead));
    aHead -> info = 0;

    chunkhead *upperEl = aHead -> prev;
    chunkhead *lowerEl = aHead -> next;

    //merge with upper element
    if(upperEl != NULL && upperEl -> info == 0)
    {
        upperEl -> next = lowerEl;
        if(lowerEl != 0)
            lowerEl -> prev = upperEl;
        upperEl -> size += aHead -> size;
        aHead = upperEl;
    }

    //merge with lower element
    if(lowerEl == 0)
    {
        if(aHead -> prev != NULL) aHead -> prev -> next = NULL;
        else startofheap = NULL;
        brk(aHead);
    }
    if(lowerEl != NULL && lowerEl -> info == 0)
    {
        aHead -> next = lowerEl -> next;
        aHead -> size += lowerEl -> size;
    }

    return;
}



void analyze()
{
    printf("\n-----------------------------------------------------------\n");
    if(!startofheap) 
    {
        cout << "No heap. Program break on address: " << sbrk(0) << endl;
        return;
    }

    chunkhead *p = (chunkhead *)startofheap;
    for(int element = 0; p; p = (chunkhead *)p -> next, element++)
    {
        cout << element << " | current address " << p 
             << " | size: " << p -> size << " | info: " << p-> info 
             << " | next: " << p -> next << " | prev: " << p -> prev << endl;
    }
    cout << "Program break on address: " << sbrk(0) << endl;
    return;
}


int main()
{   
    BYTE *a[100];
    clock_t ca = clock(), cb;
    analyze();
    for(int i = 0; i<100; i++) a[i] = (BYTE *)mymalloc(1000);
    for(int i = 0; i < 90; i++) myfree(a[i]);
    analyze();
    myfree(a[95]);
    a[95] = (BYTE*)mymalloc(1000);
    analyze();
    for(int i = 90; i < 100; i++) myfree(a[i]);
    analyze();
    cb = clock();
    cout << "\nduration: " << (double)(cb-ca) << endl;
    return 0;
}