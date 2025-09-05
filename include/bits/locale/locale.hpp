#pragma once

#include <bits/string/basic_string.hpp>

namespace std {

class locale {
public:
    // types:
    class facet {};
    class id;
    typedef int category;
    static const category // values assigned here are for exposition only
        none = 0,
        collate = 0x010, ctype = 0x020, monetary = 0x040, numeric = 0x080,
        time = 0x100, messages = 0x200,
        all = collate | ctype | monetary | numeric | time | messages;

    // construct/copy/destroy:
    locale() noexcept = default;
    locale(const locale& other) noexcept = default;
    explicit locale(const char* std_name);
    explicit locale(const string& std_name);
    locale(const locale& other, const char* std_name, category);
    locale(const locale& other, const string& std_name, category);
    template <class Facet> locale(const locale& other, Facet* f);
    locale(const locale& other, const locale& one, category);
    ~locale() = default;

    // not virtual
    const locale& operator=(const locale& other) noexcept;
    template <class Facet> locale combine(const locale& other) const;

    // locale operations:
    basic_string<char> name() const;
    bool operator==(const locale& other) const;
    bool operator!=(const locale& other) const;
    template <class CharT, class Traits, class Allocator>
    bool operator()(const basic_string<CharT, Traits, Allocator>& s1,
                    const basic_string<CharT, Traits, Allocator>& s2) const;

    // global locale objects:
    static locale global(const locale&);
    static const locale& classic();
};

} // namespace std