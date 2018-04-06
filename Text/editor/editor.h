#ifndef RWE_TEXT_EDITOR_EDITOR
#define RWE_TEXT_EDITOR_EDITOR

#include "Text/editor/file.h"
#include "Text/editor/lang.h"
#include "Sgs/interpreter/sgsdebug.h"
#include "Window/scene/bitmap.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Editor {
private:
	File *content;
	int fileNum = 0;

	int active = -1;
	bool editing = false;

	glm::vec2 fileListBase;
	glm::vec2 fileListSize;
	glm::vec2 outputWindowBase;
	glm::vec2 outputWindowSize;

	RGB lnColor;
	RGB selectColor;

	void scroll(int line);
	inline bool selecting() {
		File *tmpFile = actFile();
		if (tmpFile->selectBegin != tmpFile->selectEnd)return true;
		Block *tmpBlock = tmpFile->blockAt(tmpFile->selectBegin);
		if (tmpBlock->selectBegin != tmpBlock->selectEnd)return true;
		Line *tmpLine = tmpBlock->lineAt(tmpBlock->selectBegin);
		if (tmpLine->selectBegin != tmpLine->selectEnd)return true;
		return false;
	}
public:
	Canvas canvas;

	int fontSize;
	string fontName;
	glm::vec2 editorBase;
	glm::vec2 editorSize;
	glm::vec2 cursorPos;
	RGB cursorColor;

	Editor(string config = string());
	~Editor();

	void resize(int x, int y);
	void create(char *dir);
	void open(char *filename);
	void save(char *filename = NULL);
	void close();
	void cut();
	vector<string> copy();
	void paste(vector<string> cont);
	void choose();
	void tab();
	void untab();
	void refresh();

	void key(int key);
	void mouse(glm::vec3 mouse);
	void drag(glm::vec2 pos);

	File *actFile();
	int getNum();
};

#endif