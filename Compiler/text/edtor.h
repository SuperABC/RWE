#ifndef RWE_COMPILER_TEXT_EDITOR
#define RWE_COMPILER_TEXT_EDITOR
#include "Frame/main/main.h"
#include "block.h"
#include "Window/scene/model.h"
#include "Geometry/primitive/shape.h"

class Editor {
private:
	vector<Element> files;
	int active;

	Square area;
public:
	Editor();
	~Editor();
	void build();
};

#endif