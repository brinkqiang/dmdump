
#include "dmdump.h"
#include "dmflags.h"

#include <iostream>
#include "dmstrtk.hpp"

DEFINE_string(n, "zsh", "name");

// dmdump -n zsh
int main(int argc, char **argv) 
{   
    DMFLAGS_INIT(argc, argv);
    DMGenDumpFile(FLAGS_n);
    return 0;
}
