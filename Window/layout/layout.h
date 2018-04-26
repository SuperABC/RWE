#ifndef RWE_WINDOW_LAYOUT_LAYOUT
#define RWE_WINDOW_LAYOUT_LAYOUT

#include "Frame/main/main.h"
#include "Window/scene/model.h"
#include "Window/scene/bitmap.h"
#include "Text/editor/editor.h"
#include "Cpp/gcc/gcc.h"

#define MAX_WND 16

enum LAYOUTTYPE {
	LT_WELCOME,
	LT_TEXT,
	LT_SGS,
	LT_CPP
};

class Layout {
protected:
	Scene window;
public:
	LAYOUTTYPE type;
	string name;
	bool active;

	Layout(string n) : name(n), active(false) {};
	~Layout() {};

	void registerPage();
	void destroy();
	void activate(bool act) {
		active = act;
		window.active = act;
	}

	virtual void keyDown(int code) = 0;
	virtual void keyUp(int code) = 0;
	virtual void mouseMove(Ray r) = 0;
	virtual void mouseDrag(Ray r, Ray pre) = 0;
	virtual void mouseClick(int button, int state, Ray r) = 0;
	virtual void mouseWheel(int wheel, int dir, Ray r) = 0;

	virtual void idle() = 0;
	virtual void rebuild() = 0;
};

class Welcome : public Layout {
private:
	Canvas canvas;
public:
	Welcome(string name);

	virtual void keyDown(int code);
	virtual void keyUp(int code);
	virtual void mouseMove(Ray r);
	virtual void mouseDrag(Ray r, Ray pre);
	virtual void mouseClick(int button, int state, Ray r);
	virtual void mouseWheel(int wheel, int dir, Ray r);
	virtual void idle();
	virtual void rebuild();
};
class Text : public Layout {
private:
	Editor editor;
public:
	Text(string name);

	virtual void keyDown(int code);
	virtual void keyUp(int code);
	virtual void mouseMove(Ray r);
	virtual void mouseDrag(Ray r, Ray pre);
	virtual void mouseClick(int button, int state, Ray r);
	virtual void mouseWheel(int wheel, int dir, Ray r);
	virtual void idle();
	virtual void rebuild();
};
class Sgs : public Layout {
private:
	Editor editor;
	//SgsMach machine;
	//Console console;
public:
	Sgs(string name);

	virtual void keyDown(int code);
	virtual void keyUp(int code);
	virtual void mouseMove(Ray r);
	virtual void mouseDrag(Ray r, Ray pre);
	virtual void mouseClick(int button, int state, Ray r);
	virtual void mouseWheel(int wheel, int dir, Ray r);
	virtual void idle();
	virtual void rebuild();
};
class Cpp : public Layout {
private:
	Editor editor;
	Gcc gcc;
public:
	Cpp(string name);

	virtual void keyDown(int code);
	virtual void keyUp(int code);
	virtual void mouseMove(Ray r);
	virtual void mouseDrag(Ray r, Ray pre);
	virtual void mouseClick(int button, int state, Ray r);
	virtual void mouseWheel(int wheel, int dir, Ray r);
	virtual void idle();
	virtual void rebuild();
};

#endif