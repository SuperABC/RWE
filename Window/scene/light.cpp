#include "Window/scene/light.h"
#include "Window/scene/model.h"
#include "Window/scene/eye.h"
#include "Frame/shader/shader.h"

extern int SCR_WIDTH;
extern int SCR_HEIGHT;

extern vector<Scene*> globe;
extern Eye *eye;

extern Shader *elementShader;
extern Shader *texShader;
extern Shader *shadowShader;

void Light::genShadow() {
	glm::mat4 lightProjection, lightView;
	lightProjection = glm::perspective(60.f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, 8.f, 10000.0f);
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

