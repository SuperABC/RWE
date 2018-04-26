#include "Window/layout/layout.h"
#include <time.h>

extern vector<Layout*> window;
extern vector<Scene*> globe;

extern Mouse *mouse;
extern Keyboard *key;

Cpp::Cpp(string name) : Layout(name), editor("config.ini") {
	type = LT_CPP;

	editor.canvas.area = Square(
		Point(-24.f, -11.5f, -33.f), Point(24.f, -11.5f, -33.f),
		Point(-24.f, -11.5f, -6.f), Point(24.f, -11.5f, -6.f),
		1280, 720);

	editor.canvas.load("Window//source//welcome//paper.obj");
	editor.refresh();
	window.canvases.push_back(&editor.canvas);

	globe.push_back(&window);
}
void Cpp::keyDown(int code) {
	if (code == 0x1B) {
		activate(false);
		::window[0]->rebuild();
	}
	if (code == RWE_KEY_F5) {
		gcc.run(gcc.compile(editor.actFile()->getPath()));
	}
}
void Cpp::keyUp(int code) {

}
void Cpp::mouseMove(Ray r) {

}
void Cpp::mouseDrag(Ray r, Ray pre) {
	if (key->ctrl)return;
	glm::vec2 pos = editor.canvas.area.intersect(r);
	editor.drag(pos);
}
void Cpp::mouseClick(int button, int state, Ray r) {
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
void Cpp::mouseWheel(int wheel, int dir, Ray r) {

}
void Cpp::idle() {
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
void Cpp::rebuild() {
	for (auto l : ::window) {
		if (l->name == name) {
			l->activate(true);
		}
		else l->active = false;
	}
}
