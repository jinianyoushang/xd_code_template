#include <iostream>
#include <bits/stdc++.h>

using namespace std;


void *mYmemcpy(void *pDest, const void *pSrc, unsigned int n) {
    assert((nullptr != pDest) && (nullptr != pSrc));
//    assert(pDest >= (char *) pSrc + n || pSrc > (char *) pDest + n);//禁止内存重叠

    char *pTmpDest = (char *) pDest;
    char *pTmpSrc = (char *) pSrc;
    //重叠时候倒着复制
    if (pTmpSrc <= pTmpDest && pTmpDest <= pTmpSrc + n) {
        pTmpDest += n;
        pTmpSrc += n;
        //倒着复制
        while (n--) {
            *pTmpDest = *pTmpSrc;
            pTmpDest--;
            pTmpSrc--;
        }
    } else {//正常逻辑
        while (n--) {
            *pTmpDest = *pTmpSrc;
            pTmpDest++;
            pTmpSrc++;
        }
    }
    return pDest;
}

int main() {

    //测试
    int a = 3;
    int c;
    mYmemcpy(&c, &a, 4);
    cout << c << endl;
    return 0;
}