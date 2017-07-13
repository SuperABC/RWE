#ifndef RWE_COMPILER_MACHINE_SGVM
#define RWE_COMPILER_MACHINE_SGVM
#include "Frame/main/main.h"
#include "Compiler/interpreter/lexeme.h"
#include "Compiler/interpreter/syntax.h"
#include "bitmap.h"

class Sgvm {
private:
	Lexeme parser;
	Syntax processor;

	Screen screen;
public:
	void execute(string input);
};

#endif