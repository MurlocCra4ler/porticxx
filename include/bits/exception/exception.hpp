#pragma once

namespace std {

class exception {
public:
    virtual ~exception() = default;

    virtual const char* what() const noexcept;
};

} // namespace std