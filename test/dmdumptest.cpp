
#include "dmdump.h"
#include "dmflags.h"

#include <iostream>

DEFINE_string(NAME, "httpserver", "n");

// dmdumptest --NAME=httpserver
int main(int argc, char **argv) 
{   
	DMFLAGS_INIT(argc, argv);
    DMGenDumpFile(FLAGS_NAME);
	std::cout << "NAME = " << FLAGS_NAME << std::endl;
	return 0;   
}