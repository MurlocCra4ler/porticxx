#include <iostream>

std::streambuf my_buf;

namespace std {

//istream cin;
ostream cout(&my_buf);
//ostream cerr;
//ostream clog;
    
//wistream wcin;
//wostream wcout;
//wostream wcerr;
//wostream wclog;

}
