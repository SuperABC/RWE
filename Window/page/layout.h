#ifndef RWE_WINDOW_PAGE_LAYOUT
#define RWE_WINDOW_PAGE_LAYOUT
#include "Frame/main/main.h"
#include "Window/scene/model.h"

enum LAYOUTTYPE {
	LT_WELCOME,
	LT_COMPILER,
	LT_SGS
};

class Layout {
private:
	Scene window;
	Scene dynam;
public:
	LAYOUTTYPE type;
	int id;
	bool active;

	Layout();
	~Layout();

	void registerPage();

	virtual void keyDown(int code) = 0;
	virtual void keyUp(int code) = 0;
	virtual void mouseMove(int x, int y) = 0;
	virtual void mouseDrag(int x, int y, int dx, int dy) = 0;
	virtual void mouseClick(int button, int state, int x, int y) = 0;
	virtual void mouseWheel(int wheel, int dir, int x, int y) = 0;

	virtual void idle() = 0;
};

class Welcome : public Layout {

};
class Compiler : public Layout {

};
class Sgs : public Layout {

};
#endif