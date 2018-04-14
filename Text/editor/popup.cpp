#include "Text/editor/popup.h"

Popup::Popup() {

}
Popup::~Popup() {

}

void Popup::add(std::string input) {
	content.push_back(input);
}
void Popup::add(std::vector<std::string> input) {
	for (auto s : input)content.push_back(s);
}
void Popup::layout() {

}
void Popup::prepare() {
	layout();
}
void  Popup::show(int x, int y) {

}
void Popup::clear() {

}
