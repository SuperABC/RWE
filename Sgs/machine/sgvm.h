#ifndef RWE_COMPILER_MACHINE_SGVM
#define RWE_COMPILER_MACHINE_SGVM
#include "Frame/main/main.h"
#include "Sgs/interpreter/lexeme.h"
#include "Sgs/interpreter/syntax.h"
#include "bitmap.h"

class Sgvm {
private:
	Lexeme parser;
	Syntax processor;

	Screen screen;
public:
	Sgvm() {};
	void execute(string input);
};

#endif