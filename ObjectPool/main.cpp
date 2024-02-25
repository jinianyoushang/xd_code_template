#include <iostream>
#include"ObjectPool.h"

using namespace std;


class Test {
public:
    void print() {
        cout << "hello" << endl;
    }
};

int main() {
    ObjectPool<Test> pool;

    auto temp1 = pool.getObject();
    auto temp2 = pool.getObject();
    auto temp3 = pool.getObject();
    if (temp1 == nullptr) {
        printf("error !\n");
    }
    temp1->print();


    temp1.reset();
    auto temp4 = pool.getObject();

    return 0;
}
