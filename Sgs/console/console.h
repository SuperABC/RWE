#ifndef RWE_SGS_MACHINE_CONSOLE
#define RWE_SGS_MACHINE_CONSOLE
#include "Frame/main/main.h"
#include "Compiler/text/editor.h"
#include "Sgs/machine/sgvm.h"

class Console {
private:
	Editor editor;
	Sgvm vm;
public:
	Console() {};
	~Console() {};
};

#endif