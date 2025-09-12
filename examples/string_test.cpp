#include <cassert>
#include <string>

int main() {
    // default
    std::string s1;
    assert(s1.empty());

    // c-string
    std::string s2("hello");
    assert(s2.size() == 5);
    assert(s2[0] == 'h');

    // copy
    std::string s3(s2);
    assert(s3 == s2);

    // move
    std::string s4(std::move(s3)); 
    assert(s4 == s2);
    assert(s3.empty());

    // append / operator+=
    s4.append(" world");
    assert(s4 == "hello world");

    s4 += "!";
    assert(s4 == "hello world!");

    // push_back / back
    s4.push_back('?');
    assert(s4.back() == '?'); 
    assert(s4 == "hello world!?");

    // erase
    s4.erase(5, 6); // entfernt " world"
    assert(s4 == "hello!?");

    // iteration
    std::cout << "Characters in s4: ";
    for (char c : s4) std::cout << c << ' ';
    std::cout << '\n';

    // capacity / size / empty
    std::cout << "size: " << s4.size() << ", capacity: " << s4.capacity() 
              << ", empty: " << s4.empty() << "\n";

    std::cout << "All tests passed!" << std::endl;
    return 0;
}