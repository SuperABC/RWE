#include "light.h"
#include "model.h"
#include "Frame/shader/shader.h"
#include "Window/page/layout.h"

extern vector<Layout*> window;
extern vector<Scene*> globe;

extern Shader *elementShader;
extern Shader *texShader;
extern Shader *shadowShader;

void Light::genShadow() {
	shadowShader->use();
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindRenderbuffer(GL_RENDERBUFFER, depthHandle);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 2048, 2048);

	glBindFramebuffer(GL_FRAMEBUFFER, frameHandle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureHandle, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthHandle);

	glActiveTexture(GL_TEXTURE0);
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 perspectMatrix;
	perspectMatrix = glm::perspective(70.f, 1.f, 1.f, 100.f);
	glUniformMatrix4fv(glGetUniformLocation(shadowShader->program, "u_PerspectMatrix"), 1, false, glm::value_ptr(perspectMatrix));
	glm::mat4 viewMatrix;
	viewMatrix = glm::lookAt(pos, glm::vec3(), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(shadowShader->program, "u_ViewMatrix"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(elementShader->program, "u_MatrixFromLight"), 1, false, glm::value_ptr(perspectMatrix*viewMatrix));

	glViewport(0, 0, 2048, 2048);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform1i(glGetUniformLocation(shadowShader->program, "u_LightMode"), mode);

	for (auto s : globe) {
		if (s->active)s->shadow();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	glBindRenderbuffer(GL_RENDERBUFFER, NULL);
}
void Light::render() {
	//if (mode)genShadow();

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glViewport(0, 0, 800, 600);
	elementShader->use();
	glUniform1i(glGetUniformLocation(elementShader->program, "u_ShadowMap"), 0);

	glUniform1i(glGetUniformLocation(elementShader->program, "u_LightMode"), mode);
	glUniform3f(glGetUniformLocation(elementShader->program, "u_LightDir"), dir.x, dir.y, dir.z);
	glUniform3f(glGetUniformLocation(elementShader->program, "u_LightPos"), pos.x, pos.y, pos.z);

	glUniform3f(glGetUniformLocation(elementShader->program, "u_LightDiffuse"), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(elementShader->program, "u_LightAmbient"), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(elementShader->program, "u_LightSpecular"), specular.x, specular.y, specular.z);
	
	for (auto s : globe) {
		if (s->active)s->show();
	}
	/*for (auto w : window) {
		if (w->active)w->idle();
	}*/
}

