#ifndef RWE_COMPILER_INTERPRETOR_LEXEME
#define RWE_COMPILER_INTERPRETOR_LEXEME
#include "Frame/main/main.h"

enum TOKENTYPE {
	TT_USER,
	TT_DATA,
	TT_OP,
	TT_SYS,
};

enum LEXEMEERROR {
	LE_UNDEFINED,
	LE_TOOLONG,
	LE_INCOMPLETE
};

typedef struct _hn {
	enum TOKENTYPE type;
	string name;
	int id;
	struct _hn *next;
}hashNode;
typedef struct _tp {
	enum TOKENTYPE type;
	int id;
	float value;
	char *s;
}tokenPrim;

class Lexeme {
private:
	std::string content;
	int idNum;
	hashNode *list[256];

	std::vector<tokenPrim>output;
public:
	std::vector<char *>strId;

	Lexeme(const unsigned char *input = NULL) {};
	~Lexeme() {};

	void prepare();
	std::vector<tokenPrim>parse();
	int preserve(char *str);
	int hash(char *s);
	static void error(const char *word, int type);
};

class LexemeException {
private:
	string msg;
public:
	LexemeException(std::string s) {
		msg = s;
	}
	const char *message() { return msg.c_str(); }
};
#endif