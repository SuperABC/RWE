#ifndef RWE_COMPILER_TEXT_BLOCK
#define RWE_COMPILER_TEXT_BLOCK
#include "Frame/main/main.h"

#define DEFAULT_FONT 0
#define DEFAULT_SIZE 20

#define BLOCK_SIZE 4096
struct Character {
	int colorR, colorG, colorB;
	int fontId;
	int size;
	char ch;
};
class Block {
private:
	vector<Character> content;
	int startPos, endPos;

public:
	Block *next;

	Block();
	~Block();

	string get();
	int length();
	Block *load(const char *filename, int length, int start = 0);
	Block *add(int pos, char ch);
	Block *del(int pos);
	Block *operator[] (int n);

	vector<Character> output(int start, int end);
};

#endif