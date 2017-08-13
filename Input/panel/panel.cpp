#include "Frame/main/main.h"
#include "Frame/shader/shader.h"
#include "Window/scene/model.h"
#include "Window/scene/light.h"
#include "Window/scene/eye.h"
#include "Window/page/layout.h"
#include "panel.h"

extern Eye *eye;
extern vector<Layout*> window;

Mouse *mouse;
Keyboard *key;

void RWEMouse(int x, int y) {
	mouse->tmp.x = float(x);
	mouse->tmp.y = float(y);
	for (auto l : window) {
		l->mouseMove(x, y);
	}
}
void RWEDrag(int x, int y) {
	if (mouse->state[GLUT_LEFT_BUTTON] == GLUT_DOWN)
		eye->rotate(float(x - mouse->pre.x) / 200, float(y - mouse->pre.y) / 200);
	for (auto l : window) {
		l->mouseDrag(x, y, x - mouse->pre.x, y - mouse->pre.y);
	}
	mouse->pre.x = float(x);
	mouse->pre.y = float(y);
	glutPostRedisplay();
}
void RWEClick(int button, int state, int x, int y) {
	mouse->state[button] = state;
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN) {
		mouse->pre.x = float(x);
		mouse->pre.y = float(y);
	}
	for (auto l : window) {
		l->mouseClick(button, state, x, y);
	}
}
void RWEWheel(int wheel, int dir, int x, int y) {
	if (dir > 0)eye->zoom(.8f);
	if (dir < 0)eye->zoom(1.2f);
	for (auto l : window) {
		l->mouseClick(wheel, dir, x, y);
	}
	glutPostRedisplay();
}
void RWESpecialDown(int key, int x, int y) {
	for (auto l : window) {
		l->keyDown(key);
	}
}
void RWESpecialUp(int key, int x, int y) {
	for (auto l : window) {
		l->keyUp(key);
	}
}
void RWEKeyDown(unsigned char cAscii, int x, int y) {
	for (auto l : window) {
		l->keyDown(cAscii);
	}
}
void RWEKeyUp(unsigned char cAscii, int x, int y) {
	for (auto l : window) {
		l->keyUp(cAscii);
	}
}

