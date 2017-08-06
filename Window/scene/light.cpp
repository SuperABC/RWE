#include "light.h"
#include "model.h"
#include "eye.h"
#include "Frame/shader/shader.h"
#include "Window/page/layout.h"

extern vector<Layout*> window;
extern vector<Scene*> globe;
extern Eye *eye;

extern Shader *elementShader;
extern Shader *texShader;
extern Shader *shadowShader;

void Light::genShadow() {
	glm::mat4 lightProjection, lightView;
	lightProjection = glm::perspective(60.f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, 1.f, 1000.0f);
	lightView = glm::lookAt(pos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightMatrix = lightProjection * lightView;
	shadowShader->use();
	shadowShader->setMat4("u_lightMatrix", lightMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, frameHandle);
	glClear(GL_DEPTH_BUFFER_BIT);
	for (auto s : globe)
		s->shadow();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Light::render() {
	genShadow();

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	elementShader->use();
	glm::mat4 projection = glm::perspective(45.f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(eye->pos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	elementShader->setMat4("u_projection", projection);
	elementShader->setMat4("u_view", view);
	elementShader->setMat4("u_lightMatrix", lightMatrix);
	elementShader->setVec3("u_eyePos", eye->pos);
	elementShader->setVec3("u_lightPos", pos);
	elementShader->setVec3("u_lightDiff", diffuse);
	elementShader->setVec3("u_lightAmb", ambient);
	elementShader->setFloat("u_lightSpec", specular);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	for (auto s : globe)
		s->show();
}

