#include "block.h"

Block::Block() {
	startPos = endPos = 0;
	next = NULL;
}
Block::~Block() {

}

string Block::get() {
	string ret;
	for (int i = 0; content[i].ch != 0; i++) {
		ret += content[i].ch;
	}
	return ret;
}
int Block::length() {
	return endPos - startPos;
}
Block *Block::load(const char *filename, int length, int start) {
	std::ifstream fin(filename, std::ios::in);
	char buf[BLOCK_SIZE / 2] = { 0 };
	fin.seekg(start);
	fin.read(buf, BLOCK_SIZE/2);
	fin.close();

	for (int i = 0; buf[i] != 0 && i < BLOCK_SIZE / 2; i++) {
		content.push_back(Character());
		content[i].ch = buf[i];
		content[i].colorR = content[i].colorG = content[i].colorB = 1;
		content[i].fontId = DEFAULT_FONT;
		content[i].size = DEFAULT_SIZE;
		endPos = i + 1;
	}

	if (length - start > BLOCK_SIZE / 2)
		next = (new Block())->load(filename, length, start + BLOCK_SIZE / 2);
	return this;
}
Block *Block::add(int pos, char ch) {
	if (pos > endPos)
		return next->add(pos-endPos, ch);
	if (endPos >= BLOCK_SIZE) {
		Block *tmp = next;
		next = new Block();
		next->next = tmp;
		next->add(0, content[BLOCK_SIZE].ch);
	}
	content.push_back(Character());
	for (int i = endPos < BLOCK_SIZE ? endPos : BLOCK_SIZE - 1; i > pos; i--) {
		content[i] = content[i - 1];
	}
	content[pos].ch = ch;
	content[pos].colorR = content[pos].colorG = content[pos].colorB = 1;
	content[pos].fontId = DEFAULT_FONT;
	content[pos].size = DEFAULT_SIZE;

	return this;
}
Block *Block::del(int pos) {
	if (pos == 0)return;
	if (pos > endPos)return next->del(pos - endPos);
	if (pos == endPos)content.pop_back();
	for (int i = pos; i < endPos; i++) {
		content[i - 1] = content[i];
	}
	return this;
}
Block *Block::operator[] (int n) {
	if (n == 0)return this;
	if (n == 1)return next;
	if (next == NULL)return NULL;
	return (*next)[n - 1];
}

vector<Character> Block::output(int start, int end) {
	return content;
}
