
#include "dmdump.h"

int main( int argc, char* argv[] ) {
    std::string strResult = exec("ps -aux");
    std::cout << strResult;
    return 0;
}
