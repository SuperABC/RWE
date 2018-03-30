#include "layout.h"

extern vector<Layout*> window;
extern vector<Scene*> globe;

extern Mouse *mouse;

Welcome::Welcome() {
	type = LT_WELCOME;
	window.elements.push_back((new Element())->load("Window//source//welcome//table.obj"));
	window.elements.push_back((new Element())->load("Window//source//welcome//origin.obj"));
	window.textures.push_back((new Texture())->load("Window//source//welcome//ground.obj"));
	window.textures.push_back((new Texture())->load("Window//source//welcome//surf.obj"));

	canvas.load("Window//source//welcome//phone.obj");
	canvas.area = Square(
		Point(-8.19f, 11.80f, 1.15f), Point(9.57f, 11.80f, 1.15f),
		Point(-8.19f, -16.44f, 1.15f), Point(9.57f, -16.44f, 1.15f),
		1080, 1920);
	window.canvases.push_back(&canvas);

	globe.push_back(&window);

}
void Welcome::keyDown(int code) {

}
void Welcome::keyUp(int code) {

}
void Welcome::mouseMove(Ray r) {
	if (::window.size() > 1)return;
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
		if (pos.x >= 0 && pos.x < 1080 && pos.y >= 470 && pos.y < 590) {
			window.elements.pop_back();
			window.textures.pop_back();
			canvas.area = Square();
			(new Text)->registerPage();
			window.textures.push_back((new Texture())->load("Window/src/welcome/paper.obj"));
		}
		if (pos.x >= 0 && pos.x < 1080 && pos.y >= 600 && pos.y < 720) {
			window.elements.pop_back();
			window.textures.pop_back();
			canvas.area = Square();
			(new Sgs)->registerPage();
			window.textures.push_back((new Texture())->load("Window/src/welcome/paper.obj"));
		}
	}
}
void Welcome::mouseWheel(int wheel, int dir, Ray r) {

}
void Welcome::idle() {

}
