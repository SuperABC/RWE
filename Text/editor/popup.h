#ifndef RWE_TEXT_EDITOR_POPUP
#define RWE_TEXT_EDITOR_POPUP

#include <vector>

class Popup {
private:
	std::vector<std::string>content;

	void layout();

public:
	Popup();
	~Popup();

	void add(std::string input);
	void add(std::vector<std::string> input);
	void prepare();
	void show(int x, int y);
	void clear();
};

#endif