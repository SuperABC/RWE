#ifndef RWE_WINDOW_LAYOUT_LAYOUT
#define RWE_WINDOW_LAYOUT_LAYOUT

#include "Frame/main/main.h"
#include "Window/scene/model.h"
#include "Window/scene/bitmap.h"

#define MAX_WND 16

enum LAYOUTTYPE {
	LT_WELCOME,
	LT_COMPILER,
	LT_SGS
};

class Layout {
protected:
	Scene window;
public:
	LAYOUTTYPE type;
	bool active;

	Layout() : active(false) {};
	~Layout() {};

	void registerPage();

	virtual void keyDown(int code) = 0;
	virtual void keyUp(int code) = 0;
	virtual void mouseMove(Ray r) = 0;
	virtual void mouseDrag(Ray r, Ray pre) = 0;
	virtual void mouseClick(int button, int state, Ray r) = 0;
	virtual void mouseWheel(int wheel, int dir, Ray r) = 0;

	virtual void idle() = 0;
};

class Welcome : public Layout {
private:
	Canvas canvas;
public:
	Welcome();

	virtual void keyDown(int code);
	virtual void keyUp(int code);
	virtual void mouseMove(Ray r);
	virtual void mouseDrag(Ray r, Ray pre);
	virtual void mouseClick(int button, int state, Ray r);
	virtual void mouseWheel(int wheel, int dir, Ray r);
	virtual void idle();
};
class Text : public Layout {
private:
	//Editor editor;
public:
	Text();

	virtual void keyDown(int code);
	virtual void keyUp(int code);
	virtual void mouseMove(Ray r);
	virtual void mouseDrag(Ray r, Ray pre);
	virtual void mouseClick(int button, int state, Ray r);
	virtual void mouseWheel(int wheel, int dir, Ray r);
	virtual void idle();
};
class Sgs : public Layout {
private:
	//Editor editor;
	//SgsMach machine;
	//Console console;
public:
	Sgs();

	virtual void keyDown(int code);
	virtual void keyUp(int code);
	virtual void mouseMove(Ray r);
	virtual void mouseDrag(Ray r, Ray pre);
	virtual void mouseClick(int button, int state, Ray r);
	virtual void mouseWheel(int wheel, int dir, Ray r);
	virtual void idle();
};
#endif