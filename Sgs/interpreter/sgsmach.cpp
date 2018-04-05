#define _CRT_SECURE_NO_WARNINGS
#include "Sgs/interpreter/sgsmach.h"
#include <time.h>

SgsMachine::SgsMachine() {

}
SgsMachine::~SgsMachine() {
	
}

SgsMachine *SgsMachine::input(stateSeq *seq,
	vector<classType> &classType, vector<funcNode> &func,
	vector<varNode> &var) {
	content = seq;
	globeClassType = classType;
	globeFunc = func;

	for (unsigned int i = globeVar.size(); i < var.size(); i++) {
		globeVar.push_back(var[i]);
	}
	return this;
}
void SgsMachine::execute() {
	stateSeq *iter = content;
	while (iter) {
		step(iter);
		iter = iter->next;
	}
}
void SgsMachine::execute(stateSeq *s, varNode *par, int funcid) {
	int preFunc = func;
	if (funcid != -1)func = funcid;
	stateSeq *iter = s;

	while (par != NULL) {
		for (auto &v : globeFunc[func].localVar) {
			if (v.name == par->name) {
				v.t = par->t;
				v.val = par->val;
			}
		}

		par = par->next;
	}
	while (iter) {
		step(iter);
		iter = iter->next;
	}

	func = preFunc;
}
void SgsMachine::step(stateSeq *s) {
	if (s->act.op == VO_ASSIGN) {
		int offset = -1;
		string element = string();
		if (s->act.left->name[0] >= '0' && s->act.left->name[0] <= '9') {
			offset = atoi(s->act.left->name.data());
			s->act.left->name = s->act.left->name.data() + s->act.left->name.find('@') + 1;
		}
		else if ((offset = s->act.left->name.find('@')) != -1) {
			element = s->act.left->name.substr(0, offset);
		}
		for (unsigned int i = 0; i < globeVar.size(); i++) {
			if (globeVar[i].name == s->act.left->name) {
				varNode tmp = exp(s->act.right);
				switch (s->act.left->t) {
				case _VT_INTEGER:
					if (tmp.t == _VT_INTEGER) {
						globeVar[i].val = tmp.val;
					}
					else if (tmp.t == _VT_FLOAT) {
						globeVar[i].val = new int((int)*(float *)tmp.val);
					}
					else if (tmp.t == _VT_BOOL) {
						globeVar[i].val = new int(*(bool *)tmp.val);
					}
					else if (tmp.t == _VT_CHAR) {
						globeVar[i].val = new int(*(char *)tmp.val);
					}
					else
						error(s->act.left->name.data(), VE_TYPEMISMATCH);

					break;
				case _VT_BOOL:
					if (tmp.t == _VT_INTEGER) {
						globeVar[i].val = new bool(*(int *)tmp.val);
					}
					else if (tmp.t == _VT_FLOAT) {
						globeVar[i].val = new bool(*(float *)tmp.val);
					}
					else if (tmp.t == _VT_BOOL) {
						globeVar[i].val = tmp.val;
					}
					else if (tmp.t == _VT_CHAR) {
						globeVar[i].val = new bool(*(char *)tmp.val);
					}
					else
						error(s->act.left->name.data(), VE_TYPEMISMATCH);

					break;
				case _VT_CHAR:
					if (tmp.t == _VT_INTEGER) {
						globeVar[i].val = new char(*(int *)tmp.val);
					}
					else if (tmp.t == _VT_FLOAT) {
						globeVar[i].val = new char((char)*(float *)tmp.val);
					}
					else if (tmp.t == _VT_BOOL) {
						globeVar[i].val = new char(*(bool *)tmp.val);
					}
					else if (tmp.t == _VT_CHAR) {
						globeVar[i].val = tmp.val;
					}
					else
						error(s->act.left->name.data(), VE_TYPEMISMATCH);

					break;
				case _VT_FLOAT:
					if (tmp.t == _VT_INTEGER) {
						globeVar[i].val = new float((float)*(int *)tmp.val);
					}
					else if (tmp.t == _VT_FLOAT) {
						globeVar[i].val = tmp.val;
					}
					else if (tmp.t == _VT_BOOL) {
						globeVar[i].val = new float(*(bool *)tmp.val);
					}
					else if (tmp.t == _VT_CHAR) {
						globeVar[i].val = new float(*(char *)tmp.val);
					}
					else
						error(s->act.left->name.data(), VE_TYPEMISMATCH);

					break;
				case _VT_STRING:
					if (tmp.t == _VT_STRING)
						globeVar[i].val = tmp.val;
					else
						error(s->act.left->name.data(), VE_TYPEMISMATCH);
					break;
				case _VT_ARRAY:
					if (offset == -1) {
						if (tmp.t == _VT_ARRAY)
							globeVar[i].val = tmp.val;
						else
							error(s->act.left->name.data(), VE_TYPEMISMATCH);
					}
					else {
						varNode *start = (varNode *)globeVar[i].val;
						while (offset--) {
							start = start->next;
						}
						switch (start->t) {
						case _VT_INTEGER:
							if (tmp.t == _VT_INTEGER) {
								start->val = tmp.val;
							}
							else if (tmp.t == _VT_FLOAT) {
								start->val = new int((int)*(float *)tmp.val);
							}
							else if (tmp.t == _VT_BOOL) {
								start->val = new int(*(bool *)tmp.val);
							}
							else if (tmp.t == _VT_CHAR) {
								start->val = new int(*(char *)tmp.val);
							}
							else
								error(s->act.left->name.data(), VE_TYPEMISMATCH);

							break;
						case _VT_BOOL:
							if (tmp.t == _VT_INTEGER) {
								start->val = new bool(*(int *)tmp.val);
							}
							else if (tmp.t == _VT_FLOAT) {
								start->val = new bool(*(float *)tmp.val);
							}
							else if (tmp.t == _VT_BOOL) {
								start->val = tmp.val;
							}
							else if (tmp.t == _VT_CHAR) {
								start->val = new bool(*(char *)tmp.val);
							}
							else
								error(s->act.left->name.data(), VE_TYPEMISMATCH);

							break;
						case _VT_CHAR:
							if (tmp.t == _VT_INTEGER) {
								start->val = new char(*(int *)tmp.val);
							}
							else if (tmp.t == _VT_FLOAT) {
								start->val = new char((char)*(float *)tmp.val);
							}
							else if (tmp.t == _VT_BOOL) {
								start->val = new char(*(bool *)tmp.val);
							}
							else if (tmp.t == _VT_CHAR) {
								start->val = tmp.val;
							}
							else
								error(s->act.left->name.data(), VE_TYPEMISMATCH);

							break;
						case _VT_FLOAT:
							if (tmp.t == _VT_INTEGER) {
								start->val = new float((float)*(int *)tmp.val);
							}
							else if (tmp.t == _VT_FLOAT) {
								start->val = tmp.val;
							}
							else if (tmp.t == _VT_BOOL) {
								start->val = new float(*(bool *)tmp.val);
							}
							else if (tmp.t == _VT_CHAR) {
								start->val = new float(*(char *)tmp.val);
							}
							else
								error(s->act.left->name.data(), VE_TYPEMISMATCH);

							break;
						case _VT_STRING:
							if (tmp.t == _VT_STRING)
								start->val = tmp.val;
							else
								error(s->act.left->name.data(), VE_TYPEMISMATCH);
							break;
						}
					}

					break;
				case _VT_CLASS:
					if (tmp.t == _VT_CLASS && tmp.classType == s->act.left->classType) {
						if (element.empty()) {
							globeVar[i].val = tmp.val;
						}
						else {
							for (auto &ele : ((classNode *)globeVar[i].val)->varList) {
								if (ele.name == element) {
									ele.val = tmp.val;
									break;
								}
							}
						}
					}
					else
						error(s->act.left->classType.data(), VE_TYPEMISMATCH);
					break;
				case _VT_NULL:
					globeVar[i].t = tmp.t;
					globeVar[i].val = tmp.val;
					globeVar[i].classType = tmp.classType;

					break;
				}

				break;
			}
		}
		if (func != -1) {
			for (unsigned int i = 0; i < globeFunc[func].localVar.size(); i++) {
				if (globeFunc[func].localVar[i].name == s->act.left->name) {
					varNode tmp = exp(s->act.right);
					switch (s->act.left->t) {
					case _VT_INTEGER:
						if (tmp.t == _VT_INTEGER) {
							globeFunc[func].localVar[i].val = tmp.val;
						}
						else if (tmp.t == _VT_FLOAT) {
							globeFunc[func].localVar[i].val = new int((int)*(float *)tmp.val);
						}
						else if (tmp.t == _VT_BOOL) {
							globeFunc[func].localVar[i].val = new int(*(bool *)tmp.val);
						}
						else if (tmp.t == _VT_CHAR) {
							globeFunc[func].localVar[i].val = new int(*(char *)tmp.val);
						}
						else
							error(s->act.left->name.data(), VE_TYPEMISMATCH);

						break;
					case _VT_BOOL:
						if (tmp.t == _VT_INTEGER) {
							globeFunc[func].localVar[i].val = new bool(*(int *)tmp.val);
						}
						else if (tmp.t == _VT_FLOAT) {
							globeFunc[func].localVar[i].val = new bool(*(float *)tmp.val);
						}
						else if (tmp.t == _VT_BOOL) {
							globeFunc[func].localVar[i].val = tmp.val;
						}
						else if (tmp.t == _VT_CHAR) {
							globeFunc[func].localVar[i].val = new bool(*(char *)tmp.val);
						}
						else
							error(s->act.left->name.data(), VE_TYPEMISMATCH);

						break;
					case _VT_CHAR:
						if (tmp.t == _VT_INTEGER) {
							globeFunc[func].localVar[i].val = new char(*(int *)tmp.val);
						}
						else if (tmp.t == _VT_FLOAT) {
							globeFunc[func].localVar[i].val = new char((char)*(float *)tmp.val);
						}
						else if (tmp.t == _VT_BOOL) {
							globeFunc[func].localVar[i].val = new char(*(bool *)tmp.val);
						}
						else if (tmp.t == _VT_CHAR) {
							globeFunc[func].localVar[i].val = tmp.val;
						}
						else
							error(s->act.left->name.data(), VE_TYPEMISMATCH);

						break;
					case _VT_FLOAT:
						if (tmp.t == _VT_INTEGER) {
							globeFunc[func].localVar[i].val = new float((float)*(int *)tmp.val);
						}
						else if (tmp.t == _VT_FLOAT) {
							globeFunc[func].localVar[i].val = tmp.val;
						}
						else if (tmp.t == _VT_BOOL) {
							globeFunc[func].localVar[i].val = new float(*(bool *)tmp.val);
						}
						else if (tmp.t == _VT_CHAR) {
							globeFunc[func].localVar[i].val = new float(*(char *)tmp.val);
						}
						else
							error(s->act.left->name.data(), VE_TYPEMISMATCH);

						break;
					case _VT_STRING:
						if (tmp.t == _VT_STRING)
							globeFunc[func].localVar[i].val = tmp.val;
						else
							error(s->act.left->name.data(), VE_TYPEMISMATCH);
						break;
					case _VT_ARRAY:
						if (offset == -1) {
							if (tmp.t == _VT_ARRAY)
								globeVar[i].val = tmp.val;
							else
								error(s->act.left->name.data(), VE_TYPEMISMATCH);
						}
						else {
							varNode *start = (varNode *)globeFunc[func].localVar[i].val;
							while (offset--) {
								start = start->next;
							}
							switch (start->t) {
							case _VT_INTEGER:
								if (tmp.t == _VT_INTEGER) {
									start->val = tmp.val;
								}
								else if (tmp.t == _VT_FLOAT) {
									start->val = new int((int)*(float *)tmp.val);
								}
								else if (tmp.t == _VT_BOOL) {
									start->val = new int(*(bool *)tmp.val);
								}
								else if (tmp.t == _VT_CHAR) {
									start->val = new int(*(char *)tmp.val);
								}
								else
									error(s->act.left->name.data(), VE_TYPEMISMATCH);

								break;
							case _VT_BOOL:
								if (tmp.t == _VT_INTEGER) {
									start->val = new bool(*(int *)tmp.val);
								}
								else if (tmp.t == _VT_FLOAT) {
									start->val = new bool(*(float *)tmp.val);
								}
								else if (tmp.t == _VT_BOOL) {
									start->val = tmp.val;
								}
								else if (tmp.t == _VT_CHAR) {
									start->val = new bool(*(char *)tmp.val);
								}
								else
									error(s->act.left->name.data(), VE_TYPEMISMATCH);

								break;
							case _VT_CHAR:
								if (tmp.t == _VT_INTEGER) {
									start->val = new char(*(int *)tmp.val);
								}
								else if (tmp.t == _VT_FLOAT) {
									start->val = new char((char)*(float *)tmp.val);
								}
								else if (tmp.t == _VT_BOOL) {
									start->val = new char(*(bool *)tmp.val);
								}
								else if (tmp.t == _VT_CHAR) {
									start->val = tmp.val;
								}
								else
									error(s->act.left->name.data(), VE_TYPEMISMATCH);

								break;
							case _VT_FLOAT:
								if (tmp.t == _VT_INTEGER) {
									start->val = new float((float)*(int *)tmp.val);
								}
								else if (tmp.t == _VT_FLOAT) {
									start->val = tmp.val;
								}
								else if (tmp.t == _VT_BOOL) {
									start->val = new float(*(bool *)tmp.val);
								}
								else if (tmp.t == _VT_CHAR) {
									start->val = new float(*(char *)tmp.val);
								}
								else
									error(s->act.left->name.data(), VE_TYPEMISMATCH);

								break;
							case _VT_STRING:
								if (tmp.t == _VT_STRING)
									start->val = tmp.val;
								else
									error(s->act.left->name.data(), VE_TYPEMISMATCH);
								break;
							}
						}

						break;
					case _VT_CLASS:
						if (tmp.t == _VT_CLASS && tmp.classType == s->act.left->classType) {
							if (element.empty()) {
								globeFunc[func].localVar[i].val = tmp.val;
							}
							else {
								for (auto &ele : ((classNode *)globeFunc[func].localVar[i].val)->varList) {
									if (ele.name == element) {
										ele.val = tmp.val;
										break;
									}
								}
							}
						}
						else
							error(s->act.left->classType.data(), VE_TYPEMISMATCH);
						break;
					case _VT_NULL:
						globeFunc[func].localVar[i].t = tmp.t;
						globeFunc[func].localVar[i].val = tmp.val;
						globeFunc[func].localVar[i].classType = tmp.classType;

						break;
					}

					break;
				}
			}
		}
	}
	else if (s->act.op == VO_EXE) {
		varNode *iter = s->act.right;
		varNode *par = new varNode();
		varNode *start = par;
		while (iter != NULL) {
			*par = exp(iter);
			par->name = iter->name;
			iter = iter->next;
			if(iter)par->next = new varNode();
			par = par->next;
		}
		if (s->act.left->name == "out") {
			out(start);
		}
		else if (s->act.left->name == "outln") {
			outln(start);
		}
		else if (s->act.left->name == "current time") {
			time(start);
		}
		else if(s->act.left->name == "in") {
			in(start);
		}
		else {
			for (unsigned int i = 0; i < globeFunc.size(); i++) {
				if (globeFunc[i].declare.name == s->act.left->name) {
					execute(&globeFunc[i].content, start, i);
				}
			}
		}
	}
	else if (s->act.op == VO_IF) {
		varNode res = exp(s->act.left);
		bool ar;
		switch (res.t) {
		case _VT_INTEGER:
			if (*(int *)res.val)ar = true;
			else ar = false;
			break;
		case _VT_BOOL:
			ar = *(bool *)res.val;
			break;
		}
		if (ar) {
			execute(s->act.right->left->block, NULL);
		}
		else {
			execute(s->act.right->right->block, NULL);
		}
	}
	else if (s->act.op == VO_WHILE) {
		while (true) {
			varNode cond = exp(s->act.left);

			bool loop;
			switch (cond.t) {
			case _VT_INTEGER:
				if (*(int *)cond.val)loop = true;
				else loop = false;
				break;
			case _VT_BOOL:
				loop = *(bool *)cond.val;
				break;
			}
			if (!loop)break;

			execute(s->act.right->block, NULL);
		}
	}
}
varNode SgsMachine::exp(varNode *e) {
	varNode ret;
	if (e->t == _VT_FUNCTION) {
		if (e->block->act.left->name == "current time") {
			ret = time(e->block->act.right);
		}
		else if (e->block->act.left->name == "in") {
			ret = in(e->block->act.right);
		}
		else if (e->block->act.left->name == "random number") {
			ret = random(e->block->act.right);
		}
		else {
			for (unsigned int i = 0; i < globeFunc.size(); i++) {
				if (globeFunc[i].declare.name == e->block->act.left->name) {
					execute(&globeFunc[i].content, e->block->act.right, i);
					ret = globeFunc[i].localVar[0];
				}
			}
		}
	}
	else if (e->t == _VT_EXP) {
		varNode l = exp(e->left);
		varNode r = exp(e->right);

		switch (*(int *)e->val) {
		case SGS_OP_PLUS:
			if (l.t == _VT_FLOAT && r.t == _VT_FLOAT) {
				ret.t = _VT_FLOAT;
				ret.val = new float(*(float *)(l.val) + *(float *)(r.val));
			}
			else if (l.t == _VT_FLOAT && r.t == _VT_INTEGER) {
				ret.t = _VT_FLOAT;
				ret.val = new float(*(float *)(l.val) + *(int *)(r.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_FLOAT) {
				ret.t = _VT_FLOAT;
				ret.val = new float(*(int *)(l.val) + *(float *)(r.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_INTEGER) {
				ret.t = _VT_INTEGER;
				ret.val = new int(*(int *)(l.val) + *(int *)(r.val));
			}
			else if (l.t == _VT_STRING && r.t == _VT_STRING) {
				ret.t = _VT_STRING;
				ret.val = new char[strlen((char *)l.val) + strlen((char *)r.val)];
				strcpy((char *)ret.val, (char *)r.val);
				strcat((char *)ret.val, (char *)l.val);
			}
			break;
		case SGS_OP_MINUS:
			if (l.t == _VT_FLOAT && r.t == _VT_FLOAT) {
				ret.t = _VT_FLOAT;
				ret.val = new float(*(float *)(r.val) - *(float *)(l.val));
			}
			else if (l.t == _VT_FLOAT && r.t == _VT_INTEGER) {
				ret.t = _VT_FLOAT;
				ret.val = new float(*(int *)(r.val) - *(float *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_FLOAT) {
				ret.t = _VT_FLOAT;
				ret.val = new float(*(float *)(r.val) - *(int *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_INTEGER) {
				ret.t = _VT_INTEGER;
				ret.val = new int(*(int *)(r.val) - *(int *)(l.val));
			}
			break;
		case SGS_OP_MULTY:
			if (l.t == _VT_FLOAT && r.t == _VT_FLOAT) {
				ret.t = _VT_FLOAT;
				ret.val = new float(*(float *)(l.val) * *(float *)(r.val));
			}
			else if (l.t == _VT_FLOAT && r.t == _VT_INTEGER) {
				ret.t = _VT_FLOAT;
				ret.val = new float(*(float *)(l.val) * *(int *)(r.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_FLOAT) {
				ret.t = _VT_FLOAT;
				ret.val = new float(*(int *)(l.val) * *(float *)(r.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_INTEGER) {
				ret.t = _VT_INTEGER;
				ret.val = new int(*(int *)(l.val) * *(int *)(r.val));
			}
			break;
		case SGS_OP_DIVIDE:
			if (l.t == _VT_FLOAT && r.t == _VT_FLOAT) {
				ret.t = _VT_FLOAT;
				if (*(float *)(l.val) == 0)
					error("", VE_DIVBYZERO);
				ret.val = new float(*(float *)(r.val) / *(float *)(l.val));
			}
			else if (l.t == _VT_FLOAT && r.t == _VT_INTEGER) {
				ret.t = _VT_FLOAT;
				if (*(float *)(l.val) == 0)
					error("", VE_DIVBYZERO);
				ret.val = new float(*(int *)(r.val) / *(float *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_FLOAT) {
				ret.t = _VT_FLOAT;
				if (*(int *)(l.val) == 0)
					error("", VE_DIVBYZERO);
				ret.val = new float(*(float *)(r.val) / *(int *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_INTEGER) {
				ret.t = _VT_INTEGER;
				if (*(int *)(l.val) == 0)
					error("", VE_DIVBYZERO);
				ret.val = new int(*(int *)(r.val) / *(int *)(l.val));
			}
			break;
		case SGS_OP_MOD:
			if (l.t == _VT_INTEGER && r.t == _VT_INTEGER) {
				ret.t = _VT_INTEGER;
				ret.val = new int(*(int *)(r.val) % *(int *)(l.val));
			}
			else error("mod", VE_TYPEMISMATCH);
			break;
		case SGS_OP_EQUAL:
			if (l.t == _VT_FLOAT && r.t == _VT_FLOAT) {
				ret.t = _VT_BOOL;
ret.val = new bool(*(float *)(r.val) == *(float *)(l.val));
			}
			else if (l.t == _VT_FLOAT && r.t == _VT_INTEGER) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(int *)(r.val) == *(float *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_FLOAT) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(float *)(r.val) == *(int *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_INTEGER) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(int *)(r.val) == *(int *)(l.val));
			}
			break;
		case SGS_OP_GREATER:
			if (l.t == _VT_FLOAT && r.t == _VT_FLOAT) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(float *)(r.val) > *(float *)(l.val));
			}
			else if (l.t == _VT_FLOAT && r.t == _VT_INTEGER) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(int *)(r.val) > *(float *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_FLOAT) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(float *)(r.val) > *(int *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_INTEGER) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(int *)(r.val) > *(int *)(l.val));
			}
			break;
		case SGS_OP_SMALLER:
			if (l.t == _VT_FLOAT && r.t == _VT_FLOAT) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(float *)(r.val) < *(float *)(l.val));
			}
			else if (l.t == _VT_FLOAT && r.t == _VT_INTEGER) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(int *)(r.val) < *(float *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_FLOAT) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(float *)(r.val) < *(int *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_INTEGER) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(int *)(r.val) < *(int *)(l.val));
			}
			break;
		case SGS_OP_NSMALLER:
			if (l.t == _VT_FLOAT && r.t == _VT_FLOAT) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(float *)(r.val) >= *(float *)(l.val));
			}
			else if (l.t == _VT_FLOAT && r.t == _VT_INTEGER) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(int *)(r.val) >= *(float *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_FLOAT) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(float *)(r.val) >= *(int *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_INTEGER) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(int *)(r.val) >= *(int *)(l.val));
			}
			break;
		case SGS_OP_NGREATER:
			if (l.t == _VT_FLOAT && r.t == _VT_FLOAT) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(float *)(r.val) <= *(float *)(l.val));
			}
			else if (l.t == _VT_FLOAT && r.t == _VT_INTEGER) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(int *)(r.val) <= *(float *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_FLOAT) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(float *)(r.val) <= *(int *)(l.val));
			}
			else if (l.t == _VT_INTEGER && r.t == _VT_INTEGER) {
				ret.t = _VT_BOOL;
				ret.val = new bool(*(int *)(r.val) <= *(int *)(l.val));
			}
			break;
		}
	}
	else if (e->t == _VT_VAR) {
		int offset = -1;
		string element = string();
		if (((char *)e->val)[0] >= '0' && ((char *)e->val)[0] <= '9') {
			offset = atoi((char *)e->val);
			while (*(char *)e->val != '@') {
				e->val = (char *)e->val + 1;
			}
			e->val = (char *)e->val + 1;
		}
		else if ((offset = string((char *)e->val).find('@'))!=-1) {
			element = string((char *)e->val).substr(0, offset);
			e->val = (char *)e->val + offset + 1;
		}
		for (auto v : globeVar) {
			if ((char *)e->val == v.name) {
				if (offset == -1) {
					ret.t = v.t;
					ret.val = v.val;
					break;
				}
				else if(element.empty()) {
					varNode *start = (varNode *)v.val;
					while (offset--) {
						start = start->next;
					}
					ret.t = start->t;
					ret.val = start->val;
					break;
				}
				else {
					classNode *parent = (classNode *)v.val;
					for(unsigned int i = 0; i < parent->type.varList.size(); i++) {
						if (parent->type.varList[i].name == element) {
							ret.t = parent->varList[i].t;
							ret.val = parent->varList[i].val;
							break;
						}
					}
				}
			}
		}
		if (func != -1) {
			for (auto v : globeFunc[func].localVar) {
				if ((char *)e->val == v.name) {
					ret.t = v.t;
					ret.val = v.val;
					break;
				}
			}
		}
	}
	else {
		ret.t = e->t;
		ret.classType = e->classType;
		ret.val = e->val;
		ret.next = e->next;
	}

	return ret;
}
void SgsMachine::setup() {
	for (unsigned int i = 0; i < globeFunc.size(); i++) {
		if (globeFunc[i].declare.name == "sg setup") {
			execute(&globeFunc[i].content, NULL, i);
		}
	}
}
void SgsMachine::loop() {
	for (unsigned int i = 0; i < globeFunc.size(); i++) {
		if (globeFunc[i].declare.name == "sg main") {
			execute(&globeFunc[i].content, NULL, i);
		}
	}
}

