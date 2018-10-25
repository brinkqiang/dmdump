
#include "dmdump.h"
#include "dmflags.h"

#include <iostream>

DEFINE_string(n, "zsh", "name");

// dmdumptest --NAME=httpserver
int main(int argc, char **argv) 
{   
	DMFLAGS_INIT(argc, argv);
    DMGenDumpFile(FLAGS_n);
	std::cout << "name = " << FLAGS_n << std::endl;
	return 0;   
}