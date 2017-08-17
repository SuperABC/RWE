#ifndef RWE_COMPILER_MACHINE_BITMAP
#define RWE_COMPILER_MACHINE_BITMAP
#include "Frame/main/main.h"

typedef struct {
	unsigned char r, g, b;
}RGB;

class Bitmap {
public:
	int sizeX, sizeY;
	unsigned char *data;
};

class Screen : public Bitmap {
public:
	void setColor(int r, int g, int b);
	void clearScreen();
	int putPixel(int x, int y);
	RGB getPixel(int x, int y);
	void putLine(int x1, int y1, int x2, int y2, int mode);
	void putQuad(int x1, int y1, int x2, int y2, int mode);
	void putTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int mode);
	void putCircle(int xc, int yc, int r, int mode);
	void putEllipse(int xc, int yc, int a, int b, int mode);
	int loadBmp(int x, int y, char *filename);
	void putNumber(int n, int x, int y, char lr);
	void putChar(char ch, int x, int y);
	void putChinese(unsigned char *ch, int x, int y);
	void putString(unsigned char *str, int x, int y);
	int putStringConstraint(unsigned char *str, int x, int y, int constraint);
	int getImage(int left, int top, int right, int bottom, Bitmap *bitmap);
	void putImage(int left, int top, Bitmap *bitmap, int op);
	int maskImage(int left, int top, Bitmap *mask, Bitmap *bitmap);
	void funcMap(int x1, int x2, int y1, int y2, float(*vect)(float x));
	void floodFill(int x, int y, RGB c);
};

#endif