#ifndef MOLE_FRAME_SHADER_SHADER
#define MOLE_FRAME_SHADER_SHADER

#include "Frame/main/main.h"

char *readText(const char *file);

class Shader {
public:
	GLuint program;
	Shader(const GLchar *vertexSource, const GLchar *fragmentSource);
	void use();
};

#endif