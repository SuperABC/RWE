#ifndef RWE_SGS_INTERPRETER_SGSDEBUG
#define RWE_SGS_INTERPRETER_SGSDEBUG

#include "Sgs/interpreter/sgsmach.h"

void sgsDebug();

class SgsDebug : public SgsMachine {
public:
	void execute(int step);
	varNode *getVar(string name);
};

#endif