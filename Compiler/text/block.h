#ifndef RWE_EDITOR_TEXT_BLOCK
#define RWE_EDITOR_TEXT_BLOCK
#include "Frame/main/main.h"

#define BLOCK_SIZE 4096
struct Character {
	int colorR, colorG, colorB;
	int fontId;
	int size;
	char ch;
};
class Block {
private:
	char content[BLOCK_SIZE];
	int startPos, endPos;
public:
	Block();
	~Block();
	void load(int start);
	vector<Character> output(int start, int end);
};

#endif