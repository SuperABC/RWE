#ifndef RWE_BUILDER_GENERATOR_WIDGET
#define RWE_BUILDER_GENERATOR_WIDGET
#include "Frame/main/main.h"

typedef void(*mouseMoveCall)(void *w, int x, int y);
typedef void(*mouseClickCall)(void *w, int x, int y, int status);
typedef void(*keyCall)(void *w, int key);
typedef struct {
	int sizeX, sizeY;
	unsigned char *data;
}bitMap;

enum WIDGETTYPE {
	WT_BUTTON,
	WT_LABEL,
	WT_INPUT,
	WT_OUTPUT,
	WT_LIST,
	WT_CHECK,
	WT_DIALOG,
	WT_PROGRESS,
	WT_OPTION
};

class Widget {
private:
	int posX, posY;
	int sizeX, sizeY;

	int visible;
	int priority;
	int status;
	int style;

	int hide;
	int value;
	string name;
	string content;
	string helpMessage;

	bitMap *cover;
	Widget *associate;

	mouseMoveCall mouseIn, mouseOut;
	mouseClickCall mouseDown, mouseUp, mouseClick;
	keyCall keyDown, keyUp, keyPress;
public:
	WIDGETTYPE type;

	Widget();
	~Widget();

	string SGLlayout();
	void show();
};

#endif