#ifndef MOLE_WINDOW_SCENE_LIGHT
#define MOLE_WINDOW_SCENE_LIGHT
#include "Frame/main/main.h"

class Light {
private:
	glm::vec3 pos;
	GLuint frameHandle, textureHandle;

	glm::mat4 lightMatrix;

	GLuint id;

	void genShadow();
public:
	glm::vec3 diffuse, ambient;
	GLfloat specular;

	Light(GLuint ID) : id(ID) {};
	Light(GLuint ID, glm::vec3 position, glm::vec3 diff, glm::vec3 amb, GLfloat spec) {
		pos = position;
		diffuse = diff;
		ambient = amb;
		specular = spec;
		glGenFramebuffers(1, &frameHandle);
		glGenTextures(1, &textureHandle);

		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, frameHandle);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureHandle, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	};
	Light(const Light &l) {
		pos = l.pos;
		diffuse = l.diffuse;
		ambient = l.ambient;
		specular = l.specular;
		textureHandle = l.textureHandle;
		frameHandle = l.frameHandle;
		id = l.id;
	};
	void render();
};

#endif
