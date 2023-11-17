#include <iostream>
using namespace std;


int main() {
    int a=0x12345678;
    char *p=(char*)&a;
    printf("%x",*p);// 78小端
    return 0;
}
