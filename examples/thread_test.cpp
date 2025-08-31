#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
    std::vector<std::thread> threads;
    std::mutex mtx;

    for (int i = 0; i < 4; ++i) {
        threads.push_back(std::thread([&] -> void {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Hello world :)" << std::endl;
        }));
    }

    for (int i = 0; i < 4; ++i) {
        threads[i].join();
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Thread exited;" << std::endl;
    }
}
