#ifndef RWE_SGS_INTERPRETER_MACHINE
#define RWE_SGS_INTERPRETER_MACHINE
#include "Sgs/interpreter/sgsyntax.h"
#include "Window/scene/bitmap.h"

void sgsInterprete();

class SgsMachine {
private:
	stateSeq *content;

	vector<varNode> globeVar;
	vector<classType> globeClassType;
	vector<funcNode> globeFunc;

	SgsMemory macMem;

	int func = -1;

	Canvas canvas;
public:
	SgsMachine();
	~SgsMachine();

	SgsMachine *input(stateSeq *seq,
		vector<classType> &classType, vector<funcNode> &func,
		vector<varNode> &var);
	void execute();
	void execute(stateSeq *s, varNode *par, int funcid = -1);
	void step(stateSeq *s);
	varNode exp(varNode *e);
	void setup();
	void loop();

	void print(varNode *par);
	void out(varNode *par);
	void outln(varNode *par);
	varNode in(varNode *par);
	varNode time(varNode *par);
	varNode random(varNode *par);

	void initWindow(varNode *par);
	void setColor(varNode *par);
	void clearScreen(varNode *par);
	void putLine(varNode *par);
	void putQuad(varNode *par);
	void putTriangle(varNode *par);
	void putCircle(varNode *par);
	void putEllipse(varNode *par);

	void clearMem();
	static void error(const char *inst, int type);
};

enum VMERROR {
	VE_DIVBYZERO,
	VE_TYPEMISMATCH,
	VE_BROKEN
};
class MachineException {
private:
	std::string msg;
public:
	MachineException(std::string s) {
		msg = s;
	}
	const char *message() { return msg.data(); }
};
#endif