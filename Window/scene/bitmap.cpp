#include "Window/scene/bitmap.h"
#include "Window/scene/light.h"
#include "Window/scene/model.h"
#include "Window/scene/eye.h"
#include "Frame/shader/shader.h"

extern Shader *elementShader;
extern Shader *texShader;
extern Shader *shadowShader;

extern vector<Scene*> globe;
extern vector<Light*> light;
extern Eye *eye;

void Screen::initWindow(int x, int y, const char *title, int mode) {

}

void Screen::setColor(int r, int g, int b) {
	rgb[0] = r % 256;
	rgb[1] = g % 256;
	rgb[2] = b % 256;

	tf.color.r = r;
	tf.color.g = g;
	tf.color.b = b;
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
void Screen::putLine(int x1, int y1, int x2, int y2, int mode) {
	int dx, dy, ux, uy, x, y, eps;

	if (x1 > x2)x1 ^= x2 ^= x1 ^= x2;
	if (y1 > y2)y1 ^= y2 ^= y1 ^= y2;

	dx = x2 - x1;
	dy = y2 - y1;

	ux = ((dx > 0) << 1) - 1;
	uy = ((dy > 0) << 1) - 1;

	x = x1;
	y = y1;

	eps = 0;

	if (dx > dy) {
		for (x = x1; x != x2 + ux; x += ux) {
			putPixel(x, y);
			eps += dy;
			if ((eps << 1) >= dx) {
				y += uy;
				eps -= dx;
			}
		}
	}
	else {
		for (y = y1; y != y2 + uy; y += uy) {
			putPixel(x, y);
			eps += dx;
			if ((eps << 1) >= dy) {
				x += ux;
				eps -= dy;
			}
		}
	}
}
void Screen::putQuad(int x1, int y1, int x2, int y2, int mode) {
	int i, j;

	y1 = sizeY - 1 - y1;
	y2 = sizeY - 1 - y2;
	if (x1 > x2)x1 ^= x2 ^= x1 ^= x2;
	if (y1 > y2)y1 ^= y2 ^= y1 ^= y2;

	if (mode == SOLID_FILL)
		for (j = y1; j <= y2; j++) {
			int pos = (j * sizeX + x1) * 3;
			for (i = x1; i <= x2; i++) {
				data[pos] = rgb[2];
				data[pos + 1] = rgb[1];
				data[pos + 2] = rgb[0];
				pos += 3;
			}
		}

	if (mode == EMPTY_FILL) {
		int pos1 = (y1 * sizeX + x1) * 3;
		int pos2 = (y2 * sizeX + x1) * 3;
		for (i = x1; i <= x2; i++) {
			data[pos1] = rgb[2];
			data[pos1 + 1] = rgb[1];
			data[pos1 + 2] = rgb[0];
			pos1 += 3;
			data[pos2] = rgb[2];
			data[pos2 + 1] = rgb[1];
			data[pos2 + 2] = rgb[0];
			pos2 += 3;
		}
		for (j = y1; j <= y2; j++) {
			putPixel(x1, sizeY - 1 - j);
			putPixel(x2, sizeY - 1 - j);
		}
	}
}
void Screen::putCircle(int xc, int yc, int r, int mode) {
	int x, y, yi, d;

	x = 0;
	y = r;
	d = 3 - 2 * r;

	if (mode == SOLID_FILL) {
		while (x <= y) {
			for (yi = x; yi <= y; yi++) {
				putPixel(xc + x, yc + yi);
				putPixel(xc + x, yc - yi);
				putPixel(xc - x, yc - yi);
				putPixel(xc - x, yc + yi);
				putPixel(xc + yi, yc + x);
				putPixel(xc + yi, yc - x);
				putPixel(xc - yi, yc - x);
				putPixel(xc - yi, yc + x);
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
			putPixel(xc + x, yc + y);
			putPixel(xc + x, yc - y);
			putPixel(xc - x, yc - y);
			putPixel(xc - x, yc + y);
			putPixel(xc + y, yc + x);
			putPixel(xc + y, yc - x);
			putPixel(xc - y, yc - x);
			putPixel(xc - y, yc + x);

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

	sqa = a * a;
	sqb = b * b;
	x = 0;
	y = b;
	d = 2 * sqb - 2 * b * sqa + sqa;
	P_x = (int)((double)sqa / sqrt((double)(sqa + sqb)));

	putPixel(xc + x, yc + y);
	putPixel(xc - x, yc - y);

	if (mode == SOLID_FILL) {
		while (x <= P_x) {
			if (d < 0) d += 2 * sqb * (2 * x + 3);
			else {
				d += 2 * sqb * (2 * x + 3) - 4 * sqa * (y - 1);
				y--;
			}
			x++;

			for (xi = 0; xi <= x; xi++) {
				putPixel(xc + xi, yc + y);
				putPixel(xc - xi, yc + y);
				putPixel(xc + xi, yc - y);
				putPixel(xc - xi, yc - y);
			}
		}
		d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
		while (y >= 0) {
			for (xi = 0; xi <= x; xi++) {
				putPixel(xc + xi, yc + y);
				putPixel(xc - xi, yc + y);
				putPixel(xc + xi, yc - y);
				putPixel(xc - xi, yc - y);
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
			putPixel(xc + x, yc + y);
			putPixel(xc - x, yc + y);
			putPixel(xc + x, yc - y);
			putPixel(xc - x, yc - y);
		}
		d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
		while (y >= 0) {
			putPixel(xc + x, yc + y);
			putPixel(xc - x, yc + y);
			putPixel(xc + x, yc - y);
			putPixel(xc - x, yc - y);
			y--;
			if (d < 0) {
				x++;
				d = d - 2 * sqa * y - sqa + 2 * sqb * x + 2 * sqb;
			}
			else d = d - 2 * sqa * y - sqa;
		}
	}
}
int Screen::loadBmp(int x, int y, const char *filename) {
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

	if (sizeY - 1 - y + height > (int)sizeY) {
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

	if (lr == 'l') {
		if (n == 0)putChar('0', x, y);
		while (n > 0) {
			s[sn++] = n % 10;
			n /= 10;
		}

		while (sn > 0) {
			putChar((char)(s[--sn] + '0'), x, y);
			x += 8;
		}
	}
	else if (lr == 'r') {
		if (n == 0)putChar('0', x - 8, y);
		while (n > 0) {
			x -= 8;
			putChar((char)(n % 10 + '0'), x, y);
			n /= 10;
		}
	}
}
void Screen::putChar(char ch, int x, int y) {
	int j, k;

	for (j = 0; j < 8; j++)
		for (k = 7; k >= 0; k--)
			if ((letters[ch][j] >> k) & 1) {
				putPixel(x + 7 - k, y + 2 * j);
				putPixel(x + 7 - k, y + 2 * j + 1);
			}
}
void Screen::setFontSize(int size) {
	if (size < 128)tf.size = size;
	else tf.size = 128;

	DeleteObject(text.font);
	text.font = CreateFont(tf.size, 0, 0, 0, FW_THIN, 0, 0, 0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, FF_MODERN, tf.name);
	SelectObject(text.memDC, text.font);
}
void Screen::setFontName(const char *name) {
	tf.name = name;

	DeleteObject(text.font);
	text.font = CreateFont(tf.size, 0, 0, 0, FW_THIN, 0, 0, 0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, FF_MODERN, tf.name);
	SelectObject(text.memDC, text.font);
}
void Screen::putString(const char *str, int x, int y) {
	unsigned int tab = 0, i, j;
	char *tmp;
	if (str == NULL)return;

	y = sizeY - 1 - y;
	for (i = 0; i < strlen(str); i++) {
		if (str[i] == '\t')tab++;
	}
	tmp = (char *)malloc(strlen(str) + 3 * tab + 1);
	memset(tmp, 0, strlen(str) + 3 * tab + 1);
	for (i = 0, j = 0; i < strlen(str); i++) {
		if (str[i] == '\t') {
			tmp[j] = tmp[j + 1] = tmp[j + 2] = tmp[j + 3] = ' ';
			j += 4;
		}
		else tmp[j++] = str[i];
	}

	GetTextExtentPoint32(text.memDC, tmp, strlen(tmp), &text.strRect);
	memset(text.bitBuf, 0, text.bufSize);
	RECT imgRect = { 0, 0, text.strRect.cx, text.strRect.cy };
	FillRect(text.memDC, &imgRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

	TextOut(text.memDC, 0, 0,tmp, strlen(tmp));
	_hbmImage(text.memDC, text.hbm, x, y, tmp);
	free(tmp);
}
int Screen::stringWidth(const char *str, int x) {
	SIZE ret;
	int tab = 0, i, j;
	char *tmp;

	if (x < 0)return 0;

	for (i = 0; i < x; i++) {
		if (str[i] == '\t')tab++;
	}
	tmp = (char *)malloc(x + 3 * tab + 1);
	memset(tmp, 0, x + 3 * tab + 1);
	for (i = 0, j = 0; i < x; i++) {
		if (str[i] == '\t') {
			tmp[j] = tmp[j + 1] = tmp[j + 2] = tmp[j + 3] = ' ';
			j += 4;
		}
		else tmp[j++] = str[i];
	}
	x += 3 * tab;

	GetTextExtentPoint32(text.memDC, tmp, x, &ret);
	free(tmp);
	return ret.cx;
}
int Screen::putStringConstraint(const char *str, int x, int y, int constraint) {
	int len, i, j, k;
	int xBegin;

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
					putPixel(x + 7 - k, y + 2 * j);
					putPixel(x + 7 - k, y + 2 * j + 1);
				}
		x += SG_CHAR_WIDTH;
	}
	return len;
}

Canvas::Canvas(glm::vec3 d, glm::vec3 a, float s){
	pos.clear();
	coord.clear();
	normal.clear();
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vboHandles);
	positionBufferHandle = vboHandles[0];
	coordBufferHandle = vboHandles[1];
	normalBufferHandle = vboHandles[2];
	glGenTextures(1, &texName);

	kd = d;
	ka = a;
	ks = s;
}
Canvas::~Canvas() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(3, vboHandles);
}
void Canvas::pic(const char *fileName) {
	if (!fileName) {
		sizeX = (int)area.getSize().x;
		sizeY = (int)area.getSize().y;
		data = (unsigned char *)malloc(3 * sizeX*sizeY);
		memset(data, 0, 3 * sizeX*sizeY);
		return;
	}
	CImage *img = new CImage();
	HRESULT hr = img->Load(fileName);
	if (!SUCCEEDED(hr)) {
		return;
	}
	sizeX = img->GetWidth();
	sizeY = img->GetHeight();
	if (img->GetPitch()<0)data = (unsigned char *)img->GetBits() + (img->GetPitch()*(img->GetHeight() - 1));
	else data = (unsigned char *)img->GetBits();
}
Canvas *Canvas::load(const char *filename) {
	std::ifstream fin;

	fin.open(filename);
	if (fin.is_open() == FALSE)return NULL;

	string op;
	float num1, num2, num3;

	vector<float>pos;
	vector<float>coord;
	vector<float>normal;

	vector<string>name;
	vector<string>str;

	while (fin >> op) {
		if (op == "v") {
			fin >> num1 >> num2 >> num3;
			pos.push_back(num1);
			pos.push_back(num3);
			pos.push_back(num2);
		}
		else if (op == "vt") {
			fin >> num1 >> num2;
			coord.push_back(num1);
			coord.push_back(num2);
		}
		else if (op == "vn") {
			fin >> num1 >> num2 >> num3;
			normal.push_back(num1);
			normal.push_back(num2);
			normal.push_back(num3);
		}
		else if (op == "f") {
			string v1, v2, v3;
			int tmp1 = 0, tmp2 = 0, tmp3 = 0;

			fin >> v1 >> v2 >> v3;
			num1 = (float)atoi(v1.c_str() + tmp1);
			num2 = (float)atoi(v2.c_str() + tmp2);
			num3 = (float)atoi(v3.c_str() + tmp3);
			this->pushPos(pos[(int)num1 * 3 - 3], pos[(int)num1 * 3 - 2], pos[(int)num1 * 3 - 1]);
			this->pushPos(pos[(int)num2 * 3 - 3], pos[(int)num2 * 3 - 2], pos[(int)num2 * 3 - 1]);
			this->pushPos(pos[(int)num3 * 3 - 3], pos[(int)num3 * 3 - 2], pos[(int)num3 * 3 - 1]);

			tmp1 = v1.find_first_of('/', tmp1) + 1;
			tmp2 = v2.find_first_of('/', tmp2) + 1;
			tmp3 = v3.find_first_of('/', tmp3) + 1;
			if (tmp1 == 0 || tmp2 == 0 || tmp3 == 0) {
				this->pushCoord();
				this->pushCoord();
				this->pushCoord();
			}
			else {
				num1 = (float)atoi(v1.c_str() + tmp1);
				num2 = (float)atoi(v2.c_str() + tmp2);
				num3 = (float)atoi(v3.c_str() + tmp3);
				this->pushCoord(coord[(int)num1 * 2 - 2], coord[(int)num1 * 2 - 1]);
				this->pushCoord(coord[(int)num2 * 2 - 2], coord[(int)num2 * 2 - 1]);
				this->pushCoord(coord[(int)num3 * 2 - 2], coord[(int)num3 * 2 - 1]);
			}

			tmp1 = v1.find_first_of('/', tmp1) + 1;
			tmp2 = v2.find_first_of('/', tmp2) + 1;
			tmp3 = v3.find_first_of('/', tmp3) + 1;
			if (tmp1 == 0 || tmp2 == 0 || tmp3 == 0) {
				int p1 = atoi(v1.c_str());
				int p2 = atoi(v2.c_str());
				int p3 = atoi(v3.c_str());
				glm::vec3 edge1(pos[p2 * 3 - 3] - pos[p1 * 3 - 3],
					pos[p2 * 3 - 2] - pos[p1 * 3 - 2],
					pos[p2 * 3 - 1] - pos[p1 * 3 - 1]);
				glm::vec3 edge2(pos[p3 * 3 - 3] - pos[p2 * 3 - 3],
					pos[p3 * 3 - 2] - pos[p2 * 3 - 2],
					pos[p3 * 3 - 1] - pos[p2 * 3 - 1]);
				glm::vec3 norm(edge2.y *edge1.z - edge1.y*edge2.z,
					edge2.z*edge1.x - edge1.z*edge2.x,
					edge2.x*edge1.y - edge1.x*edge2.y);
				this->pushNormal(norm.x, norm.y, norm.z);
				this->pushNormal(norm.x, norm.y, norm.z);
				this->pushNormal(norm.x, norm.y, norm.z);
			}
			else {
				num1 = (float)atoi(v1.c_str() + tmp1);
				num2 = (float)atoi(v2.c_str() + tmp2);
				num3 = (float)atoi(v3.c_str() + tmp3);
				this->pushNormal(normal[(int)num1 * 3 - 3], normal[(int)num1 * 3 - 2], normal[(int)num1 * 3 - 1]);
				this->pushNormal(normal[(int)num2 * 3 - 3], normal[(int)num2 * 3 - 2], normal[(int)num2 * 3 - 1]);
				this->pushNormal(normal[(int)num3 * 3 - 3], normal[(int)num3 * 3 - 2], normal[(int)num3 * 3 - 1]);
			}
		}
		else if (op == "mtllib") {
			fin >> op;
			if (op == "empty") {
				pic();
				continue;
			}

			std::ifstream min;
			string path = string(filename);
			unsigned int tmp = path.find_last_of('/');
			for (unsigned int i = path.length() - 1; i > tmp; i--)
				path.pop_back();
			path += op;

			min.open(path);
			if (min.is_open() == FALSE)continue;

			bool complete = true;
			while (min >> op) {
				if (op == "newmtl") {
					if (!complete)str.push_back("");
					min >> op;
					name.push_back(op);
					complete = false;
				}
				else if (op == "map_Kd") {
					assert(!complete);
					min >> op;
					str.push_back(op);
					complete = true;
				}
				else {
					char *buf = new char[256];
					min.getline(buf, 256);
					delete buf;
				}
			}
		}
		else if (op == "usemtl") {
			fin >> op;
			for (unsigned int i = 0; i < name.size(); i++) {
				if (op == name[i]) {
					pic(str[i].data());
				}
			}
		}
		else {
			char *buf = new char[256];
			fin.getline(buf, 256);
			delete buf;
		}
	}

	return this;
}
void Canvas::show() {
	if (pos.size() == 0)return;

	texShader->use();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, pos.size() * 4, getPos(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glBindBuffer(GL_ARRAY_BUFFER, coordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, coord.size() * 4, getCoord(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * 4, getNormal(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sizeX, sizeY, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	texShader->setInt("u_enLight", 1);
	texShader->setMat4("u_modelMatrix", model);
	glm::mat4x4 inv = glm::transpose(glm::inverse(model));
	elementShader->setMat4("u_normalMatrix", inv);
	for (unsigned int i = 0; i < light.size(); i++) {
		char tmp[64];
		sprintf(tmp, "u_shadowMap[%d]", i);
		texShader->setInt(tmp, -1);
	}
	texShader->setInt("u_textureMap", 0);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, pos.size() / 3);
}
