#ifndef RWE_COMPILER_TEXT_EDITOR
#define RWE_COMPILER_TEXT_EDITOR
#include "Frame/main/main.h"
#include "block.h"
#include "file.h"
#include "Geometry/primitive/shape.h"

class Editor {
private:
	vector<File> files;
	int active;

	Square area;
public:
	Editor() {};
	Editor(Square s) {};
	~Editor() {};
	void show();


};

#endif