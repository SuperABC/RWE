#include "Window/layout/layout.h"

extern vector<Layout*> window;
extern vector<Scene*> globe;

extern Mouse *mouse;

void Layout::registerPage() {
	for (auto l : ::window) {
		l->active = false;
	}
	this->active = true;
	::window.push_back(this);
}
void Layout::destroy() {

}
