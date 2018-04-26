#include "Window/layout/layout.h"
#include "Window/scene/eye.h"

extern vector<Layout*> window;
extern vector<Scene*> globe;

extern Mouse *mouse;
extern Eye *eye;

Welcome::Welcome(string name) : Layout(name) {
	type = LT_WELCOME;

	canvas.area = Square(
		Point(-8.19f, 11.80f, 1.15f), Point(9.57f, 11.80f, 1.15f),
		Point(-8.19f, -16.44f, 1.15f), Point(9.57f, -16.44f, 1.15f),
		1080, 1920);

	window.elements.push_back((new Element())->load("Window//source//welcome//table.obj"));
	window.elements.push_back((new Element())->load("Window//source//welcome//origin.obj"));
	window.textures.push_back((new Texture())->load("Window//source//welcome//ground.obj"));
	window.textures.push_back((new Texture())->load("Window//source//welcome//surf.obj"));

	canvas.load("Window//source//welcome//phone.obj");
	window.canvases.push_back(&canvas);

	globe.push_back(&window);
}
void Welcome::keyDown(int code) {

}
void Welcome::keyUp(int code) {

}
void Welcome::mouseMove(Ray r) {
	glm::vec2 pos = canvas.area.intersect(r);
	if (pos.x >= 930 && pos.x < 1040 && pos.y >= 80 && pos.y < 140) {
		canvas.loadBmp(930, 80, "Window/source/welcome/quit.bmp");
	}
	else {
		canvas.loadBmp(930, 80, "Window/source/welcome/quitback.bmp");
	}
}
void Welcome::mouseDrag(Ray r, Ray pre) {

}
void Welcome::mouseClick(int button, int state, Ray r) {
	glm::vec2 pos = canvas.area.intersect(r);
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP
		&&mouse->pre.x == mouse->tmp.x&&mouse->pre.y == mouse->tmp.y) {
		if (pos.x >= 930 && pos.x < 1040 && pos.y >= 80 && pos.y < 140)exit(0);
		if (pos.x >= 0 && pos.x < 1080 && pos.y >= 480 && pos.y < 600) {
			window.elements.pop_back();
			window.canvases.pop_back();
			for (auto l : ::window) {
				if (l->name == "Text") {
					l->rebuild();
					return;
				}
			}
			(new Text("Text"))->registerPage();
			eye->animate(glm::vec3(1.7f, 14.6f, -9.7f), glm::vec3(0.0f, -14.6f, -4.f));
		}
		if (pos.x >= 0 && pos.x < 1080 && pos.y >= 600 && pos.y < 720) {
			window.elements.pop_back();
			window.canvases.pop_back();
			//(new Text("Text"))->registerPage();
			(new Sgs("Sgs"))->registerPage();
		}
		if (pos.x >= 0 && pos.x < 1080 && pos.y >= 720 && pos.y < 840) {
			window.elements.pop_back();
			window.canvases.pop_back();
			//(new Text("Text"))->registerPage();
			(new Cpp("Cpp"))->registerPage();
		}
	}
}
void Welcome::mouseWheel(int wheel, int dir, Ray r) {

}

void Welcome::idle() {

}
void Welcome::rebuild() {
	active = true;
	window.elements.push_back((new Element())->load("Window//source//welcome//origin.obj"));
	window.canvases.push_back(&canvas);

	eye->animate(glm::vec3(0.f, 0.f, 32.f), glm::vec3(0.f, 0.f, -32.f));
}
