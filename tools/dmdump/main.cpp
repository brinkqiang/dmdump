
#include "dmdump.h"
#include "dmflags.h"

#include <iostream>
#include "dmstrtk.hpp"

DEFINE_string(n, "zsh", "name");
DEFINE_bool(isWin32, false, "WIN32 / WIN64");

// dmdump -n zsh
int main(int argc, char **argv) 
{   
    DMFLAGS_INIT(argc, argv);
    DMGenDumpFile(FLAGS_n, FLAGS_isWin32);
    return 0;
}
