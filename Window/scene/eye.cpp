#include "Frame/main/main.h"
#include "Window/scene/eye.h"

extern int SCR_WIDTH;
extern int SCR_HEIGHT;

extern Eye *eye;

glm::vec3 posDst, dirDst;
int eyeLock = 0;
DWORD WINAPI eyeAnimate(LPVOID pM) {
	while (eyeLock == 1);
	eyeLock = 1;
	glm::vec3 v = (posDst - eye->pos)*.04f, d = (dirDst - eye->dir)*.04f;
	float threshold = length(v);
	while(1) {
		eye->pos = eye->pos *.9f + posDst * .1f;
		eye->dir = eye->dir * .9f + dirDst * .1f;
		if (length(posDst - eye->pos) < threshold)break;
		Sleep(24);
	}
	for (int i = 0; i < 20; i++) {
		if (length(posDst - eye->pos) < threshold / 20)break;
		else {
			eye->pos = eye->pos += v * .05f;
			eye->dir = eye->dir += d * .05f;
		}
		Sleep(24);
	}

	eye->pos = posDst;
	eye->dir = dirDst;
	eye->center = eye->pos + eye->dir;
	eye->sync(eye->dir);
	eyeLock = 0;
	return 0;
}

Eye::Eye() {
	this->pos = glm::vec3(0, 0, 4);
	this->center = glm::vec3(0, 0, 0);
	this->dir = glm::vec3(0, 0, -4);
	this->up = glm::vec3(0, 1, 0);
	this->theta = float(PI / 2);
	this->phy = float(PI / 2);
	this->dist = 8.f;
	this->model = glm::mat4();
}
Eye::Eye(glm::vec3 pos) {
	this->pos = pos;
	this->center = glm::vec3(0, 0, 0);
	this->dir = -pos;
	this->sync(this->dir);
	this->up = glm::vec3(0, 1, 0);
	this->dist = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
	this->model = glm::mat4();
}
Eye::~Eye() {

}

void Eye::sync(float theta, float phy) {
	dir.x = sin(phy)*cos(theta) * dist;
	dir.y = cos(phy) * dist;
	dir.z = sin(phy)*sin(theta) * dist;
}
void Eye::sync(glm::vec3 dir) {
	dist = sqrt(dir.z * dir.z + dir.y *dir.y + dir.x * dir.x);

	if (dir.x > 0)
		theta = atan(dir.z / dir.x);
	else if (dir.x < 0)
		theta = atan(dir.z / dir.x) + PI / 2;
	else if (dir.z > 0)
		theta = 3 * PI / 2;
	else theta = PI / 2;
	if (theta < 0)theta = 2*PI;
	phy = asin(sqrt(dir.z * dir.z + dir.x * dir.x)/dist);
}
glm::mat4 Eye::view() {
	return glm::lookAt(this->pos, this->pos + this->dir, this->up);
}
void Eye::move(float dx, float dy) {
	pos.x -= dy*cos(theta) + dx*sin(theta);
	center.x -= dy * cos(theta) + dx * sin(theta);
	pos.z -= dy*sin(theta) - dx*cos(theta);
	center.z -= dy * sin(theta) - dx * cos(theta);
}
void Eye::rotate(float angx, float angy) {
	float atx = pos.x + dir.x;
	float aty = pos.y + dir.y;
	float atz = pos.z + dir.z;
	this->theta += angx;
	this->phy = clamp<float>(0.01f, float(PI - 0.01), this->phy - angy);
	this->pos.x = atx + dist*sin(phy)*cos(theta);
	this->pos.y = aty + dist*cos(phy);
	this->pos.z = atz + dist*sin(phy)*sin(theta);
	this->dir.x = atx-this->pos.x;
	this->dir.y = aty-this->pos.y;
	this->dir.z = atz-this->pos.z;
}
void Eye::zoom(float prop) {
	float preDist = dist;
	dist = clamp<float>(.1f, 256.f, dist*prop);

	prop = dist / preDist;
	this->pos += dir * (1 - prop);
	this->dir *= prop;
}
Ray Eye::ray(int x, int y) {
	Ray ret = Ray();
	ret.src = Point(pos);

	float fx = 2 * float(x - SCR_WIDTH / 2) / SCR_WIDTH;
	float fy = 2 * float(SCR_HEIGHT/2 - y)/SCR_HEIGHT;
	glm::mat4 projection = glm::perspective(45.f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(pos, center, glm::vec3(0.0, 1.0, 0.0));
	ret.dir = Point(glm::inverse(projection*view)*glm::vec4(fx, fy, 1.f, 1.f)) - Point(pos.x, pos.y, pos.z);
	return ret;
}

void Eye::animate(glm::vec3 pos, glm::vec3 dir) {
	posDst = pos;
	dirDst = dir;
	CreateThread(NULL, 0, eyeAnimate, NULL, 0, NULL);
}
