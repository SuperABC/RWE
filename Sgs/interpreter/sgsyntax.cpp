#define _CRT_SECURE_NO_WARNINGS
#include "Sgs/interpreter/sgsyntax.h"

SgsSyntax::SgsSyntax(){
	proc = 0;

	prepare();

	output = new stateSeq();//Safely released [0].
	last = output;
}
SgsSyntax::SgsSyntax(vector<string> &ids, vector<sgsTokenPrim> &input) {
	proc = 0;

	content = input;
	strId = ids;

	prepare();

	output = new stateSeq();//Safely released [0].
	last = output;
}
SgsSyntax::~SgsSyntax() {
	stateSeq *tmp = output;
	while (output != last) {
		tmp = output->next;
		delete output;//Releasing [0].
		output = tmp;
	}
	delete tmp;//Releasing [0].
}
void SgsSyntax::prepare() {
	funcNode out = funcNode(funcType(string("out")));
	out.declare.varList.push_back(varNode(_VT_STRING, "content"));
	globeFunc.push_back(out);

	funcNode outln = funcNode(funcType(string("outln")));
	outln.declare.varList.push_back(varNode(_VT_STRING, "content"));
	globeFunc.push_back(outln);

	funcNode in = funcNode(funcType(string("in")));
	in.localVar.push_back(varNode());
	globeFunc.push_back(in);

	funcNode time = funcNode(funcType(string("current time")));
	time.localVar.push_back(varNode());
	globeFunc.push_back(time);

	funcNode random = funcNode(funcType(string("random number")));
	random.localVar.push_back(varNode());
	globeFunc.push_back(random);

	funcNode initWindow = funcNode(funcType(string("init window")));
	initWindow.declare.varList.push_back(varNode(_VT_INTEGER, "width"));
	initWindow.declare.varList.push_back(varNode(_VT_INTEGER, "height"));
	initWindow.declare.varList.push_back(varNode(_VT_STRING, "title"));
	globeFunc.push_back(initWindow);

	funcNode setColor = funcNode(funcType(string("set color")));
	setColor.declare.varList.push_back(varNode(_VT_INTEGER, "red"));
	setColor.declare.varList.push_back(varNode(_VT_INTEGER, "green"));
	setColor.declare.varList.push_back(varNode(_VT_INTEGER, "blue"));
	globeFunc.push_back(setColor);

	funcNode clearScreen = funcNode(funcType(string("clear screen")));
	globeFunc.push_back(clearScreen);

	funcNode putLine = funcNode(funcType(string("put line")));
	putLine.declare.varList.push_back(varNode(_VT_INTEGER, "first x"));
	putLine.declare.varList.push_back(varNode(_VT_INTEGER, "first y"));
	putLine.declare.varList.push_back(varNode(_VT_INTEGER, "second x"));
	putLine.declare.varList.push_back(varNode(_VT_INTEGER, "second y"));
	globeFunc.push_back(putLine);

	funcNode putQuad = funcNode(funcType(string("put quad")));
	putQuad.declare.varList.push_back(varNode(_VT_INTEGER, "left"));
	putQuad.declare.varList.push_back(varNode(_VT_INTEGER, "top"));
	putQuad.declare.varList.push_back(varNode(_VT_INTEGER, "right"));
	putQuad.declare.varList.push_back(varNode(_VT_INTEGER, "bottom"));
	putQuad.declare.varList.push_back(varNode(_VT_BOOL, "filling"));
	globeFunc.push_back(putQuad);
}

const char *SgsSyntax::opStr(int id) {
	switch (id) {
	case SGS_OP_PLUS: return "+";
	case SGS_OP_PLUSPLUS: return "++";
	case SGS_OP_EQPLUS: return "+=";
	case SGS_OP_MINUS: return "-";
	case SGS_OP_MINUSMINUS: return "--";
	case SGS_OP_EQMINUS: return "-=";
	case SGS_OP_MULTY: return "*";
	case SGS_OP_EQMULTY: return "*=";
	case SGS_OP_DIVIDE: return "/";
	case SGS_OP_EQDIVIDE: return "/=";
	case SGS_OP_AND: return "&";
	case SGS_OP_ANDAND: return "&&";
	case SGS_OP_EQAND: return "&=";
	case SGS_OP_OR: return "|";
	case SGS_OP_OROR: return "||";
	case SGS_OP_EQOR: return "|=";
	case SGS_OP_NOR: return "^";
	case SGS_OP_EQNOR: return "^=";
	case SGS_OP_INVERSE: return "~";
	case SGS_OP_EQINVERSE: return "~=";
	case SGS_OP_LBRACE: return "(";
	case SGS_OP_RBRACE: return ")";
	case SGS_OP_LPARENTHESIS: return "[";
	case SGS_OP_RPARENTHESIS: return "]";
	case SGS_OP_LBRAKET: return "{";
	case SGS_OP_RBRAKET: return "}";
	case SGS_OP_SEMI: return ";";
	case SGS_OP_COMMA: return ",";
	case SGS_OP_DOT: return ".";
	case SGS_OP_SMALLER: return "<";
	case SGS_OP_NSMALLER: return ">=";
	case SGS_OP_GREATER: return ">";
	case SGS_OP_NGREATER: return "<=";
	case SGS_OP_NOT: return "!";
	case SGS_OP_NOTEQ: return "!=";
	case SGS_OP_EQUAL: return "=";
	case SGS_OP_QUERY: return "?";
	case SGS_OP_QUOT: return "\'";
	case SGS_OP_DBQUOT: return "\"";
	default: return "";
	}
}

