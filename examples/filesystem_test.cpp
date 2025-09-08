#include <filesystem>
#include <iostream>

int main() {
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Current working directory: " << cwd << std::endl;
}
