
#include "dmdump.h"
#include "dmflags.h"

#include <iostream>

DEFINE_uint64(AGE, 23, "age");
DEFINE_string(NAME, "andy", "name");

// dmflagstest --AGE=36 --NAME=tom
int main(int argc, char **argv) 
{   
	DMFLAGS_INIT(argc, argv);
    std::string strResult = exec("ps -aux");
	std::cout << "AGE = " << FLAGS_AGE << std::endl;
	std::cout << "NAME = " << FLAGS_NAME << std::endl;
	return 0;   
}