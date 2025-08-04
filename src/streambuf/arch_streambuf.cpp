#pragma once

#include <streambuf>

class arch_streambuf : public std::streambuf {
public:
    static arch_streambuf& get_cin_buf();
    static arch_streambuf& get_cout_buf();
};
