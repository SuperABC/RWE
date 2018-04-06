#include "Text/editor/file.h"

File::File() : filename("new file"), format(""), next(NULL), prev(NULL) {
	content = new Block();
	start = 0;
	total = 1;
	focus = 0;
	selectBegin = 0;
	selectEnd = 0;
}
File::File(char *filename) : filename(filename),
	content(NULL), next(NULL), prev(NULL) {
	string tmp;
	vector<string> whole;

	if (this->filename.rfind('.') == -1)format = string();
	else format = string(this->filename.begin() + this->filename.rfind('.') + 1, this->filename.end());

	std::ifstream fin(filename);

	if (!fin.is_open())return;

	while (!fin.eof()) {
		getline(fin, tmp);
		whole.push_back(tmp);
	}

	content = new Block(whole, total);
	start = 0;
	focus = 0;
	selectBegin = 0;
	selectEnd = 0;
	fin.close();
};
File::~File() {
	delete content;
}

string File::getPath() {
	return filename;
}
string File::getDir() {
	int end = filename.rfind('/');
	if (end == -1)
		end = filename.rfind('\\');
	if (end == -1)return string();
	else return string(filename.begin(), filename.begin() + end);
}
string File::getName() {
	int end = filename.rfind('/');
	if(end == -1)
		end = filename.rfind('\\');
	if (end == -1)return filename;
	else return string(filename.begin() + end + 1, filename.end());
}
string File::getFormat() {
	return format;
}

Block *File::startBlock() {
	Block *tmpBlock = content;
	for (int i = 0; i < start; i++)tmpBlock = tmpBlock->next;
	return tmpBlock;
}
Block *File::focusBlock() {
	Block *tmpBlock = content;
	for (int i = 0; i < focus; i++)tmpBlock = tmpBlock->next;
	return tmpBlock;

}
Block *File::blockAt(int n){
	Block *tmpBlock = content;
	for (int i = 0; i < n; i++)tmpBlock = tmpBlock->next;
	return tmpBlock;
}
int File::totalLine() {
	Block *tmpBlock = content;
	int line = 0;
	while (tmpBlock) {
		line += tmpBlock->total;
		tmpBlock = tmpBlock->next;
	}
	return line;
}
int File::startLine() {
	Block *tmpBlock = content;
	int line = 0;
	for (int i = 0; i < start; i++) {
		line += tmpBlock->total;
		tmpBlock = tmpBlock->next;
	}
	line += tmpBlock->start;
	return line;
}
void File::setStart(int line) {
	Block *tmpBlock = content;
	int block = 0;
	while(line >= tmpBlock->total) {
		line -= tmpBlock->total;
		block++;
		tmpBlock = tmpBlock->next;
	}
	start = block;
	startBlock()->start = line;
}
