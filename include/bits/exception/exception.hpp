#pragma once

namespace std {

class exception {
public:
    virtual const char* what() const noexcept;
};

}