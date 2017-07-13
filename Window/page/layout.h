#ifndef RWE_WINDOW_PAGE_LAYOUT
#define RWE_WINDOW_PAGE_LAYOUT
#include "Frame/main/main.h"
#include "Window/scene/model.h"

enum LAYOUTTYPE {
	LT_WELCOME,
	LT_NORMAL,
	LT_SGS
};

class Layout {
private:
	Scene window;
public:
	LAYOUTTYPE type;
	int id;
	bool active;

	Layout();
	~Layout();

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
class Normal : public Layout {

};
class Sgs : public Layout {

};
#endif