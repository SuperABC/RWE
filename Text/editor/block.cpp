#include "Text/editor/block.h"

Block::Block(string str): next(NULL), prev(NULL) {
	content = new Line(str);
	size = 0;
	start = 0;
	total = 1;
	focus = 0;
	selectBegin = 0;
	selectEnd = 0;
}
Block::Block(vector<string> &cont, int &count, int offset) :
	content(NULL), next(NULL), prev(NULL) {
	int start = offset, end = offset;
	int length = 0;

	if (count == 0) {
		focus = 0;
		selectBegin = 0;
		selectEnd = 0;
	}
	count++;

	while (length < BLOCK_SIZE / 2 && end < (int)cont.size()) {
		length += cont[end++].length();
	}

	size = length;

	vector<string> line(cont.begin() + start, cont.begin() + end);
	content = new Line(line, total);
	if (end < (int)cont.size()) {
		next = new Block(cont, count, end);
		next->prev = this;
	}

	start = 0;
};
Block::~Block() {
	if (next)delete next;
	delete content;
}

Line *Block::startLine() {
	Line *tmpLine = content;
	for (int i = 0; i < start; i++)tmpLine = tmpLine->next;
	return tmpLine;
}
Line *Block::focusLine() {
	Line *tmpLine = content;
	for (int i = 0; i < focus; i++)tmpLine = tmpLine->next;
	return tmpLine;
}
Line *Block::lineAt(int n){
	Line *tmpLine = content;
	for (int i = 0; i < n; i++)tmpLine = tmpLine->next;
	return tmpLine;
}
