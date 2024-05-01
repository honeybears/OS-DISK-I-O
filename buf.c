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

}

Buf* BufFind(int blkno)
{

}


void BufRead(int blkno, char* pData)
{

}


void BufWrite(int blkno, char* pData)
{

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

