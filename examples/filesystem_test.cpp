#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    fs::path cwd = fs::current_path();
    std::cout << "Current working directory: " << cwd << std::endl;

    std::string path = "."; // aktuelles Verzeichnis
    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << "path: " << entry.path()
                  << ", filename: " << entry.path().filename() << std::endl;
    }

    fs::current_path("..");
}
