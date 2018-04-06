#ifndef RWE_SGS_INTERPRETER_SYNTAX
#define RWE_SGS_INTERPRETER_SYNTAX
#include "Sgs/interpreter/sgslex.h"
#include "Sgs/interpreter/sgsmem.h"
#include <stack>

struct varNode;
struct classType;
struct classNode;
struct funcType;
struct funcNode;
struct stateSeq;

enum valOp {
	VO_NULL,
	VO_ASSIGN,
	VO_EXE,
	VO_IF,
	VO_WHILE
};
struct rtVal {
	enum valOp op;
	varNode *left, *right;
};
struct stateSeq {
	rtVal act;
	stateSeq *next;
};

enum varType {
	_VT_INTEGER,
	_VT_CHAR,
	_VT_STRING,
	_VT_FLOAT,
	_VT_BOOL,
	_VT_ARRAY,
	_VT_CLASS,
	_VT_FUNCTION,
	_VT_VAR,
	_VT_EXP,
	_VT_NULL
};
struct varNode {
	enum varType t;
	string name;
	string classType;
	void const *val;
	varNode *next;//Used for function parameter and arraies.
	varNode *left, *right;//Used for expression.
	stateSeq *block;//Used for flow and function.
	varNode() : t(_VT_NULL), name(""), classType(""),
		val(NULL), next(NULL), left(NULL), right(NULL), block(NULL) {};
	varNode(int type, string n, string str = "") : t((enum varType)type), name(n), classType(str),
		val(NULL), next(NULL), left(NULL), right(NULL), block(NULL) {};
};
struct classType {
	string name;
	vector<varNode> varList;
};
struct classNode {
	classType type;
	string name;
	vector<varNode> varList;
	classNode() {}
	classNode(classType t, string name) {
		this->type = t;
		this->name = name;
	}
};
struct funcType {
	string name;
	vector<varNode> varList;
	funcType() {}
	funcType(string name,
		vector<varNode> vars = vector<varNode>()) {
		this->name = name;
		varList = vars;
	}
};
struct funcNode {
	funcType declare;
	stateSeq content;
	vector<varNode> localVar;
	funcNode() {}
	funcNode(funcType dec) { declare = dec; }
};

class SgsSyntax {
private:
	vector<sgsTokenPrim> content;
	vector<string> strId;

	SgsMemory synMem;

	enum synState {
		SS_NULL,
		SS_FUNC,
		SS_IF,
		SS_WHILE
	};
	synState state = SS_NULL;

	unsigned int proc;
	int func = -1;
public:
	vector<classType> globeClassType;
	vector<funcNode> globeFunc;
	vector<varNode> globeVar;
	stateSeq *output;
	stateSeq *last;

	SgsSyntax();
	SgsSyntax(vector<string> &ids, vector<sgsTokenPrim> &input);
	~SgsSyntax();
	void prepare();

	static const char *opStr(int id);

	SgsSyntax *input(vector<string> &ids, vector<sgsTokenPrim> &src);
	stateSeq *parse();

	void parseLibrary(string lib);
	string parseUser();
	varNode *parseExpression();
	funcNode parseFuncDec();
	stateSeq parseFuncDef(int funcid);
	varNode *parseParameter(int funcid);
	stateSeq *parseBlock();
	classType parseClassDec();
	classNode parseClassConstr();

	string findVar();
	int findClass();
	int findFunc();
	int findType();

	void clearMem();

	static bool compare(int op1, int op2);
	static void error(const char *inst, int type);
};

enum SGSYNTAXERROR {
	SGS_SE_EXPOSE,
	SGS_SE_UNIQUE,
	SGS_SE_EXPDOT,
	SGS_SE_EXPCOMMA,
	SGS_SE_EXPBRACE,
	SGS_SE_REDEF,
	SGS_SE_INVALIDTYPE,
	SGS_SE_DISACCORD,
	SGS_SE_NOID,
	SGS_SE_INCOMPLETE,
	SGS_SE_UNKNOWN,
	SGS_SE_UNSUPPORT
};
class SGSyntaxException {
private:
	std::string msg;
public:
	SGSyntaxException(std::string s) {
		msg = s;
	}
	const char *message() { return msg.data(); }
};
#endif