SgsSyntax *SgsSyntax::input(vector<string> &ids, vector<sgsTokenPrim> &src) {
	content = src;

	stateSeq *tmp = output;
	while (output != last) {
		tmp = output->next;
		delete output;//Releasing [0].
		output = tmp;
	}
	delete tmp;//Releasing [0].
	output = new stateSeq();//Safely released [0].
	last = output;
	
	for (auto i : ids) {
		unsigned int j = 0;
		for (; j < strId.size(); j++) {
			if (i == strId[j])break;
		}
		if (j == strId.size())strId.push_back(i);
	}
	return this;
}
stateSeq *SgsSyntax::parse() {
	proc = 0;

	while (proc < content.size()) {
		if (content[proc].type == SGS_TT_SYS&&content[proc].id == SGS_ID_USE) {
			proc++;
			if (content[proc].type == SGS_TT_SYS&&content[proc].id == SGS_ID_LIBRARY) {
				proc++;
				parseLibrary(strId[content[proc].id]);

				proc++;
				/*if (content[proc].type == TT_OP&&content[proc].id != SGS_OP_DOT)
					error("Use library", SE_EXPDOT);
				proc++;*/
			}
			else {
				proc++;
				error("Use", SGS_SE_UNIQUE);
			}
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS&&content[proc].id == SGS_ID_NEW) {
			proc++;
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_CLASS) {
				proc++;
				globeClassType.push_back(parseClassDec());
			}
			else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_FUNCTION) {
				proc++;
				globeFunc.push_back(parseFuncDec());
			}
			else
				error("New", SGS_SE_UNIQUE);
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS&&content[proc].id == SGS_ID_LET) {
			proc++;
			string newVar;
			int varType;
			int classIdx = -1;
			if (content[proc].type == SGS_TT_USER) {
				if ((classIdx = findClass()) >= 0) {
					varType = _VT_CLASS;
					newVar = findVar();
					if (newVar[0] != '$') {
						globeVar.push_back(varNode(varType, newVar, globeClassType[classIdx].name));
					}
					else
						error(newVar.data(), SGS_SE_REDEF);
				}
				else if ((newVar = findVar())[0] == '$') {
					newVar = newVar.data() + 1;
					if (newVar[0] >= '0' &&newVar[0] <= '9') {
						string comp = newVar;
						int offset = atoi(newVar.data());
						newVar = newVar.data() + newVar.find('@') + 1;

						for (auto n : globeVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
						newVar = comp;
					}
					else {
						for (auto n : globeVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
					}
				}
				else
					error(strId[content[proc].id].data(), SGS_SE_NOID);
			}
			else if (content[proc].type == SGS_TT_SYS) {
				if ((varType = findType()) != -1) {
					if ((newVar = findVar())[0] == '$')
						error(strId[content[proc].id].data(), SGS_SE_REDEF);
					else {
						globeVar.push_back(varNode(varType, newVar));
					}
				}
				else
					error(strId[content[proc].id].data(), SGS_SE_UNIQUE);
			}
			else
				error("Let", SGS_SE_UNIQUE);

			if (content[proc].type == SGS_TT_OP&&content[proc].id == SGS_OP_DOT)
				continue;
			else if (content[proc].type == SGS_TT_SYS&&content[proc].id == SGS_ID_BE) {
				proc++;
				rtVal st = rtVal();
				st.op = VO_ASSIGN;
				if (classIdx == -1)
					st.left = new varNode(varType, newVar);//Safely released [1].
				else
					st.left = new varNode(varType, newVar, globeClassType[classIdx].name);
				st.right = parseExpression();//Safely released [2].
				last->act = st;
				last->next = new stateSeq();//Safely released [0].
				last = last->next;
			}
			else
				error("Let(be)", SGS_SE_INCOMPLETE);
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS&&content[proc].id == SGS_ID_START) {
			proc++;
			int funcIdx;
			if ((funcIdx = findFunc()) >= 0) {
				parseFuncDef(funcIdx);
				if (funcIdx != findFunc())
					error(globeFunc[funcIdx].declare.name.data(), SGS_SE_DISACCORD);
			}
			else
				error(strId[content[proc].id].data(), SGS_SE_NOID);
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
			proc++;

			varNode *branch = parseExpression();
			if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_THEN)
				error("If(then)", SGS_SE_INCOMPLETE);
			proc++;

			rtVal st = rtVal();
			st.op = VO_IF;
			st.left = branch;
			varNode *accept = new varNode(_VT_FUNCTION, "");
			accept->block = parseBlock();
			varNode * reject = new varNode(_VT_FUNCTION, "");
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ELSE) {
				proc++;
				reject->block = parseBlock();
			}
			st.right = new varNode(_VT_FUNCTION, "");
			st.right->left = accept;
			st.right->right = reject;

			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
				proc++;
				if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF)
					proc++;
				else
					error("End if", SGS_SE_INCOMPLETE);
			}

			last->act = st;
			last->next = new stateSeq();//Safely released [0].
			last = last->next;

			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP) {
			proc++;

			varNode *cond = NULL;
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WHEN) {
				proc++;
				cond = parseExpression();
			}

			rtVal st = rtVal();
			st.op = VO_WHILE;
			st.left = cond;
			varNode *body = new varNode(_VT_FUNCTION, "");
			body->block = parseBlock();
			st.right = body;

			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
				proc++;
				if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP)
					proc++;
				else
					error("End loop", SGS_SE_INCOMPLETE);
			}

			last->act = st;
			last->next = new stateSeq();//Safely released [0].
			last = last->next;

			continue;
		}
		else if (content[proc].type == SGS_TT_USER) {
			int funcIdx;
			if ((funcIdx = findFunc()) != -1) {
				rtVal st = rtVal();

				st.op = VO_EXE;
				st.left = new varNode(_VT_FUNCTION, globeFunc[funcIdx].declare.name);
				st.right = parseParameter(funcIdx);
				last->act = st;
				last->next = new stateSeq();//Safely released [0].
				last = last->next;
			}
		}
		else if (content[proc].type == SGS_TT_OP&&content[proc].id == SGS_OP_DOT) {
			proc++;
			continue;
		}
	}

	return output;
}

