#ifndef RWE_TEXT_EDITOR_LANG
#define RWE_TEXT_EDITOR_LANG

#include <vector>

class Language {
public:
	Language() {}
	virtual ~Language() {}

	virtual Language * input(const char *str) = 0;
	virtual std::vector<std::pair<int, std::pair<int, int>>> highlight() = 0;
	virtual std::vector<std::string> complete(std::string tab) = 0;
};

#endif