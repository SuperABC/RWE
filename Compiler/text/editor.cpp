#include "editor.h"

Editor::Editor() {
	active = -1;
}
Editor::Editor(Square s) {
	active = -1;
	area = s;
}

