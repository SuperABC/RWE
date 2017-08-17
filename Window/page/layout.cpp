#include "layout.h"

extern vector<Layout*> window;
extern vector<Scene*> globe;

void Layout::registerPage() {
	for (auto l : ::window) {
		l->active = false;
	}
	this->active = true;
	::window.push_back(this);
}

Welcome::Welcome() {
	type = LT_WELCOME;
	window.elements.push_back(*Element().load("Window//src//welcome//origin.obj"));
	window.elements.push_back(*Element().load("Window//src//welcome//table.obj"));
	window.textures.push_back(*Texture(true,
		glm::vec3(.4f, .4f, .4f), glm::vec3(.8f, .8f, .8f), 
		0.f).load("Window//src//welcome//phone.obj"));
	window.textures.push_back(*Texture().load("Window//src//welcome//surf.obj"));
	globe.push_back(&window);
	
	area = Square(
		Point(-8.19f, 11.80f, 1.15f), Point(9.57f, 11.80f, 1.15f),
		Point(9.57f, -16.44f, 1.15f), Point(-8.19f, -16.44f, 1.15f),
		180, 320);
}
void Welcome::keyDown(int code) {

}
void Welcome::keyUp(int code) {

}
void Welcome::mouseMove(Ray r) {

}
void Welcome::mouseDrag(Ray r, Ray pre) {

}
void Welcome::mouseClick(int button, int state, Ray r) {

}
void Welcome::mouseWheel(int wheel, int dir, Ray r) {

}
void Welcome::idle() {

}

Compiler::Compiler() {
	type = LT_COMPILER;
}
void Compiler::keyDown(int code) {

}
void Compiler::keyUp(int code) {

}
void Compiler::mouseMove(Ray r) {

}
void Compiler::mouseDrag(Ray r, Ray pre) {

}
void Compiler::mouseClick(int button, int state, Ray r) {

}
void Compiler::mouseWheel(int wheel, int dir, Ray r) {

}
void Compiler::idle() {

}

Sgs::Sgs() {
	type = LT_SGS;
}
void Sgs::keyDown(int code) {

}
void Sgs::keyUp(int code) {

}
void Sgs::mouseMove(Ray r) {

}
void Sgs::mouseDrag(Ray r, Ray pre) {

}
void Sgs::mouseClick(int button, int state, Ray r) {

}
void Sgs::mouseWheel(int wheel, int dir, Ray r) {

}
void Sgs::idle() {

}