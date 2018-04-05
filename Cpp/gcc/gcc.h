#ifndef RWE_CPP_GCC_GCC
#define RWE_CPP_GCC_GCC

#include "Frame/main/main.h"
#include "Cpp/gcc/pipe.h"

class Gcc {
private:
	string compiler;

	Pipe pipe;
public:
	Gcc(string path = "") { compiler = path; }
	string compile(string src);
	void run(string exe);
};


#endif

