#include "main.h"
#include "Frame/shader/shader.h"
#include "Window/scene/model.h"
#include "Window/scene/light.h"
#include "Window/scene/eye.h"
#include "Window/page/layout.h"

vector<Layout*> window;
vector<Scene*> globe;
vector<Light*> light;

Eye *eye;
Mouse *mouse;
extern Shader *elementShader;
extern Shader *texShader;
extern Shader *shadowShader;

static void RWEInit() {
	glewInit();
	glEnable(GL_DEPTH_TEST);
	elementShader = new Shader("Frame//shader//element.vert", "Frame//shader//element.frag");
	texShader = new Shader("Frame//shader//tex.vert", "Frame//shader//tex.frag");
	shadowShader = new Shader("Frame//shader//shadow.vert", "Frame//shader//shadow.frag");

	eye = new Eye(glm::vec3(0.0f, 0.0f, 8.0f));
	mouse = new Mouse();
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
}
static void RWEDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto &l : light) {
		l->render();
	}

	glutSwapBuffers();
}
static void RWEMouse(int x, int y) {
	mouse->tmp.x = float(x);
	mouse->tmp.y = float(y);
}
static void RWEDrag(int x, int y) {
	if(mouse->state[GLUT_LEFT_BUTTON] == GLUT_DOWN)
		eye->rotate(float(x - mouse->pre.x)/200, float(y - mouse->pre.y) / 200);
	mouse->pre.x = float(x);
	mouse->pre.y = float(y);
	glutPostRedisplay();
}
static void RWEClick(int button, int state, int x, int y) {
	mouse->state[button] = state;
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN) {
		mouse->pre.x = float(x);
		mouse->pre.y = float(y);
	}
}
static void RWEWheel(int wheel, int dir, int x, int y) {
	if (dir > 0)eye->zoom(.8f);
	if (dir < 0)eye->zoom(1.2f);
	glutPostRedisplay();
}

void build() {
	Element inputLayout;
	Scene *tmpScene = new Scene();
	inputLayout.load("Window//src//welcome.obj");
	tmpScene->elements.push_back(inputLayout);
	globe.push_back(tmpScene);

	light.push_back(new Light(glm::vec3(-28.f, 20.f, 12.f), glm::vec3(.8f, .8f, .8f), glm::vec3(.2f, .2f, .2f), .6f));
}
int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Real Writing Environment - SG Creater");
	RWEInit();

	glutDisplayFunc(RWEDisplay);
	glutPassiveMotionFunc(RWEMouse);
	glutMotionFunc(RWEDrag);
	glutMouseFunc(RWEClick);
	glutMouseWheelFunc(RWEWheel);

	build();
	glutMainLoop();
	return 0;
}
