#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
    std::vector<std::thread> threads;
    std::mutex mtx;

    for (int i = 0; i < 8; ++i) {
        threads.push_back(std::thread([&, i] -> void {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Hello from Thread " << i << "." << std::endl;
        }));
    }

    for (int i = 0; i < 8; ++i) {
        threads[i].join();
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Thread " << i << " exited." << std::endl;
    }
}
