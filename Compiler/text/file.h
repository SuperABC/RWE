#ifndef RWE_COMPILER_TEXT_FILE
#define RWE_COMPILER_TEXT_FILE
#include "Frame/main/main.h"
#include "block.h"
#include "Window/scene/model.h"

enum FILETYPE {
	FT_TXT,
	FT_C,
	FT_CPP,
	FT_SGS
};

class File {
private:
	list<Block> text;
	vector<Element> model;
public:
	FILETYPE type;

	string path;
	string name;
	int lineHeight;
	
	File();
	~File();
	void load(string filename);
	void save(string filename);
	vector<Element> show();
};

#endif