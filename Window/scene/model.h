#ifndef MOLE_WINDOW_SCENE_MODEL
#define MOLE_WINDOW_SCENE_MODEL
#include "Frame/main/main.h"
#include "Sgs/machine/bitmap.h"
#include <atlimage.h>

class Element {
private:
	vector<float> pos;
	vector<float> color;
	vector<float> normal;

	struct Material {
		string name;
		glm::vec3 kd, ka, ks;
	};
	vector<Material> surface;
	int active = -1;

	glm::vec3 kd, ka;
	float ks;

	glm::mat4 model;
public:
	bool diy = false;

	GLuint vao, svao;
	GLuint vboHandles[3],
		positionBufferHandle, colorBufferHandle, normalBufferHandle;
	GLuint spositionBufferHandle;

	Element(bool diy = false,
		glm::vec3 d = glm::vec3(0.0, 0.0, 0.0), glm::vec3 a = glm::vec3(0.0, 0.0, 0.0),
		float s = 0) {
		pos.clear();
		color.clear();
		normal.clear();
		glGenVertexArrays(1, &vao);
		glGenVertexArrays(1, &svao);
		glGenBuffers(3, vboHandles);
		positionBufferHandle = vboHandles[0];
		colorBufferHandle = vboHandles[1];
		normalBufferHandle = vboHandles[2];
		glGenBuffers(1, &spositionBufferHandle);

		this->diy = diy;
		kd = d;
		ka = a;
		ks = s;
	}
	void pushPos(float pos1 = 0, float pos2 = 0, float pos3 = 0) {
		pos.push_back(pos1);
		pos.push_back(pos2);
		pos.push_back(pos3);
	}
	void pushColor(float color1 = 1, float color2 = 1, float color3 = 1) {
		color.push_back(color1);
		color.push_back(color2);
		color.push_back(color3);
	}
	void pushNormal(float normal1, float normal2, float normal3) {
		normal.push_back(normal1);
		normal.push_back(normal2);
		normal.push_back(normal3);
	}
	float *getPos() {
		return toArray<float>(&pos);
	}
	float *getColor() {
		return toArray<float>(&color);
	}
	float *getNormal() {
		return toArray<float>(&normal);
	}

	Element *load(const char *filename);
	void shadow();
	void show();
};
class Texture {
private:
	vector<float> pos;
	vector<float> coord;
	vector<float> normal;

	glm::vec3 kd, ka;
	float ks;

	glm::mat4 model;
public:
	bool diy = false;
	Screen src;

	GLuint vao, svao, texName;
	GLuint vboHandles[3],
		positionBufferHandle, coordBufferHandle, normalBufferHandle;
	GLuint spositionBufferHandle;

	Texture(bool diy = false,
		glm::vec3 d = glm::vec3(0.0, 0.0, 0.0), glm::vec3 a = glm::vec3(0.0, 0.0, 0.0),
		float s = 0) {
		pos.clear();
		coord.clear();
		normal.clear();
		glGenVertexArrays(1, &vao);
		glGenVertexArrays(1, &svao);
		glGenBuffers(3, vboHandles);
		positionBufferHandle = vboHandles[0];
		coordBufferHandle = vboHandles[1];
		normalBufferHandle = vboHandles[2];
		glGenBuffers(1, &spositionBufferHandle);
		glGenTextures(1, &texName);

		this->diy = diy;
		kd = d;
		ka = a;
		ks = s;
	}
	void pushPos(float pos1 = 0, float pos2 = 0, float pos3 = 0) {
		pos.push_back(pos1);
		pos.push_back(pos2);
		pos.push_back(pos3);
	}
	void pushCoord(float coord1 = 0.f, float coord2 = 0.f) {
		coord.push_back(coord1);
		coord.push_back(coord2);
	}
	void pushNormal(float normal1, float normal2, float normal3) {
		normal.push_back(normal1);
		normal.push_back(normal2);
		normal.push_back(normal3);
	}
	float *getPos() {
		return toArray<float>(&pos);
	}
	float *getCoord() {
		return toArray<float>(&coord);
	}
	float *getNormal() {
		return toArray<float>(&normal);
	}

	Texture *load(const char *filename);
	void shadow();
	void show();

	void pic(const char *fileName);
};

class Scene {
public:
	string name;
	bool active;

	vector<Element> elements;
	vector<Texture> textures;
	vector<Element> motions;
	vector<Texture> gifs;

	Scene() { active = true; };
	~Scene() {};
	void shadow();
	void show(GLuint id);
};

#endif