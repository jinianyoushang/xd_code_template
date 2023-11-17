#include <iostream>

using namespace std;


class Singleton {
private:
    Singleton() = default;

    Singleton(const Singleton &) = delete;

    Singleton(const Singleton &&) = delete;

    Singleton &operator=(Singleton &) = delete;

    Singleton &operator=(Singleton &&) = delete;

public:
    static Singleton &getInstance() {
        static Singleton singleton;
        return singleton;
    }
};


int main() {
    auto &temp = Singleton::getInstance();


    return 0;
}

