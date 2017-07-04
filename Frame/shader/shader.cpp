#include "Shader.h"

char *readText(const char *file) {
	FILE *fp;
	fp = fopen(file, "r");
	if (fp == NULL)return NULL;
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	char *cont = (char *)malloc(size);
	memset(cont, 0, size);
	fseek(fp, 0, SEEK_SET);
	fread(cont, sizeof(char), size, fp);
	fclose(fp);
	return cont;
}

Shader::Shader(const GLchar *vertexSource, const GLchar *fragmentSource) {
	char *vv, *ff;
	GLuint vShader, fShader;

	int infoLogLength = 0;
	int charWritten = 0;
	char *infoLog;

	vShader = glCreateShader(GL_VERTEX_SHADER);
	vv = readText(vertexSource);
	if (vv == NULL)return;
	glShaderSource(vShader, 1, &vv, NULL);
	glCompileShader(vShader);
	free(vv);
	glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		infoLog = (char *)malloc(infoLogLength);
		glGetShaderInfoLog(vShader, infoLogLength, &charWritten, infoLog);
		cout << infoLog << endl;
		free(infoLog);
	}

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	ff = readText(fragmentSource);
	if (ff == NULL)return;
	glShaderSource(fShader, 1, &ff, NULL);
	glCompileShader(fShader);
	free(ff);
	glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		infoLog = (char *)malloc(infoLogLength);
		glGetShaderInfoLog(fShader, infoLogLength, &charWritten, infoLog);
		printf("%s \n", infoLog);
		free(infoLog);
	}

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		infoLog = (char *)malloc(infoLogLength);
		glGetProgramInfoLog(program, infoLogLength, &charWritten, infoLog);
		printf("%s \n", infoLog);
		free(infoLog);
	}
	glDeleteShader(vShader);
	glDeleteShader(fShader);
}
void Shader::use() {
	glUseProgram(this->program);
}