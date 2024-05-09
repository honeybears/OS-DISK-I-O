#include <stdio.h>
#include <stdlib.h>
#include "buf.h"
#include "queue.h"
#include "disk.h"
#include <string.h>

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
    for(int i = 0; i < MAX_BUF_NUM; i++){
        Buf* block = (Buf*) malloc(BUFSIZ);

        // TODO :free list link 만들어지면 구현
        // CIRCLEQ_INSERT_HEAD(&freeListHead, block,);
    }
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
            memcpy(pData,tmp->pMem,BLOCK_SIZE);
            CIRCLEQ_REMOVE(&lruListHead,tmp,llist);
            CIRCLEQ_INSERT_TAIL(&lruListHead,tmp,llist);
            return;
        }
    }

    // TODO :free list link 만들어지면 malloc 대체
    Buf* newBuf =(Buf*)malloc(sizeof(Buf));
    newBuf->blkno = blkno;
    newBuf->pMem = (void*)malloc(BLOCK_SIZE);

    DevReadBlock(blkno,newBuf->pMem);

    CIRCLEQ_INSERT_HEAD(&bufList[hash],newBuf,blist);

    CIRCLEQ_INSERT_TAIL(&stateList[BUF_CLEAN_LIST], newBuf, blist);
    CIRCLEQ_INSERT_TAIL(&lruListHead,tmp,llist);

    memcpy(pData,tmp->pMem,BLOCK_SIZE);


}


void BufWrite(int blkno, char* pData)
{
    Buf* tmp;
    int hash = blkno % MAX_BUFLIST_NUM;

    CIRCLEQ_FOREACH(tmp, &bufList[hash],  blist){
        if(tmp-> blkno == blkno){
            memcpy(tmp->pMem, pData, BLOCK_SIZE);

            if(tmp->state == BUF_STATE_DIRTY){
                CIRCLEQ_REMOVE(&lruListHead, tmp, llist);
                CIRCLEQ_INSERT_TAIL(&lruListHead, tmp, llist);
                return;
            }

            if(tmp->state == BUF_STATE_CLEAN){

                tmp->state = BUF_STATE_DIRTY;

                CIRCLEQ_REMOVE(&stateList[BUF_CLEAN_LIST], tmp, slist);
                CIRCLEQ_INSERT_TAIL(&stateList[BUF_DIRTY_LIST], tmp, slist);

                CIRCLEQ_REMOVE(&lruListHead, tmp, llist);
                CIRCLEQ_INSERT_TAIL(&lruListHead, tmp, llist);

            }

            return;
        }
    }

    // TODO :free list link 만들어지면 malloc 대체
    Buf* newBuf = (Buf*)malloc(sizeof(Buf));
    newBuf->blkno = blkno;
    newBuf->pMem = (void*)malloc(BLOCK_SIZE);

    newBuf->pMem = pData;

    CIRCLEQ_INSERT_HEAD(&bufList[hash], newBuf, blist);

    CIRCLEQ_INSERT_TAIL(&stateList[BUF_DIRTY_LIST], newBuf, slist);

    CIRCLEQ_INSERT_TAIL(&lruListHead, newBuf, llist);

}

void BufSync(void)
{
    Buf* tmp;
    CIRCLEQ_FOREACH(tmp, &stateList[BUF_DIRTY_LIST], slist){

        CIRCLEQ_REMOVE(&stateList[BUF_DIRTY_LIST], tmp, slist);

        DevWriteBlock(tmp->blkno,tmp->pMem);

        tmp->state = BUF_STATE_CLEAN;

        CIRCLEQ_INSERT_TAIL(&stateList[BUF_CLEAN_LIST], tmp, slist);

    }
}

void BufSyncBlock(int blkno)
{
    Buf* tmp;
    CIRCLEQ_FOREACH(tmp, &stateList[BUF_DIRTY_LIST], slist){

        if(tmp->blkno == blkno){

            CIRCLEQ_REMOVE(&stateList[BUF_DIRTY_LIST], tmp, slist);

            DevWriteBlock(tmp->blkno, tmp->pMem);

            tmp->state = BUF_STATE_CLEAN;

            CIRCLEQ_INSERT_TAIL(&stateList[BUF_CLEAN_LIST], tmp, slist);

            return;
        }
    }
}


int GetBufInfoInStateList(BufStateList listnum, Buf* ppBufInfo[], int numBuf)
{
    Buf* tmp;

    int bufferContainedCounter = 0;

    CIRCLEQ_FOREACH(tmp, &stateList[listnum], slist){
        if(numBuf > bufferContainedCounter){
            ppBufInfo[bufferContainedCounter++] = tmp;
        }
    }
    return bufferContainedCounter;
}

int GetBufInfoInLruList(Buf* ppBufInfo[], int numBuf)
{
    Buf* tmp;

    int bufferContainedCounter = 0;

    CIRCLEQ_FOREACH(tmp, &lruListHead, llist){
        if(numBuf > bufferContainedCounter){
            ppBufInfo[bufferContainedCounter++] = tmp;
        }
    }
    return bufferContainedCounter;
}



int GetBufInfoInBufferList(int index, Buf* ppBufInfo[], int numBuf)
{
    Buf* tmp;

    int bufferContainedCounter = 0;

    CIRCLEQ_FOREACH(tmp, &bufList[index], blist){
        if(numBuf > bufferContainedCounter){
            ppBufInfo[bufferContainedCounter++] = tmp;
        }
    }
    return bufferContainedCounter;
}