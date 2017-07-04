#ifndef RWE_WINDOW_PAGE_LAYOUT
#define RWE_WINDOW_PAGE_LAYOUT
#include "Window/scene/model.h"

enum LAYOUTTYPE {
	LT_WELCOME = 0,
	LT_TEXT = 1,
	LT_GRAPHIC,
	LT_SGS
};

class Layout {
private:
	Scene window;
public:
	LAYOUTTYPE type;
	int id;
	bool active;

	void registerPage();

	void keyDown(int code);
	void keyUp(int code);
	void mouseMove(int x, int y);
	void mosueDrag(int x, int y);
	void mouseClick(int button, int state, int x, int y);
	void mouseWheel(int wheel, int dir, int x, int y);

	void idle();
};

class Welcome : public Layout {

};
class Text : public Layout {

};
class Graphic : public Layout {

};
class Sgs : public Layout {

};
#endif