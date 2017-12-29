#include "layout.h"

extern vector<Layout*> window;
extern vector<Scene*> globe;

extern Mouse *mouse;

Welcome::Welcome() {
	type = LT_WELCOME;
	window.elements.push_back(*Element().load("Window//src//welcome//table.obj"));
	window.elements.push_back(*Element().load("Window//src//welcome//origin.obj"));
	window.textures.push_back(*Texture().load("Window//src//welcome//ground.obj"));
	window.textures.push_back(*Texture().load("Window//src//welcome//surf.obj"));
	window.textures.push_back(*Texture(true,
		glm::vec3(.4f, .4f, .4f), glm::vec3(.8f, .8f, .8f),
		0.f).load("Window//src//welcome//phone.obj"));
	globe.push_back(&window);

	area = Square(
		Point(-8.19f, 11.80f, 1.15f), Point(9.57f, 11.80f, 1.15f),
		Point(-8.19f, -16.44f, 1.15f), Point(9.57f, -16.44f, 1.15f),
		1080, 1920);
}
void Welcome::keyDown(int code) {

}
void Welcome::keyUp(int code) {

}
void Welcome::mouseMove(Ray r) {
	if (::window.size() > 1)return;
	glm::vec2 pos = area.intersect(r);
	if (pos.x >= 930 && pos.x < 1040 && pos.y >= 80 && pos.y < 140) {
		window.textures[window.textures.size() - 1].src.loadBmp(930, 80, "Window/src/welcome/quit.bmp");
	}
	else {
		window.textures[window.textures.size() - 1].src.loadBmp(930, 80, "Window/src/welcome/quitback.bmp");
	}
}
void Welcome::mouseDrag(Ray r, Ray pre) {

}
void Welcome::mouseClick(int button, int state, Ray r) {
	glm::vec2 pos = area.intersect(r);
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_UP
		&&mouse->pre.x == mouse->tmp.x&&mouse->pre.y == mouse->tmp.y) {
		if (pos.x >= 930 && pos.x < 1040 && pos.y >= 80 && pos.y < 140)exit(0);
		if (pos.x >= 0 && pos.x < 1080 && pos.y >= 470 && pos.y < 590) {
			window.elements.pop_back();
			window.textures.pop_back();
			area = Square();
			(new Compiler)->registerPage();
			window.textures.push_back(*Texture().load("Window/src/welcome/paper.obj"));
		}
		if (pos.x >= 0 && pos.x < 1080 && pos.y >= 600 && pos.y < 720) {
			window.elements.pop_back();
			window.textures.pop_back();
			area = Square();
			(new Sgs)->registerPage();
			window.textures.push_back(*Texture().load("Window/src/welcome/paper.obj"));
		}
	}
}
void Welcome::mouseWheel(int wheel, int dir, Ray r) {

}
void Welcome::idle() {

}
