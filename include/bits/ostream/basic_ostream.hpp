#pragma once

#include <bits/ios/basic_ios.hpp>
#include <bits/iterator/ostreambuf_iterator.hpp>
#include <bits/locale/num_put.hpp>

namespace std {

template <class CharT, class Traits = char_traits<CharT>>
class basic_ostream : virtual public basic_ios<CharT, Traits> {
public:
    using char_type = CharT;
    using int_type = typename Traits::int_type;
    using pos_type = typename Traits::pos_type;
    using off_type = typename Traits::off_type;
    using traits_type = Traits;

    // prefix/suffix
    class sentry {
    public:
        explicit sentry(basic_ostream& os) : os_(os), ok_(false) {
            if (auto* tie = os.tie()) {
                tie->flush();
            }
            ok_ = true;
        }
        ~sentry() {
            if ((os_.flags() & ios_base::unitbuf) && !os_.bad()) {
                if (!os_.flush().ok()) {
                    os_.setstate(ios_base::badbit);
                }
            }
        }
        explicit operator bool() const { return ok_; }

        sentry(const sentry&) = delete;
        sentry& operator=(const sentry&) = delete;

    private:
        basic_ostream& os_;
        bool ok_;
    };

    // constructor/destructor
    explicit basic_ostream(basic_streambuf<char_type, Traits>* sb) {
        this->init(sb);
    }
    virtual ~basic_ostream();

    // formatted output
    basic_ostream& operator<<(basic_ostream& (*pf)(basic_ostream&));
    basic_ostream&
    operator<<(basic_ios<CharT, Traits>& (*pf)(basic_ios<CharT, Traits>&));
    basic_ostream& operator<<(ios_base& (*pf)(ios_base&));

    basic_ostream& operator<<(bool value) { return insert_impl(value); }
    basic_ostream& operator<<(short n);
    basic_ostream& operator<<(unsigned short n);
    basic_ostream& operator<<(int value) { return insert_impl<long>(value); }
    basic_ostream& operator<<(unsigned int n);
    basic_ostream& operator<<(long n);
    basic_ostream& operator<<(unsigned long value) {
        return insert_impl(value);
    }
    basic_ostream& operator<<(long long n);
    basic_ostream& operator<<(unsigned long long n);
    basic_ostream& operator<<(float f);
    basic_ostream& operator<<(double f);
    basic_ostream& operator<<(long double f);

    basic_ostream& operator<<(const void* p);
    basic_ostream& operator<<(const volatile void* p);
    basic_ostream& operator<<(nullptr_t);
    basic_ostream& operator<<(basic_streambuf<char_type, Traits>* sb);

    // unformatted output
    result_ref<basic_ostream> put(char_type c);
    result_ref<basic_ostream> write(const char_type* s, streamsize n);

    result_ref<basic_ostream> flush();

private:
    template <class T> basic_ostream& insert_impl(T val) {
        using numput_type = num_put<CharT, ostreambuf_iterator<CharT, Traits>>;

        sentry s(*this);
        if (!s)
            return *this;

        const numput_type& np = use_facet<numput_type>(this->getloc());
        ostreambuf_iterator<CharT, Traits> it(*this);

        if (np.put(it, *this, this->fill(), val).failed()) {
            this->setstate(std::ios_base::badbit);
        }

        return *this;
    }
};

// unformatted output
template <class CharT, class Traits>
result_ref<basic_ostream<CharT, Traits>>
basic_ostream<CharT, Traits>::put(char_type c) {
    if (this->rdbuf()->sputc(c) == Traits::eof()) {
        return this->setstate(ios_base::badbit).map_to_ref(*this);
    }
    return *this;
}

template <class CharT, class Traits>
result_ref<basic_ostream<CharT, Traits>> basic_ostream<CharT, Traits>::flush() {
    if (this->rdbuf()->pubsync() == -1) {
        return this->setstate(ios_base::badbit).map_to_ref(*this);
    }
    return *this;
}

template <class Traits>
basic_ostream<char, Traits>& operator<<(basic_ostream<char, Traits>& os,
                                        char ch) {
    return os.write(&ch, 1);
}

template <class Traits>
basic_ostream<char, Traits>& operator<<(basic_ostream<char, Traits>& os,
                                        const char* s) {
    if (!s) {
        return os << "(null)";
    }

    return os.write(s, Traits::length(s));
}

} // namespace std