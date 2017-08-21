#include "bitmap.h"

void Screen::setColor(int r, int g, int b) {
	rgb[0] = r % 256;
	rgb[1] = g % 256;
	rgb[2] = b % 256;
}
void Screen::clearScreen() {
	int i;

	if ((rgb[0] & rgb[1] & rgb[2]) == 255) {
		memset(data, -1, 3 * sizeX*sizeY);
		return;
	}
	if ((rgb[0] | rgb[1] | rgb[2]) == 0) {
		memset(data, 0, 3 * sizeX*sizeY);
		return;
	}

	for (i = 0; i < 3 * sizeX*sizeY; i += 3) {
		data[i] = rgb[2];
		data[i + 1] = rgb[1];
		data[i + 2] = rgb[0];
	}
}
int Screen::putPixel(int x, int y) {
	int p;

	y = sizeY - 1 - y;
	if (x < 0 || x >= sizeX || y < 0 || y >= sizeY)return -1;

	p = (y * sizeX + x) * 3;
	data[p] = rgb[2];
	data[p + 1] = rgb[1];
	data[p + 2] = rgb[0];

	return 0;
}
RGB Screen::getPixel(int x, int y) {
	RGB ret;
	int p;

	y = sizeY - 1 - y;
	ret.r = ret.g = ret.b = 0;

	if (x < 0 || x >= sizeX || y < 0 || y >= sizeY)return ret;
	p = (y*sizeX + x) * 3;
	ret.b = data[p++];
	ret.g = data[p++];
	ret.r = data[p++];

	return ret;
}
void Screen::putLine(int x1, int y1, int x2, int y2) {
	int dx, dy, ux, uy, x, y, eps;

	y1 = sizeY - 1 - y1;
	y2 = sizeY - 1 - y2;
#define ABS(x) ((x > 0) ? (x) : (-x))

	dx = x2 - x1;
	dy = y2 - y1;

	ux = ((dx > 0) << 1) - 1;
	uy = ((dy > 0) << 1) - 1;

	x = x1;
	y = y1;

	eps = 0;
	dx = ABS(dx);
	dy = ABS(dy);

	if (dx > dy) {
		for (x = x1; x != x2 + ux; x += ux) {
			putPixel(x, sizeY - 1 - y);
			eps += dy;
			if ((eps << 1) >= dx) {
				y += uy;
				eps -= dx;
			}
		}
	}
	else {
		for (y = y1; y != y2 + uy; y += uy) {
			putPixel(x, sizeY - 1 - y);
			eps += dx;
			if ((eps << 1) >= dy) {
				x += ux;
				eps -= dy;
			}
		}
	}
#undef ABS
}
void Screen::putQuad(int x1, int y1, int x2, int y2, int mode) {
	int i, j;
	y1 = sizeY - 1 - y1;
	y2 = sizeY - 1 - y2;

	if (mode == SOLID_FILL)
		for (i = x1; i <= x2; i++)
			for (j = y1; j <= y2; j++)
				putPixel(i, sizeY - 1 - j);

	if (mode == EMPTY_FILL) {
		for (i = x1; i <= x2; i++) {
			putPixel(i, sizeY - 1 - y1);
			putPixel(i, sizeY - 1 - y2);
		}
		for (j = y1; j <= y2; j++) {
			putPixel(x1, sizeY - 1 - j);
			putPixel(x2, sizeY - 1 - j);
		}
	}
}
void Screen::putCircle(int xc, int yc, int r, int mode) {
	int x, y, yi, d;
	yc = sizeY - 1 - yc;

	x = 0;
	y = r;
	d = 3 - 2 * r;

	if (mode == SOLID_FILL) {
		while (x <= y) {
			for (yi = x; yi <= y; yi++) {
				putPixel(xc + x, sizeY - 1 - (yc + yi));
				putPixel(xc + x, sizeY - 1 - (yc - yi));
				putPixel(xc - x, sizeY - 1 - (yc - yi));
				putPixel(xc - x, sizeY - 1 - (yc + yi));
				putPixel(xc + yi, sizeY - 1 - (yc + x));
				putPixel(xc + yi, sizeY - 1 - (yc - x));
				putPixel(xc - yi, sizeY - 1 - (yc - x));
				putPixel(xc - yi, sizeY - 1 - (yc + x));
			}

			if (d < 0)d = d + 4 * x + 6;
			else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
	if (mode == EMPTY_FILL) {
		while (x <= y) {
			putPixel(xc + x, sizeY - 1 - (yc + y));
			putPixel(xc + x, sizeY - 1 - (yc - y));
			putPixel(xc - x, sizeY - 1 - (yc - y));
			putPixel(xc - x, sizeY - 1 - (yc + y));
			putPixel(xc + y, sizeY - 1 - (yc + x));
			putPixel(xc + y, sizeY - 1 - (yc - x));
			putPixel(xc - y, sizeY - 1 - (yc - x));
			putPixel(xc - y, sizeY - 1 - (yc + x));

			if (d < 0)d = d + 4 * x + 6;
			else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}
void Screen::putEllipse(int xc, int yc, int a, int b, int mode) {
	int sqa, sqb, x, y, d, P_x, xi;
	yc = sizeY - 1 - yc;

	sqa = a * a;
	sqb = b * b;
	x = 0;
	y = b;
	d = 2 * sqb - 2 * b * sqa + sqa;
	P_x = (int)((double)sqa / sqrt((double)(sqa + sqb)));

	putPixel(xc + x, sizeY - 1 - (yc + y));
	putPixel(xc - x, sizeY - 1 - (yc - y));

	if (mode == SOLID_FILL) {
		while (x <= P_x) {
			if (d < 0) d += 2 * sqb * (2 * x + 3);
			else {
				d += 2 * sqb * (2 * x + 3) - 4 * sqa * (y - 1);
				y--;
			}
			x++;

			for (xi = 0; xi <= x; xi++) {
				putPixel(xc + xi, sizeY - 1 - (yc + y));
				putPixel(xc - xi, sizeY - 1 - (yc + y));
				putPixel(xc + xi, sizeY - 1 - (yc - y));
				putPixel(xc - xi, sizeY - 1 - (yc - y));
			}
		}
		d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
		while (y >= 0) {
			for (xi = 0; xi <= x; xi++) {
				putPixel(xc + xi, sizeY - 1 - (yc + y));
				putPixel(xc - xi, sizeY - 1 - (yc + y));
				putPixel(xc + xi, sizeY - 1 - (yc - y));
				putPixel(xc - xi, sizeY - 1 - (yc - y));
			}
			y--;

			if (d < 0) {
				x++;
				d = d - 2 * sqa * y - sqa + 2 * sqb * x + 2 * sqb;
			}
			else d = d - 2 * sqa * y - sqa;
		}
	}
	if (mode == EMPTY_FILL) {
		while (x <= P_x) {
			if (d < 0) d += 2 * sqb * (2 * x + 3);
			else {
				d += 2 * sqb * (2 * x + 3) - 4 * sqa * (y - 1);
				y--;
			}
			x++;
			putPixel(xc + x, sizeY - 1 - (yc + y));
			putPixel(xc - x, sizeY - 1 - (yc + y));
			putPixel(xc + x, sizeY - 1 - (yc - y));
			putPixel(xc - x, sizeY - 1 - (yc - y));
		}
		d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
		while (y >= 0) {
			putPixel(xc + x, sizeY - 1 - (yc + y));
			putPixel(xc - x, sizeY - 1 - (yc + y));
			putPixel(xc + x, sizeY - 1 - (yc - y));
			putPixel(xc - x, sizeY - 1 - (yc - y));
			y--;
			if (d < 0) {
				x++;
				d = d - 2 * sqa * y - sqa + 2 * sqb * x + 2 * sqb;
			}
			else d = d - 2 * sqa * y - sqa;
		}
	}
}
int Screen::loadBmp(int x, int y, char *filename) {
	FILE *fp = NULL;
	byte *p = NULL, *tmp = NULL;
	int width, height, i;
	GLuint dataOffset, lineBytes;
	GLuint lines;
	byte *vp;
	char buffer = 0;

	y = sizeY - 1 - y;
	vp = data + (y*sizeX + x) * 3;

	if (x < 0 || y < 0)goto displayError;
	p = (unsigned char *)malloc(2048 * 3 * sizeof(char));
	if (p == NULL)goto displayError;
	fp = fopen(filename, "rb");
	if (fp == NULL)goto displayError;

	fread(p, 1, 0x36, fp);
	if (*(unsigned short *)p != 0x4D42)goto displayError;
	if (*(unsigned short *)(p + 0x1C) != 24)goto displayError;

	width = *(GLuint *)(p + 0x12);
	height = *(GLuint *)(p + 0x16);
	dataOffset = *(GLuint *)(p + 0x0A);
	lineBytes = (width * 3 + 3) / 4 * 4;

	if (y + height > (int)sizeY) {
		fseek(fp, dataOffset + (y + height - sizeY)*lineBytes, SEEK_SET);
		height = sizeY - y;
	}
	else fseek(fp, dataOffset, SEEK_SET);

	for (i = height - 1; i >= 0; i--) {
		fread(p, 1, lineBytes, fp);
		lines = i * sizeX * 3;
		tmp = p;
		if (x + width > (int)sizeX)
			memcpy(vp - lines, p, (sizeX - x) * 3);
		else memcpy(vp - lines, p, width * 3);
	}

	free(p);
	fclose(fp);
	return 0;

displayError:
	if (p != NULL)free(p);
	if (fp != NULL)fclose(fp);
	return -1;
}
void Screen::putNumber(int n, int x, int y, char lr) {
	int s[20], sn = 0;

	y = sizeY - 1 - y;
	if (lr == 'l') {
		if (n == 0)putChar('0', x, sizeY - 1 - y);
		while (n > 0) {
			s[sn++] = n % 10;
			n /= 10;
		}

		while (sn > 0) {
			putChar((char)(s[--sn] + '0'), x, sizeY - 1 - y);
			x += 8;
		}
	}
	else if (lr == 'r') {
		if (n == 0)putChar('0', x - 8, sizeY - 1 - y);
		while (n > 0) {
			x -= 8;
			putChar((char)(n % 10 + '0'), x, sizeY - 1 - y);
			n /= 10;
		}
	}
}
void Screen::putChar(char ch, int x, int y) {
	int j, k;

	y = sizeY - 1 - y;
	for (j = 0; j < 8; j++)
		for (k = 7; k >= 0; k--)
			if ((letters[ch][j] >> k) & 1) {
				putPixel(x + 7 - k, sizeY - 1 - (y + 2 * j));
				putPixel(x + 7 - k, sizeY - 1 - (y + 2 * j + 1));
			}
}
void Screen::putString(char *str, int x, int y) {
	int len, i, j, k, xBegin;

	y = sizeY - 1 - y;
	if (str == NULL)return;

	xBegin = x;
	len = strlen(str);
	for (i = 0; i < len; i++) {
		if (str[i] == '\n') {
			y += SG_LINE_DELTA_DEFAULT;
			x = xBegin;
			continue;
		}
		for (j = 0; j < 8; j++)
			for (k = 7; k >= 0; k--)
				if ((letters[str[i]][j] >> k) & 1) {
					putPixel(x + 7 - k, sizeY - 1 - (y + 2 * j));
					putPixel(x + 7 - k, sizeY - 1 - (y + 2 * j + 1));
				}
		x += SG_CHAR_WIDTH;
	}
}
int Screen::putStringConstraint(char *str, int x, int y, int constraint) {
	int len, i, j, k;
	int xBegin;

	y = sizeY - 1 - y;
	if (str == NULL)return -1;

	xBegin = x;
	if (constraint < (int)strlen(str))len = constraint;
	else len = strlen(str);
	if (str[len - 1] >= 0x80)len--;
	for (i = 0; i < len; i++) {
		if (str[i] == '\n') {
			return i + 1;
		}
		for (j = 0; j < 8; j++)
			for (k = 7; k >= 0; k--)
				if ((letters[str[i]][j] >> k) & 1) {
					putPixel(x + 7 - k, sizeY - 1 - (y + 2 * j));
					putPixel(x + 7 - k, sizeY - 1 - (y + 2 * j + 1));
				}
		x += SG_CHAR_WIDTH;
	}
	return len;
}
