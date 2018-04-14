#ifndef RWE_TEXT_EDITOR_OPERATION
#define RWE_TEXT_EDITOR_OPERATION

#include <vector>

class Operation {
private:
	enum OPERATION {
		OP_INSERT,
		OP_DELETE,
		OP_CUT,
		OP_PASTE
	};

	std::vector<std::pair<std::pair<int, int>, std::pair<int, std::string>>> list;
public:
	Operation();
	~Operation();

	void add(int block, int line, int offset, char ch);
	void add(int block, int line, int offset, std::vector<std::string> str);
	void del(int block, int line, int offset, char ch);
	void del(int block, int line, int offset, std::vector<std::string> str);
};

#endif