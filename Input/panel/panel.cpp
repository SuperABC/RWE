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
		l->mouseMove(eye->ray(x, y));
	}
	glutPostRedisplay();
}
void RWEDrag(int x, int y) {
	if (mouse->state[GLUT_LEFT_BUTTON] == GLUT_DOWN)
		eye->rotate(float(x - mouse->pre.x) / 200, float(y - mouse->pre.y) / 200);
	if (mouse->state[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
		eye->move(float(x - mouse->pre.x)/2, float(y - mouse->pre.y)/2);
	for (auto l : window) {
		l->mouseDrag(eye->ray(x, y), eye->ray((int)mouse->pre.x, (int)mouse->pre.y));
	}
	mouse->pre.x = float(x);
	mouse->pre.y = float(y);
	glutPostRedisplay();
}
void RWEClick(int button, int state, int x, int y) {
	mouse->state[button] = state;
	mouse->pre.x = float(x);
	mouse->pre.y = float(y);
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN) {
		mouse->state[GLUT_LEFT_BUTTON] = GLUT_DOWN;
	}
	for (unsigned int i = 0; i < window.size(); i++) {
		window[i]->mouseClick(button, state, eye->ray(x, y));
	}
	glutPostRedisplay();
}
void RWEWheel(int wheel, int dir, int x, int y) {
	if (dir > 0)eye->zoom(.8f);
	if (dir < 0)eye->zoom(1.2f);
	for (auto l : window) {
		l->mouseClick(wheel, dir, eye->ray(x, y));
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


