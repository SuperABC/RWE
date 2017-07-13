#ifndef RWE_COMPILER_INTERPRETOR_SYNTAX
#define RWE_COMPILER_INTERPRETOR_SYNTAX
#include "Frame/main/main.h"
#include "lexeme.h"

typedef struct _on {
	enum OPTYPE type;
	int opid;
	struct _on *left, *right;
	_on() { left = right = NULL; }
	~_on() { if (left)delete left; if (right)delete right; }
}opNode;

class Syntax {
private:
	vector<tokenPrim> content;
	vector<char *> strId;
	enum QUERYSTEP step;
	int proc;
public:
	Syntax(std::vector<char *>ids, std::vector<tokenPrim>input);
	~Syntax();

	static char *opStr(int id);
	static char *valueStr(float value);

	opNode *parse();
	opNode *expression(int *proc, opNode *root);
	static void error(const char *inst, int type);
};

class SyntaxException {
private:
	std::string msg;
public:
	SyntaxException(std::string s) {
		msg = s;
	}
	const char *message() { return msg.c_str(); }
};

#endif