void SgsSyntax::parseLibrary(string lib) {//Not finished.

}
string SgsSyntax::parseUser() {
	string tmp;
	while (proc < content.size() && content[proc].type == SGS_TT_USER) {
		tmp += strId[content[proc++].id];
		tmp += " ";
	}
	if (proc == content.size()) {
		error(tmp.data(), SGS_SE_INCOMPLETE);
	}
	tmp.pop_back();
	return tmp;
}
varNode *SgsSyntax::parseExpression() {
	std::stack<int> op;
	std::stack<varNode*> value;

	while (proc < content.size()) {
		if (content[proc].type == SGS_TT_SYS && content[proc].id != SGS_ID_RESULT &&
			content[proc].id != SGS_ID_FALSE && content[proc].id != SGS_ID_TRUE)break;
		else if (content[proc].type == SGS_TT_OP && 
			(content[proc].id == SGS_OP_DOT || content[proc].id == SGS_OP_COMMA ||
			content[proc].id == SGS_OP_LPARENTHESIS || content[proc].id == SGS_OP_RPARENTHESIS ||
			content[proc].id == SGS_OP_LBRAKET || content[proc].id == SGS_OP_RBRAKET))break;
		else if (content[proc].type == SGS_TT_USER ||
			content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RESULT) {
			int pre = proc;

			string varTmp;
			int funcIdx;
			int classIdx;

			if ((classIdx = findClass()) >= 0) {
				if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_LBRAKET) {
					proc++;
					varNode *tmp = new varNode(_VT_CLASS, "", globeClassType[classIdx].name);
					classNode *node = new classNode(globeClassType[classIdx], "");

					varNode *ele;
					unsigned int i;
					for (i = 0; i < globeClassType[classIdx].varList.size(); i++) {
						ele = parseExpression();
						node->varList.push_back(*ele);

						if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							continue;
						}
						if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_RBRAKET) {
							proc++;
							break;
						}
					}

					tmp->val = node;
					value.push(tmp);
				}
				else
					error(globeClassType[classIdx].name.data(), SGS_SE_INCOMPLETE);
			}
			else if ((varTmp = findVar())[0] == '$') {

				varNode *tmp = new varNode(_VT_VAR, "");
				string *n = new string(varTmp.data() + 1);
				tmp->val = n->data();

				value.push(tmp);
			}
			else {
				proc = pre;
				if ((funcIdx = findFunc()) != -1) {
					rtVal st = rtVal();

					st.op = VO_EXE;
					st.left = new varNode(_VT_FUNCTION, globeFunc[funcIdx].declare.name);
					st.right = parseParameter(funcIdx);

					varNode *tmp = new varNode(_VT_FUNCTION, "");
					tmp->t = _VT_FUNCTION;
					tmp->block = new stateSeq();
					tmp->block->act = st;

					value.push(tmp);
				}
			}
		}
		else if (content[proc].type == SGS_TT_DATA) {
			varNode *tmp = new varNode();
			if (content[proc].s != NULL) {
				tmp->t = _VT_STRING;
				tmp->val = malloc(sizeof(content[proc].s) + 1);
				strcpy((char *)tmp->val, content[proc].s);
			}
			else {
				switch (content[proc].id) {
				case CT_INT:
					tmp->t = _VT_INTEGER;
					tmp->val = new int((int)content[proc].value);
					break;
				case CT_FLOAT:
					tmp->t = _VT_FLOAT;
					tmp->val = new float((float)content[proc].value);
					break;
				case CT_CHAR:
					tmp->t = _VT_CHAR;
					tmp->val = new char((char)content[proc].value);
					break;
				}
			}

			value.push(tmp);
			proc++;
		}
		else if (content[proc].type == SGS_TT_SYS &&
			(content[proc].id == SGS_ID_FALSE || content[proc].id == SGS_ID_TRUE)) {
			varNode *tmp = new varNode();
			tmp->t = _VT_BOOL;
			if(content[proc].id == SGS_ID_FALSE)
				tmp->val = new bool(false);
			else if(content[proc].id == SGS_ID_TRUE)
				tmp->val = new bool(true);
			value.push(tmp);
			proc++;
		}
		else if (content[proc].type == SGS_TT_OP) {
			if (content[proc].id == SGS_OP_LBRACE) {
				op.push(SGS_OP_LBRACE);
			}
			else if (content[proc].id == SGS_OP_RBRACE) {
				while (op.top() != SGS_OP_LBRACE) {
					int o = op.top();
					op.pop();
					if (value.size() < 2)
						error("", SGS_SE_INCOMPLETE);
					varNode *a = value.top();
					value.pop();
					varNode *b = value.top();
					value.pop();

					varNode *tmp = new varNode(_VT_EXP, "");
					tmp->val = new int(o);
					tmp->left = a;
					tmp->right = b;
					value.push(tmp);
				}
				op.pop();
			}
			else {
				if (op.empty()) {
					op.push(content[proc].id);
				}
				else {
					while (!op.empty() && compare(content[proc].id, op.top())) {
						int o = op.top();
						op.pop();
						if (value.size() < 2)
							error("", SGS_SE_INCOMPLETE);
						varNode *a = value.top();
						value.pop();
						varNode *b = value.top();
						value.pop();

						varNode *tmp = new varNode(_VT_EXP, "");
						tmp->val = new int(o);
						tmp->left = a;
						tmp->right = b;
						value.push(tmp);
					}
					op.push(content[proc].id);
				}
			}
			proc++;
		}
		else break;
	}

	while (!op.empty()) {
		int o = op.top();
		op.pop();
		if (value.size() < 2)
			error("", SGS_SE_INCOMPLETE);
		varNode *a = value.top();
		value.pop();
		varNode *b = value.top();
		value.pop();

		varNode *tmp = new varNode(_VT_EXP, "");
		tmp->val = new int(o);
		tmp->left = a;
		tmp->right = b;
		value.push(tmp);
	}

	if (value.size() == 1) {
		return value.top();
	}
	else {
		varNode *tmp = NULL;
		while (!value.empty()) {
			value.top()->next = tmp;
			tmp = value.top();
			value.pop();
		}
		varNode *arr = new varNode(_VT_ARRAY, "");
		arr->val = tmp;
		return arr;
	}
}
funcNode SgsSyntax::parseFuncDec() {
	funcNode ret = funcNode();
	string tmp = parseUser();

	if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WITH) {
		proc++;
		ret.declare.name = tmp;

		int type, cla;
		while (true) {
			if ((type = findType()) != -1) {
				ret.declare.varList.push_back(varNode((varType)type, parseUser()));
			}
			else if ((cla = findClass()) != -1) {
				ret.declare.varList.push_back(varNode(_VT_CLASS, parseUser(), globeClassType[cla].name));
			}

			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT)break;
			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)proc++;
		}
	}
	else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT) {
		proc++;
		ret.declare.name = tmp;
	}
	else error("New function ", SGS_SE_INCOMPLETE);

	return ret;
}
stateSeq SgsSyntax::parseFuncDef(int funcid) {
	int preFunc = func;
	func = funcid;
	globeFunc[funcid].localVar.push_back(varNode(_VT_NULL, "result"));
	for(auto v : globeFunc[funcid].declare.varList)
		globeFunc[funcid].localVar.push_back(v);

	stateSeq ret = stateSeq();
	stateSeq *last = &ret;
	while (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_END) {
		if (content[proc].type == SGS_TT_SYS&&content[proc].id == SGS_ID_LET) {
			proc++;
			string newVar;
			int varType;
			int classIdx;
			if (content[proc].type == SGS_TT_USER) {
				string valueType;
				if (classIdx = findClass() >= 0) {
					varType = _VT_CLASS;
					newVar = findVar();
					if (newVar[0] != '$') {
						globeFunc[funcid].localVar.push_back(varNode(varType, newVar, globeClassType[classIdx].name));
					}
					else
						error(newVar.data(), SGS_SE_REDEF);
				}
				else if ((valueType = findVar())[0] == '$') {
					newVar = newVar.data() + 1;
					if (newVar[0] >= '0' &&newVar[0] <= '9') {
						string comp = newVar;
						int offset = atoi(newVar.data());
						newVar = newVar.data() + newVar.find('@') + 1;

						for (auto n : globeVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
						for (auto n : globeFunc[funcid].localVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
						newVar = comp;
					}
					else {
						for (auto n : globeVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
						for (auto n : globeFunc[funcid].localVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
					}
				}
				else
					error(strId[content[proc].id].data(), SGS_SE_NOID);
			}
			else if (content[proc].type == SGS_TT_SYS) {
				if (content[proc].id == SGS_ID_RESULT) {
					varType = _VT_NULL;
					newVar = "result";
					proc++;
				}
				else if ((varType = findType()) != -1) {
					if ((newVar = findVar())[0] == '$')
						error(strId[content[proc].id].data(), SGS_SE_REDEF);
					else {
						globeFunc[funcid].localVar.push_back(varNode(varType, newVar));
					}
				}
				else
					error(strId[content[proc].id].data(), SGS_SE_UNIQUE);
			}

			if (content[proc].type == SGS_TT_OP&&content[proc].id == SGS_OP_DOT)
				continue;
			else if (content[proc].type == SGS_TT_SYS&&content[proc].id == SGS_ID_BE) {
				proc++;
				rtVal st = rtVal();
				st.op = VO_ASSIGN;
				st.left = new varNode(varType, newVar);
				st.right = parseExpression();
				last->act = st;
				last->next = new stateSeq();
				last = last->next;
			}
			else
				error("", SGS_SE_INCOMPLETE);
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
			proc++;

			varNode *branch = parseExpression();
			if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_THEN)
				error("if", SGS_SE_INCOMPLETE);
			proc++;

			rtVal st = rtVal();
			st.op = VO_IF;
			st.left = branch;
			varNode *accept = new varNode(_VT_FUNCTION, "");
			accept->block = parseBlock();
			varNode * reject = new varNode(_VT_FUNCTION, "");
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ELSE) {
				proc++;
				reject->block = parseBlock();
			}
			st.right = new varNode(_VT_FUNCTION, "");
			st.right->left = accept;
			st.right->right = reject;

			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
				proc += 2;
			}

			last->act = st;
			last->next = new stateSeq();
			last = last->next;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP) {
			proc++;

			varNode *cond = NULL;
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WHEN) {
				proc++;
				cond = parseExpression();
			}

			rtVal st = rtVal();
			st.op = VO_WHILE;
			st.left = cond;
			varNode *body = new varNode(_VT_FUNCTION, "");
			body->block = parseBlock();
			st.right = body;

			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
				proc += 2;
			}

			last->act = st;
			last->next = new stateSeq();
			last = last->next;
		}
		else if (content[proc].type == SGS_TT_USER) {
			int funcIdx;
			rtVal st = rtVal();
			if ((funcIdx = findFunc()) != -1) {
				st.op = VO_EXE;
				st.left = new varNode(_VT_FUNCTION, globeFunc[funcIdx].declare.name);
				st.right = parseParameter(funcIdx);
				last->act = st;
				last->next = new stateSeq();
				last = last->next;
			}
		}
		else if (content[proc].type == SGS_TT_OP&&content[proc].id == SGS_OP_DOT) {
			proc++;
			continue;
		}
	}
	proc++;
	globeFunc[funcid].content = ret;

	func = preFunc;
	return ret;
}
varNode *SgsSyntax::parseParameter(int funcid){
	int parNum = globeFunc[funcid].declare.varList.size();
	varNode *rt = new varNode();
	varNode *last = rt;

	if (parNum == 0) {
		delete rt;
		return NULL;
	}
	else {
		if (content[proc].type == SGS_TT_SYS&&content[proc].id == SGS_ID_WITH) {
			proc++;
			for (unsigned int i = 0; i < globeFunc[funcid].declare.varList.size(); i++) {
				last->t = _VT_VAR;
				last->name = globeFunc[funcid].declare.varList[i].name;
				if (i == globeFunc[funcid].declare.varList.size() - 1)break;
				last->next = new varNode();
				last = last->next;
			}
			last->next = NULL;

			string tmp;
			bool accord = false;
			varNode *iter;
			while (content[proc].type != SGS_TT_OP && content[proc].id != SGS_OP_DOT) {
				tmp.clear();
				while (content[proc].type == SGS_TT_USER ||
					content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RESULT) {
					tmp += strId[content[proc++].id];
					for (auto p : globeFunc[funcid].declare.varList) {
						if (p.name == tmp)accord = true;
					}
					tmp += " ";
					if (accord == true)break;
				}
				tmp.pop_back();
				if (accord == true)accord = false;
				else error(tmp.data(), SGS_SE_NOID);

				iter = rt;
				while (iter != NULL) {
					if (iter->name == tmp) {
						varNode * val = parseExpression();
						iter->t = val->t;
						iter->left = val->left;
						iter->right = val->right;
						iter->val = val->val;
						iter->block = val->block;
						break;
					}
					iter = iter->next;
				}

				if (content[proc].type != SGS_TT_OP || content[proc].id != SGS_OP_COMMA) {
					if (content[proc].type != SGS_TT_OP || content[proc].id != SGS_OP_DOT)
						error("", SGS_SE_EXPCOMMA);
				}
				else proc++;
			}
		}
		else error((string("Start ") + globeFunc[funcid].declare.name).data(), SGS_SE_INCOMPLETE);
		return rt;
	}
}
classType SgsSyntax::parseClassDec() {
	classType ret = classType();
	string tmp = parseUser();

	if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WITH) {
		proc++;
		ret.name = tmp;

		int type, cla;
		while (true) {
			if ((type = findType()) != -1) {
				ret.varList.push_back(varNode((varType)type, parseUser()));
			}
			else if ((cla = findClass()) != -1) {
				//ret.classList.push_back(classNode(globeClassType[cla], parseUser()));
			}

			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT)break;
			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)proc++;
		}
	}
	else error("New class ", SGS_SE_INCOMPLETE);

	return ret;
}
classNode SgsSyntax::parseClassConstr() {//Not finished.
	classNode ret = classNode();
	return ret;
}
stateSeq *SgsSyntax::parseBlock() {
	stateSeq *ret = new stateSeq();
	stateSeq *last = ret;
	while ((content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_END)&&
		(content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_ELSE)) {
		if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LET) {
			proc++;
			string newVar;
			int varType;
			if (content[proc].type == SGS_TT_USER) {
				int classIdx;
				if (classIdx = findClass() >= 0) {
					varType = _VT_CLASS;
					newVar = findVar();
					if (newVar[0] != '$') {
						if(func==-1)
							globeVar.push_back(varNode(varType, newVar, globeClassType[classIdx].name));
						else
							globeFunc[func].localVar.push_back(varNode(varType, newVar, globeClassType[classIdx].name));
					}
					else
						error(newVar.data(), SGS_SE_REDEF);
				}
				else if ((newVar = findVar())[0] == '$') {
					newVar = newVar.data() + 1;

					if (newVar[0] >= '0' &&newVar[0] <= '9') {
						string comp = newVar;
						int offset = atoi(newVar.data());
						newVar = newVar.data() + newVar.find('@') + 1;

						for (auto n : globeVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
						if (func != -1) {
							for (auto n : globeFunc[func].localVar) {
								if (n.name == newVar) {
									varType = n.t;
									break;
								}
							}
						}
						newVar = comp;
					}
					else {
						for (auto n : globeVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
						if (func != -1) {
							for (auto &n : globeFunc[func].localVar) {
								if (n.name == newVar) {
									varType = n.t;
									break;
								}
							}
						}
					}
				}
				else
					error(strId[content[proc].id].data(), SGS_SE_NOID);
			}
			else if (content[proc].type == SGS_TT_SYS) {
				if (content[proc].id == SGS_ID_RESULT) {
					varType = _VT_NULL;
					newVar = "result";
					proc++;
				}
				else if ((varType = findType()) != -1) {
					if ((newVar = findVar())[0] == '$')
						error(strId[content[proc].id].data(), SGS_SE_REDEF);
					else {
						if(func==-1)
							globeVar.push_back(varNode(varType, newVar));
						else
							globeFunc[func].localVar.push_back(varNode(varType, newVar));
					}
				}
				else
					error(strId[content[proc].id].data(), SGS_SE_UNIQUE);
			}

			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT)
				continue;
			else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_BE) {
				proc++;
				rtVal st = rtVal();
				st.op = VO_ASSIGN;
				st.left = new varNode(varType, newVar);
				st.right = parseExpression();
				last->act = st;
				last->next = new stateSeq();
				last = last->next;
			}
			else
				error("", SGS_SE_INCOMPLETE);
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
			proc++;

			varNode *branch = parseExpression();
			if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_THEN)
				error("if", SGS_SE_INCOMPLETE);
			proc++;

			rtVal st = rtVal();
			st.op = VO_IF;
			st.left = branch;
			varNode *accept = new varNode(_VT_FUNCTION, "");
			accept->block = parseBlock();
			varNode * reject = new varNode(_VT_FUNCTION, "");
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ELSE) {
				proc++;
				reject->block = parseBlock();
			}
			st.right = new varNode(_VT_FUNCTION, "");
			st.right->left = accept;
			st.right->right = reject;

			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
				proc += 2;
			}

			last->act = st;
			last->next = new stateSeq();
			last = last->next;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP) {
			proc++;

			varNode *cond = NULL;
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WHEN) {
				proc++;
				cond = parseExpression();
			}

			rtVal st = rtVal();
			st.op = VO_WHILE;
			st.left = cond;
			varNode *body = new varNode(_VT_FUNCTION, "");
			body->block = parseBlock();
			st.right = body;

			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
				proc += 2;
			}

			last->act = st;
			last->next = new stateSeq();
			last = last->next;
		}
		else if (content[proc].type == SGS_TT_USER) {
			int funcIdx;
			rtVal st = rtVal();
			if ((funcIdx = findFunc()) != -1) {
				st.op = VO_EXE;
				st.left = new varNode(_VT_FUNCTION, globeFunc[funcIdx].declare.name);
				st.right = parseParameter(funcIdx);
				last->act = st;
				last->next = new stateSeq();
				last = last->next;
			}
		}
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT) {
			proc++;
			continue;
		}
	}
	return ret;
}
string SgsSyntax::findVar() {
	string tmp;
	while (proc < content.size() && content[proc].type == SGS_TT_USER ||
		content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RESULT) {
		tmp += strId[content[proc++].id];

		for (auto n : globeVar) {
			if (n.name == tmp) {
				if (n.t == _VT_ARRAY) {
					int offset;
					if (proc < content.size() &&
						content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_LPARENTHESIS) {
						proc++;
						varNode *ofvar = parseExpression();
						if (ofvar->t != _VT_INTEGER)
							error("Array index", SGS_SE_INVALIDTYPE);
						if (content[proc].type != SGS_TT_OP || content[proc].id != SGS_OP_RPARENTHESIS)
							error("[]", SGS_SE_EXPBRACE);
						proc++;
						offset = *(int *)ofvar->val;

						char buf[16];
						_itoa(offset, buf, 10);
						return string("$") + buf + "@" + tmp;
					}
				}
				else if (n.t == _VT_CLASS) {
					string element;
					if (proc < content.size() &&
						content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_OF) {
						proc++;
						element = parseUser();

						return string("$") + element + "@" + tmp;
					}
				}
				return "$" + tmp;
			}
		}
		if (func != -1) {
			for (auto n : globeFunc[func].localVar) {
				if (n.name == tmp) {
					if (n.t == _VT_ARRAY) {
						int offset;
						if (proc < content.size() &&
							content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_LPARENTHESIS) {
							proc++;
							varNode *ofvar = parseExpression();
							if (ofvar->t != _VT_INTEGER)
								error("Array index", SGS_SE_INVALIDTYPE);
							if (content[proc].type != SGS_TT_OP || content[proc].id != SGS_OP_RPARENTHESIS)
								error("[]", SGS_SE_EXPBRACE);
							proc++;
							offset = *(int *)ofvar->val;

							char buf[16];
							_itoa(offset, buf, 10);
							return string("$") + buf + "@" + tmp;
						}
					}
					else if (n.t == _VT_CLASS) {
						string element;
						if (proc < content.size() &&
							content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_OF) {
							proc++;
							element = parseUser();

							return string("$") + element + "@" + tmp;
						}
					}
					return "$" + tmp;
				}
			}
		}

		tmp += " ";
	}
	if(tmp.size())tmp.pop_back();

	return tmp;
}
int SgsSyntax::findClass() {
	string tmp;
	int preProc = proc;
	while (proc < content.size() && content[proc].type == SGS_TT_USER) {
		tmp += strId[content[proc++].id];
		for (unsigned int i = 0; i < globeClassType.size(); i++) {
			if (globeClassType[i].name == tmp)return i;
		}
		tmp += " ";
	}
	if (tmp.size())tmp.pop_back();
	proc = preProc;
	return -1;
}
int SgsSyntax::findFunc() {
	string tmp;
	int preProc = proc;
	while (proc < content.size() && content[proc].type == SGS_TT_USER) {
		tmp += strId[content[proc++].id];
		for (unsigned int i = 0; i < globeFunc.size(); i++) {
			if (globeFunc[i].declare.name == tmp)return i;
		}
		tmp += " ";
	}
	proc = preProc;
	return -1;
}
int SgsSyntax::findType() {
	if (proc >= content.size())
		error("", SGS_SE_INCOMPLETE);
	if (content[proc].type == SGS_TT_SYS) {
		if (content[proc].id >= _VT_INTEGER&&content[proc].id <= _VT_ARRAY)
			return content[proc++].id;
	}
	return -1;
}

