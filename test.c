#include <stdio.h>
#include <stdlib.h>
#include "buf.h"
#include "disk.h"


void clean(void) {
    // 메모리 할당 해제 코드 작성
}


void printLRUList() {
    int i, len;
    Buf* ppBufInfo[MAX_BUF_NUM];

    printf("========= LRU List =========\n");
    len = GetBufInfoInLruList(ppBufInfo, MAX_BUF_NUM);

    for (i=0; i<len; i++)
        printf("%d ", ppBufInfo[i]->blkno);
    printf("\n");
    for (i=0; i<len; i++)
        printf("%u ", ((char *)ppBufInfo[i]->pMem)[0]);
    printf("\n");
}

void printStateList() {
    int i, len;
    Buf* ppBufInfo[MAX_BUF_NUM];

    printf("========= Clean List =========\n");
    len = GetBufInfoInStateList(BUF_CLEAN_LIST, ppBufInfo, MAX_BUF_NUM);

    for (i=0; i<len; i++)
        printf("%d ", ppBufInfo[i]->blkno);
    printf("\n");
    for (i=0; i<len; i++)
        printf("%u ", ((char *)ppBufInfo[i]->pMem)[0]);
    printf("\n");

    printf("========= Dirty List =========\n");
    len = GetBufInfoInStateList(BUF_DIRTY_LIST, ppBufInfo, MAX_BUF_NUM);

    for (i=0; i<len; i++)
        printf("%d ", ppBufInfo[i]->blkno);
    printf("\n");
    for (i=0; i<len; i++)
        printf("%u ", ((char *)ppBufInfo[i]->pMem)[0]);
    printf("\n");
}

void printBufList() {
    int i, idx, len;
    Buf* ppBufInfo[MAX_BUF_NUM];

    printf("========= Buf List =========\n");
    for (idx=0; idx<MAX_BUFLIST_NUM; idx++) {
        printf("+++ List %d +++\n", idx);
        len = GetBufInfoInBufferList(idx, ppBufInfo, MAX_BUF_NUM);

        for (i=0; i<len; i++)
            printf("%d ", ppBufInfo[i]->blkno);
        printf("\n");
        for (i=0; i<len; i++)
            printf("%u ", ((char *)ppBufInfo[i]->pMem)[0]);
        printf("\n");
    }
}


// test read/write and buffer replacement
void test1(void) {
    int i;
    char pData[BLOCK_SIZE] = {0, };

    for (i=0; i<MAX_BUF_NUM+(MAX_BUF_NUM/2); i++) {
        pData[0] = (char) i;
        
        BufWrite(i, pData);
        printf("Write #%d: %u\n", i, pData[0]);
    }
    printf("\n");

    printLRUList();
    printStateList();
    printBufList();

    printf("\n");
    for (i=0; i<MAX_BUF_NUM/2; i++) {
        BufRead(i, pData);
        printf("Read #%d: %u\n", i, ((char *)pData)[0]);
    }
    printf("\n");

    printLRUList();
    printStateList();
    printBufList();
}

// test sync
void test2(void) {
    int i;
    char pData[BLOCK_SIZE] = {0, };

    for (i=0; i<MAX_BUF_NUM/2; i++) {
        pData[0] = (char) i;
        
        BufWrite(i, pData);
        printf("Write #%d: %u\n", i, pData[0]);
    }
    printf("\n");

    printLRUList();
    printStateList();
    printBufList();

    BufSync();
    printf("\n***** BufSync() called *****\n\n");

    printLRUList();
    printStateList();
    printBufList();

    printf("\n");

    for (i=0; i<MAX_BUF_NUM/2; i++) {
        DevReadBlock(i, pData);
        printf("Read (from disk) #%d: %u\n", i, pData[0]);
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    int testNum;

    if (argc != 2) return -1;
    testNum = atoi(argv[1]);

    DevCreateDisk();
    BufInit();

    switch (testNum) {
    case 1:
        test1();
        break;
    case 2:
        test2();
        break;
    default:
        break;
    }

    DevCloseDisk();
    clean();

    return 0;
}
