#ifndef MOLE_WINDOW_SCENE_LIGHT
#define MOLE_WINDOW_SCENE_LIGHT
#include "Frame/main/main.h"

enum lm {
	NO_SRC,
	DIR_SRC,
	POINT_SRC
};

class Light {
private:
	union {
		glm::vec3 pos;
		glm::vec3 dir;
	};
	enum lm mode;
	glm::vec3 diffuse, ambient, specular;
	GLuint frameHandle, textureHandle, depthHandle;

	void genShadow();
public:
	Light(int mode, glm::vec3 para, glm::vec3 diff, glm::vec3 amb, glm::vec3 spec) {
		this->mode = enum lm(mode);
		pos = dir = para;
		diffuse = diff;
		ambient = amb;
		specular = spec;
		glGenTextures(1, &textureHandle);
		glGenFramebuffers(1, &frameHandle);
		glGenRenderbuffers(1, &depthHandle);
	};
	Light(const Light &l) {
		mode = l.mode;
		pos = l.pos;
		diffuse = l.diffuse;
		ambient = l.ambient;
		specular = l.specular;
		glGenTextures(1, &textureHandle);
		glGenFramebuffers(1, &frameHandle);
		glGenRenderbuffers(1, &depthHandle);
	};
	void render();
};

#endif