void SgsMachine::print(varNode *par) {
	char tmp[64] = "";

	switch (par->t) {
	case _VT_STRING: {
		sprintf(tmp, "%s", (char *)par->val);
		break;
	}
	case _VT_INTEGER: {
		sprintf(tmp, "%d", *(int *)(par->val));
		break;
	}
	case _VT_FLOAT: {
		sprintf(tmp, "%f", *(float *)(par->val));
		break;
	}
	case _VT_CHAR: {
		sprintf(tmp, "%c", *(char *)(par->val));
		break;
	}
	case _VT_BOOL: {
		if(*(bool *)(par->val))
			sprintf(tmp, "true");
		else sprintf(tmp, "false");
		break;
	}
	case _VT_VAR: {
		print(&exp(par));
		break;
	}
	case _VT_ARRAY: {
		varNode * iter = (varNode *)par->val;
		while (iter) {
			print(iter);
			cout << " ";
			iter = iter->next;
		}
		break;
	}
	case _VT_CLASS: {
		classNode * tmp = (classNode *)par->val;
		cout << "{ ";
		for (auto &iter : tmp->varList) {
			print(&iter);
			cout << " ";
		}
		cout << "}";
		break;
	}

	}
	cout << tmp;
}
void SgsMachine::out(varNode *par) {
	if (par->name == "content") {
		print(par);
	}
}
void SgsMachine::outln(varNode *par) {
	if (par->name == "content") {
		print(par);
		cout << endl;
	}
}
varNode SgsMachine::in(varNode *par) {
	for (auto &f : globeFunc) {
		if (f.declare.name == "in") {
			string *tmp = new string();
			std::cin >> *tmp;

			if ((*tmp)[0] >= '0'&&(*tmp)[0] <= '9') {
				if (tmp->find('.') == -1) {
					f.localVar[0].t = _VT_INTEGER;
					f.localVar[0].val = new int(atoi(tmp->data()));
					return f.localVar[0];
				}
				else {
					f.localVar[0].t = _VT_FLOAT;
					f.localVar[0].val = new float((float)atof(tmp->data()));
					return f.localVar[0];
				}
			}
			else {
				f.localVar[0].t = _VT_STRING;
				f.localVar[0].val = tmp->data();
				return f.localVar[0];
			}

		}
	}
	error("", VE_BROKEN);
	return varNode();
}
varNode SgsMachine::time(varNode *par) {
	for (auto &f : globeFunc) {
		if (f.declare.name == "current time") {
			f.localVar[0].t = _VT_INTEGER;
			f.localVar[0].val = new int(clock());
			return f.localVar[0];
		}
	}
	error("", VE_BROKEN);
	return varNode();
}
varNode SgsMachine::random(varNode *par) {
	for (auto &f : globeFunc) {
		if (f.declare.name == "random number") {
			f.localVar[0].t = _VT_INTEGER;
			f.localVar[0].val = new int(rand());
			return f.localVar[0];
		}
	}
	error("", VE_BROKEN);
	return varNode();
}

