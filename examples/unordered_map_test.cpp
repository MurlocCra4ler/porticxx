#include <cassert>
#include <string>
#include <unordered_map>

void test_unordered_map_basic() {
    std::unordered_map<std::string, int> m;

    assert(m.empty());
    assert(m.size() == 0);

    m["a"] = 1;
    m["b"] = 2;
    m["c"] = 3;
    assert(m.size() == 3);
    assert(m["a"] == 1);
    assert(m["b"] == 2);
    assert(m["c"] == 3);

    m["a"] = 42;
    assert(m["a"] == 42);
    assert(m.size() == 3);

    /*
    auto it = m.find("b");
    assert(it != m.end());
    assert(it->first == "b");
    assert(it->second == 2);

    auto it2 = m.find("z");
    assert(it2 == m.end());

    size_t erased = m.erase("b");
    assert(erased == 1);
    assert(m.find("b") == m.end());
    assert(m.size() == 2);

    auto [insIt, inserted] = m.insert({"d", 99});
    assert(inserted);
    assert(insIt->first == "d");
    assert(insIt->second == 99);

    auto [insIt2, inserted2] = m.insert({"d", 123});
    assert(!inserted2);
    assert(insIt2->second == 99);

    int sum = 0;
    for (auto &p : m) {
        sum += p.second;
    }
    assert(sum == (42 + 3 + 99)); // a=42, c=3, d=99*/
}

void test_unordered_map_bucket_interface() {
    /*
    my::unordered_map<int, int> m;

    for (int i = 0; i < 100; ++i) {
        m[i] = i * 2;
    }
    assert(m.size() == 100);

    // buckets sollten >= size sein
    assert(m.bucket_count() >= m.size());

    // ein key muss im richtigen bucket liegen
    int key = 42;
    auto b = m.bucket(key);
    bool found = false;
    for (auto it = m.begin(b); it != m.end(b); ++it) {
        if (it->first == key) {
            found = true;
            break;
        }
    }
    assert(found);*/
}

int main() {
    test_unordered_map_basic();
    test_unordered_map_bucket_interface();
    return 0;
}