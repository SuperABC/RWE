#ifndef RWE_SGS_INTERPRETER_LEXEME
#define RWE_SGS_INTERPRETER_LEXEME
#include "Text/editor/lang.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

#define _CRT_SECURE_NO_WARNINGS

using std::vector;
using std::string;

enum SGSTOKENTYPE{
	SGS_TT_USER,
	SGS_TT_DATA,
	SGS_TT_OP,
	SGS_TT_SYS,
};
enum SGSCONSTTYPE {
	CT_INT,
	CT_FLOAT,
	CT_BOOL,
	CT_CHAR,
	CT_STRING
};

enum SGSIDENTIFIER {
	SGS_ID_INTEGER,
	SGS_ID_CHAR,
	SGS_ID_STRING,
	SGS_ID_FLOAT,
	SGS_ID_BOOL,
	SGS_ID_ARRAY,
	SGS_ID_CLASS,
	SGS_ID_FUNCTION,
	SGS_ID_NONE,//locked above.

	SGS_ID_LET,
	SGS_ID_BE,
	SGS_ID_NEW,
	SGS_ID_WITH,
	SGS_ID_START,
	SGS_ID_END,
	SGS_ID_IF,
	SGS_ID_THEN,
	SGS_ID_ELSE,
	SGS_ID_LOOP,
	SGS_ID_WHEN,
	SGS_ID_OF,
	SGS_ID_IS,
	SGS_ID_ARE,
	SGS_ID_SMALLER,
	SGS_ID_LARGER,
	SGS_ID_THAN,
	SGS_ID_USE,
	SGS_ID_LIBRARY,
	SGS_ID_RESULT,
	SGS_ID_QUIT,
	SGS_ID_AND,
	SGS_ID_OR,
	SGS_ID_NOT,
	SGS_ID_XOR,
	SGS_ID_BREAK,
	SGS_ID_REDO,
	SGS_ID_FALSE,
	SGS_ID_TRUE,
	SGS_ID_COMMENT
};
enum SGSOPERATOR {
	SGS_OP_PLUS,
	SGS_OP_PLUSPLUS,
	SGS_OP_EQPLUS,
	SGS_OP_MINUS,
	SGS_OP_MINUSMINUS,
	SGS_OP_EQMINUS,
	SGS_OP_NEG,
	SGS_OP_MULTY,
	SGS_OP_EQMULTY,
	SGS_OP_DIVIDE,
	SGS_OP_EQDIVIDE,
	SGS_OP_MOD,
	SGS_OP_EQMOD,
	SGS_OP_AND,
	SGS_OP_ANDAND,
	SGS_OP_EQAND,
	SGS_OP_OR,
	SGS_OP_OROR,
	SGS_OP_EQOR,
	SGS_OP_NOR,
	SGS_OP_EQNOR,
	SGS_OP_INVERSE,
	SGS_OP_EQINVERSE,
	SGS_OP_LBRACE,
	SGS_OP_RBRACE,
	SGS_OP_LPARENTHESIS,
	SGS_OP_RPARENTHESIS,
	SGS_OP_LBRAKET,
	SGS_OP_RBRAKET,
	SGS_OP_SEMI,
	SGS_OP_COMMA,
	SGS_OP_DOT,
	SGS_OP_SMALLER,
	SGS_OP_NSMALLER,
	SGS_OP_GREATER,
	SGS_OP_NGREATER,
	SGS_OP_NOT,
	SGS_OP_NOTEQ,
	SGS_OP_EQUAL,
	SGS_OP_QUERY,
	SGS_OP_QUOT,
	SGS_OP_DBQUOT,
	SGS_OP_CROSS
};

typedef struct _sgsHn {
	enum SGSTOKENTYPE type;
	std::string name;
	int id;
	struct _sgsHn *next;
}sgsHashNode;
typedef struct {
	enum SGSTOKENTYPE type;
	int id;
	float value;
	char *s = NULL;

	int line;
	int start, end;
}sgsTokenPrim;

class SgsLex : public Language {
private:
	string content;
	int idNum;

	sgsHashNode *list[256];

	int negPos = 0;
public:
	vector<sgsTokenPrim> output;
	vector<string> strId;

	SgsLex(const char *input = NULL);
	virtual ~SgsLex();

	int preserve(const char *str);
	void prepare();
	int hash(string s);

	SgsLex *input(const char *str);
	std::vector<sgsTokenPrim>parse();
	std::vector<std::pair<int, std::pair<int, int>>> highlight();
	std::vector<std::string> complete(std::string tab);

	const char *get();

	static void error(const char *word, int type);
};

enum SGSLEXEMEERROR {
	SGS_LE_ILLEGAL,
	SGS_LE_TOOLONG,
	SGS_LE_INCOMPLETE,
	SGS_LE_EXPOP
};
class SGSLexemeException {
private:
	std::string msg;
public:
	SGSLexemeException(std::string s) {
		msg = s;
	}
	const char *message() { return msg.data(); }
};
#endif