// Graphic interfaces for sgl.
void SgsMachine::initWindow(varNode *par) {
	int width, height;
	const char *title = "SGS-SGL";
	while (par) {
		if (par->name == "width")width = *(int *)par->val;
		else if (par->name == "height")height = *(int *)par->val;
		else if (par->name == "title")title = (char *)par->val;
		par = par->next;
	}
	canvas.initWindow(width, height, title, Screen::BIT_MAP);
}
void SgsMachine::setColor(varNode *par) {
	int r, g, b;
	while (par) {
		if (par->name == "red")r = *(int *)par->val;
		else if (par->name == "green")g = *(int *)par->val;
		else if (par->name == "blue")b = *(int *)par->val;
		par = par->next;
	}
	canvas.setColor(r, g, b);
}
void SgsMachine::clearScreen(varNode *par) {
	canvas.clearScreen();
}
void SgsMachine::putLine(varNode *par) {
	int x1, y1, x2, y2;
	while (par) {
		if (par->name == "first x")x1 = *(int *)par->val;
		else if (par->name == "first y")y1 = *(int *)par->val;
		else if (par->name == "second x")x2 = *(int *)par->val;
		else if (par->name == "second y")y2 = *(int *)par->val;
		par = par->next;
	}
	canvas.putLine(x1, y1, x2, y2, Screen::SOLID_LINE);
}
void SgsMachine::putQuad(varNode *par) {
	int x1, y1, x2, y2, mode;
	while (par) {
		if (par->name == "left")x1 = *(int *)par->val;
		else if (par->name == "top")y1 = *(int *)par->val;
		else if (par->name == "right")x2 = *(int *)par->val;
		else if (par->name == "bottom")y2 = *(int *)par->val;
		else if (par->name == "filling")mode = *(bool *)par->val;
		par = par->next;
	}
	canvas.putQuad(x1, y1, x2, y2, mode);
}
void SgsMachine::putTriangle(varNode *par) {

}
void SgsMachine::putCircle(varNode *par) {

}
void SgsMachine::putEllipse(varNode *par) {

}

void SgsMachine::clearMem() {
	macMem.clear();
}

void SgsMachine::error(const char *inst, int type) {
	switch (type) {
	case VE_DIVBYZERO:
		throw new MachineException(std::string("0不能做被除数。"));
	case VE_TYPEMISMATCH:
		throw new MachineException(std::string(inst) + "类型不匹配。");
	case VE_BROKEN:
		throw new MachineException("虚拟机未准备或者已被破坏。");
	}
}
