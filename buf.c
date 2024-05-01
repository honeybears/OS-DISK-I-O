#include <stdio.h>
#include <stdlib.h>
#include "buf.h"
#include "queue.h"


struct bufList      bufList[MAX_BUFLIST_NUM];
struct stateList    stateList[MAX_BUF_STATE_NUM];
struct freeList     freeListHead;
struct lruList      lruListHead;


void BufInit(void)
{
    for(int i = 0; i < MAX_BUFLIST_NUM; i++){
        CIRCLEQ_INIT(&bufList[i]);
    }
    for(int i = 0; i < MAX_BUF_STATE_NUM; i++){
        CIRCLEQ_INIT(&stateList[i]);
    }
    CIRCLEQ_INIT(&freeListHead);
    CIRCLEQ_INIT(&lruListHead);
}

Buf* BufFind(int blkno)
{
    Buf* tmp;
    int hash = blkno % MAX_BUFLIST_NUM;
    CIRCLEQ_FOREACH(tmp, &bufList[hash],  blist){
        if(tmp-> blkno == blkno){
            return tmp;
        }
    }
    return NULL;
}


void BufRead(int blkno, char* pData)
{
    Buf* tmp;
    int hash = blkno % MAX_BUFLIST_NUM;
    CIRCLEQ_FOREACH(tmp, &bufList[hash],  blist){
        if(tmp-> blkno == blkno){
            pData = tmp -> pMem;
            return;
        }
    }

    pData = NULL;

}


void BufWrite(int blkno, char* pData)
{
    Buf* tmp;
    int hash = blkno % MAX_BUFLIST_NUM;

    CIRCLEQ_FOREACH(tmp, &bufList[hash],  blist){
        if(tmp-> blkno == blkno){
            tmp -> pMem = pData;
            tmp -> state = BUF_STATE_DIRTY;
            CIRCLEQ_REMOVE(&bufList[hash], tmp, blist);
            CIRCLEQ_REMOVE(&stateList[BUF_CLEAN_LIST], tmp, blist);
            
            return;
        }
    }
}

void BufSync(void)
{

}

void BufSyncBlock(int blkno)
{

}


int GetBufInfoInStateList(BufStateList listnum, Buf* ppBufInfo[], int numBuf)
{

}

int GetBufInfoInLruList(Buf* ppBufInfo[], int numBuf)
{

}


int GetBufInfoInBufferList(int index, Buf* ppBufInfo[], int numBuf)
{

}

