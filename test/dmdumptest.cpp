
#include "dmdump.h"
#include <iostream>

int main( int argc, char* argv[] ) {
    std::string strResult = exec("ps -aux");
    std::cout << strResult;
    return 0;
}
