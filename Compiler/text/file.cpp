#include "file.h"

File::File() {
	type = FT_TXT;
	lineHeight = DEFAULT_SIZE;
	focus = 0;
}
File::~File() {

}

void File::load(string filename) {
	std::ifstream fin(filename, std::ios::in);
	fin.seekg(0, std::ios::end);
	int len = fin.tellg();
	fin.close();

	text = new Block->load(filename.c_str(), len, 0);
}
void File::save(string filename) {
	std::ofstream fout(filename, std::ios::out);

	Block *tmp = text;
	while (tmp != NULL) {
		fout.write(tmp->get().c_str(), tmp->length());
		tmp = tmp->next;
	}
	fout.close();
}
void File::add(int pos, char ch) {
	text->add(pos, ch);
}
void File::del(int pos) {
	text->del(pos);
}
