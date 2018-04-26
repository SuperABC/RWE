#include "Window/layout/layout.h"
#include "Window/scene/bitmap.h"
#include "Window/scene/eye.h"
#include <time.h>

extern vector<Layout*> window;
extern vector<Scene*> globe;

extern Eye *eye;
extern Mouse *mouse;
extern Keyboard *key;

Text::Text(string name) : Layout(name), editor("config.ini") {
	type = LT_TEXT;

	editor.canvas.area = Square(
		Point(-24.f, -11.5f, -33.f), Point(24.f, -11.5f, -33.f),
		Point(-24.f, -11.5f, -6.f), Point(24.f, -11.5f, -6.f),
		1280, 720);

	editor.canvas.load("Window//source//welcome//paper.obj");
	editor.refresh();
	window.canvases.push_back(&editor.canvas);
	
	globe.push_back(&window);
}
void Text::keyDown(int code) {
	if (code == 0x1B) {
		activate(false);
		::window[0]->rebuild();
	}
	editor.key(code);
}
void Text::keyUp(int code) {

}
void Text::mouseMove(Ray r) {

}
void Text::mouseDrag(Ray r, Ray pre) {
	if (key->ctrl)return;
	glm::vec2 pos = editor.canvas.area.intersect(r);
	editor.drag(pos);
}
void Text::mouseClick(int button, int state, Ray r) {
	int rwes = 0;
	if (key->ctrl)return;
	glm::vec2 pos = editor.canvas.area.intersect(r);
	if (button == GLUT_LEFT_BUTTON) rwes |= SG_LEFT_BUTTON;
	if (button == GLUT_MIDDLE_BUTTON) rwes |= SG_MIDDLE_BUTTON;
	if (button == GLUT_RIGHT_BUTTON) rwes |= SG_RIGHT_BUTTON;
	if (state == GLUT_DOWN) rwes |= SG_BUTTON_DOWN;
	if (state == GLUT_UP) rwes |= SG_BUTTON_UP;
	editor.mouse(glm::vec3(pos, rwes));
}
void Text::mouseWheel(int wheel, int dir, Ray r) {

}
void Text::idle() {
	static int time = clock();
	if ((clock() - time) % 800 < 400) {
		editor.canvas.setColor(editor.cursorColor.r, editor.cursorColor.g, editor.cursorColor.b);
		editor.canvas.putQuad((int)editor.editorBase.x + (int)editor.cursorPos.x - 1,
			(int)editor.editorBase.y + (int)editor.cursorPos.y + 1,
			(int)editor.editorBase.x + (int)editor.cursorPos.x,
			(int)editor.editorBase.y + (int)editor.cursorPos.y + editor.fontSize + 4 - 4,
			Screen::SOLID_FILL);
	}
	else {
		editor.canvas.setColor(64, 64, 64);
		editor.canvas.putQuad((int)editor.editorBase.x + (int)editor.cursorPos.x - 1,
			(int)editor.editorBase.y + (int)editor.cursorPos.y + 1,
			(int)editor.editorBase.x + (int)editor.cursorPos.x,
			(int)editor.editorBase.y + (int)editor.cursorPos.y + editor.fontSize + 4 - 4,
			Screen::SOLID_FILL);
	}
}
void Text::rebuild() {
	for (auto l : ::window) {
		if (l->name == name) {
			l->activate(true);
		}
		else l->active = false;
	}

	eye->animate(glm::vec3(1.7f, 14.6f, -9.7f), glm::vec3(0.0f, -14.6f, -4.f));
}