void SgsSyntax::clearMem() {
	synMem.clear();
}

bool SgsSyntax::compare(int op1, int op2) {//op1<=op2
	int prio1, prio2;
	switch (op1) {
	case SGS_OP_LBRACE:
		prio1 = 0;
		break;
	case SGS_OP_OROR:
		prio1 = 1;
		break;
	case SGS_OP_ANDAND:
		prio1 = 2;
		break;
	case SGS_OP_OR:
		prio1 = 3;
		break;
	case SGS_OP_NOR:
		prio1 = 4;
		break;
	case SGS_OP_AND:
		prio1 = 5;
		break;
	case SGS_OP_EQUAL:
	case SGS_OP_NOTEQ:
		prio1 = 6;
		break;
	case SGS_OP_GREATER:
	case SGS_OP_NGREATER:
	case SGS_OP_SMALLER:
	case SGS_OP_NSMALLER:
		prio1 = 7;
		break;
	case SGS_OP_PLUS:
	case SGS_OP_MINUS:
		prio1 = 8;
		break;
	case SGS_OP_MULTY:
	case SGS_OP_DIVIDE:
	case SGS_OP_MOD:
		prio1 = 9;
		break;
	case SGS_OP_NOT:
	case SGS_OP_INVERSE:
	case SGS_OP_NEG:
		prio1 = 10;
		break;
	}
	switch (op2) {
	case SGS_OP_LBRACE:
		prio2 = 0;
		break;
	case SGS_OP_OROR:
		prio2 = 1;
		break;
	case SGS_OP_ANDAND:
		prio2 = 2;
		break;
	case SGS_OP_OR:
		prio2 = 3;
		break;
	case SGS_OP_NOR:
		prio2 = 4;
		break;
	case SGS_OP_AND:
		prio2 = 5;
		break;
	case SGS_OP_EQUAL:
	case SGS_OP_NOTEQ:
		prio2 = 6;
		break;
	case SGS_OP_GREATER:
	case SGS_OP_NGREATER:
	case SGS_OP_SMALLER:
	case SGS_OP_NSMALLER:
		prio2 = 7;
		break;
	case SGS_OP_PLUS:
	case SGS_OP_MINUS:
		prio2 = 8;
		break;
	case SGS_OP_MULTY:
	case SGS_OP_DIVIDE:
	case SGS_OP_MOD:
		prio2 = 9;
		break;
	case SGS_OP_NOT:
	case SGS_OP_INVERSE:
	case SGS_OP_NEG:
		prio2 = 10;
		break;
	}
	return prio1 <= prio2;
}
void SgsSyntax::error(const char *inst, int type)  {
	switch (type) {
	case SGS_SE_EXPOSE:
		throw new SGSyntaxException(std::string(inst) + "语句块不封闭。");
	case SGS_SE_UNIQUE:
		throw new SGSyntaxException(std::string(inst) + "无此用法。");
	case SGS_SE_EXPDOT:
		throw new SGSyntaxException(std::string(inst) + "缺少句号。");
	case SGS_SE_EXPCOMMA:
		throw new SGSyntaxException(std::string(inst) + "缺少逗号。");
	case SGS_SE_EXPBRACE:
		throw new SGSyntaxException(std::string(inst) + "括号不完整。");
	case SGS_SE_REDEF:
		throw new SGSyntaxException(std::string(inst) + "重定义。");
	case SGS_SE_INVALIDTYPE:
		throw new SGSyntaxException(std::string(inst) + "操作对象类型错误。");
	case SGS_SE_DISACCORD:
		throw new SGSyntaxException(std::string(inst) + "前后不一致。");
	case SGS_SE_NOID:
		throw new SGSyntaxException(std::string(inst) + "未定义。");
	case SGS_SE_INCOMPLETE:
		throw new SGSyntaxException(std::string(inst) + "语句不完整。");
	case SGS_SE_UNKNOWN:
		throw new SGSyntaxException(std::string(inst) + "未知错误。");
	case SGS_SE_UNSUPPORT:
		throw new SGSyntaxException(std::string(inst) + "暂不支持。");
	}
}
