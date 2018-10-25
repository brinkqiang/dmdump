
#include "dmdump.h"
#include "dmformat.h"

int main( int argc, char* argv[] ) {
    std::cout << fmt::format("{0} {1}", "hello world! ", DMDUMP_MAIN(argc, argv));
    return 0;
}
