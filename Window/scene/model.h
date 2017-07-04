#ifndef MOLE_WINDOW_SCENE_MODEL
#define MOLE_WINDOW_SCENE_MODEL
#include "Frame/main/main.h"

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
public:
	GLuint vao, svao;
	GLuint vboHandles[3],
		positionBufferHandle, colorBufferHandle, normalBufferHandle;
	GLuint spositionBufferHandle;

	Element() {
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

	void load(const char *filename);
	void shadow();
	void show();
};
class Texture {

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
	void show();
};

#endif