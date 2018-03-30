#include "Frame/main/main.h"
#include "Frame/shader/shader.h"
#include "Window/scene/model.h"
#include "Window/scene/light.h"
#include "Window/scene/eye.h"
#include "Window/layout/layout.h"

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

vector<Layout*> window;
vector<Scene*> globe;
vector<Light*> light;

Eye *eye;
Mouse *mouse;
Keyboard *key;
extern Shader *elementShader;
extern Shader *texShader;
extern Shader *shadowShader;

void renderGlobe() {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = glm::perspective(45.f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = glm::lookAt(eye->pos, eye->pos + eye->dir, glm::vec3(0.0, 1.0, 0.0));
	elementShader->use();
	elementShader->setMat4("u_projection", projection);
	elementShader->setMat4("u_view", view);
	elementShader->setVec3("u_eyePos", eye->pos);
	for (unsigned int i = 0; i < light.size(); i++) {
		char tmp[64];
		sprintf(tmp, "u_lightsMatrix[%d]", i);
		elementShader->setMat4(tmp, light[i]->lightMatrix);
		sprintf(tmp, "u_lightInfo[%d].u_lightPos", i);
		elementShader->setVec3(tmp, light[i]->pos);
		sprintf(tmp, "u_lightInfo[%d].u_lightDiff", i);
		elementShader->setVec3(tmp, light[i]->diffuse);
		sprintf(tmp, "u_lightInfo[%d].u_lightAmb", i);
		elementShader->setVec3(tmp, light[i]->ambient);
		sprintf(tmp, "u_lightInfo[%d].u_lightSpec", i);
		elementShader->setFloat(tmp, light[i]->specular);
	}
	texShader->use();
	texShader->setMat4("u_projection", projection);
	texShader->setMat4("u_view", view);
	texShader->setVec3("u_eyePos", eye->pos);
	for (unsigned int i = 0; i < light.size(); i++) {
		char tmp[64];
		sprintf(tmp, "u_lightsMatrix[%d]", i);
		texShader->setMat4(tmp, light[i]->lightMatrix);
		sprintf(tmp, "u_lightInfo[%d].u_lightPos", i);
		texShader->setVec3(tmp, light[i]->pos);
		sprintf(tmp, "u_lightInfo[%d].u_lightDiff", i);
		texShader->setVec3(tmp, light[i]->diffuse);
		sprintf(tmp, "u_lightInfo[%d].u_lightAmb", i);
		texShader->setVec3(tmp, light[i]->ambient);
		sprintf(tmp, "u_lightInfo[%d].u_lightSpec", i);
		texShader->setFloat(tmp, light[i]->specular);
	}
	int ln = light.size();
	for (int i = 0; i < ln; i++) {
		glActiveTexture(GL_TEXTURE2 + i);
		glBindTexture(GL_TEXTURE_2D, light[i]->textureHandle);
	}
	for (auto s : globe) {
		s->show();
	}
}

static void RWEInit() {
	glewInit();
	glEnable(GL_DEPTH_TEST);
	elementShader = new Shader("Frame//shader//element.vert", "Frame//shader//element.frag");
	texShader = new Shader("Frame//shader//tex.vert", "Frame//shader//tex.frag");
	shadowShader = new Shader("Frame//shader//shadow.vert", "Frame//shader//shadow.frag");

	eye = new Eye(glm::vec3(0.0f, 0.0f, 8.0f));
	mouse = new Mouse();
	key = new Keyboard();
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
}
static void RWEDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	elementShader->use();
	elementShader->setInt("u_lightNum", light.size());
	texShader->use();
	texShader->setInt("u_lightNum", light.size());
	for (auto &l : light) {
		l->genShadow();
	}
	renderGlobe();

	glutSwapBuffers();
}
static void RWEReshape(int w, int h) {
	SCR_WIDTH = w;
	SCR_HEIGHT = h;
}
static void RWEMouse(int x, int y) {
	mouse->tmp.x = float(x);
	mouse->tmp.y = float(y);

	for (auto l : window) {
		l->mouseMove(eye->ray(x, y));
	}

	glutPostRedisplay();
}
static void RWEDrag(int x, int y) {
	if (mouse->state[GLUT_LEFT_BUTTON] == GLUT_DOWN)
		eye->rotate(float(x - mouse->pre.x) / 200, float(y - mouse->pre.y) / 200);
	if (mouse->state[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
		eye->move(float(x - mouse->pre.x) / 2, float(y - mouse->pre.y) / 2);
	for (auto l : window) {
		l->mouseDrag(eye->ray(x, y), eye->ray((int)mouse->pre.x, (int)mouse->pre.y));
	}
	mouse->pre.x = float(x);
	mouse->pre.y = float(y);
	glutPostRedisplay();
}
static void RWEClick(int button, int state, int x, int y) {
	mouse->state[button] = state;
	mouse->pre.x = float(x);
	mouse->pre.y = float(y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouse->state[GLUT_LEFT_BUTTON] = GLUT_DOWN;
	}
	for (unsigned int i = 0; i < window.size(); i++) {
		window[i]->mouseClick(button, state, eye->ray(x, y));
	}

	glutPostRedisplay();
}
static void RWEWheel(int wheel, int dir, int x, int y) {
	if (dir > 0)eye->zoom(.8f);
	if (dir < 0)eye->zoom(1.2f);
	for (auto l : window) {
		l->mouseClick(wheel, dir, eye->ray(x, y));
	}
	glutPostRedisplay();
}
static void RWESpecialDown(int key, int x, int y) {
	if (key == RWE_KEY_CTRL)::key->ctrl = true;
	if (key == RWE_KEY_SHIFT)::key->shift = true;
	if (key == RWE_KEY_ALT)::key->alt = true;
	for (auto l : window) {
		l->keyDown(key);
	}
}
static void RWESpecialUp(int key, int x, int y) {
	if (key == RWE_KEY_CTRL)::key->ctrl = false;
	if (key == RWE_KEY_SHIFT)::key->shift = false;
	if (key == RWE_KEY_ALT)::key->alt = false;
	for (auto l : window) {
		l->keyUp(key);
	}
}
static void RWEKeyDown(unsigned char cAscii, int x, int y) {
	for (auto l : window) {
		l->keyDown(cAscii);
	}
}
static void RWEKeyUp(unsigned char cAscii, int x, int y) {
	for (auto l : window) {
		l->keyUp(cAscii);
	}
}

void build() {
	(new Welcome())->registerPage();
	light.push_back(new Light(0, glm::vec3(-24.f, 48.f, 32.f), glm::vec3(.7f, .7f, .7f), glm::vec3(.3f, .3f, .3f), .3f));
	light.push_back(new Light(1, glm::vec3(24.f, 36.f, -18.f), glm::vec3(.3f, .3f, .3f), glm::vec3(.1f, .1f, .1f), .4f));
	light.push_back(new Light(2, glm::vec3(24.f, 32.f, 10.f), glm::vec3(.2f, .2f, .2f), glm::vec3(.0f, .0f, .0f), .4f));
}
int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Frame --SG Creator");
	RWEInit();

	glutDisplayFunc(RWEDisplay);
	glutReshapeFunc(RWEReshape);
	glutPassiveMotionFunc(RWEMouse);
	glutMotionFunc(RWEDrag);
	glutMouseFunc(RWEClick);
	glutMouseWheelFunc(RWEWheel);
	glutKeyboardFunc(RWEKeyDown);
	glutKeyboardUpFunc(RWEKeyUp);
	glutSpecialFunc(RWESpecialDown);
	glutSpecialUpFunc(RWESpecialUp);

	build();
	glutMainLoop();
	return 0;
}
