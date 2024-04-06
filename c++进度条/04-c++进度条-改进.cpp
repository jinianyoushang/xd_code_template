#include <iostream>
#include <thread> // 用于 std::this_thread::sleep_for
#include <chrono> // 用于 std::chrono::seconds
#include <string> // 用于 std::string

// 函数用于显示进度条和额外的消息
void show_progress_bar(int progress, int total, const std::string& message = std::string()) {
    int width = 50; // 进度条的宽度
    float progress_level = (float)progress / total;
    int pos = width * progress_level;

    std::cout << "[";
    for (int i = 0; i < width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress_level * 100.0) << " % - "<<progress<<"/"<<total<<" " << message << "\r";
    std::cout.flush();
}

int main() {
    int total_steps = 100;

    for (int step = 0; step <= total_steps; ++step) {
        // 使用额外的消息调用show_progress_bar函数
        show_progress_bar(step, total_steps, "processing...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟任务进度
    }

    std::cout << std::endl << "finish!" << std::endl;

    return 0;
}
