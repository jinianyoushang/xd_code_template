#include <iostream>

using namespace std;


class Singleton {
private:
    static Singleton singleton;
    Singleton() = default;

    Singleton(const Singleton &) = delete;

    Singleton(const Singleton &&) = delete;

    Singleton &operator=(Singleton &) = delete;

    Singleton &operator=(Singleton &&) = delete;

public:
    static Singleton &getInstance() {
        return singleton;
    }

    void printMsg(){
        cout<<"hello"<<endl;
    }

};

Singleton Singleton::singleton;

int main() {
    auto &temp = Singleton::getInstance();
    temp.printMsg();

    return 0;
}

