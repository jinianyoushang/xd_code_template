#include <chrono>
#include <iostream>


using namespace std;

int main() {
    // 禁用std::cout和C的stdio之间的同步
    std::ios_base::sync_with_stdio(false);

    auto begin=std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) {
            std::cout << "Hello, World!" << std::endl;
    }
    auto end=std::chrono::high_resolution_clock::now();

    std::cout << "time:" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0 << " s "
            << std::endl;

    getchar();
    return 0;
}
