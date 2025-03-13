//#include <svdpi.h>

#include <iostream>

extern "C" void welcome_cpp() {
    std::cout << "Hello World from C++\n";

    return;
}
