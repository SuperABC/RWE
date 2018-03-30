#include "Frame/main/main.h"
#include "Window/scene/eye.h"

extern int SCR_WIDTH;
extern int SCR_HEIGHT;

Eye::Eye() {
	this->pos = glm::vec3(0, 0, 4);
	this->dir = glm::vec3(0, 0, -4);
	this->up = glm::vec3(0, 1, 0);
	this->theta = float(PI / 2);
	this->phy = float(PI / 2);
	this->dist = 8.f;
	this->model = glm::mat4();
}
Eye::Eye(glm::vec3 pos) {
	this->pos = pos;
	this->dir = -pos;
	this->up = glm::vec3(0, 1, 0);
	this->theta = float(PI / 2);
	this->phy = float(PI / 2);
	this->dist = 8.f;
	this->model = glm::mat4();
}
Eye::~Eye() {

}

glm::mat4 Eye::view() {
	return glm::lookAt(this->pos, this->pos + this->dir, this->up);
}
void Eye::move(float dx, float dy) {
	pos.x -= dy*cos(theta) + dx*sin(theta);
	pos.z -= dy*sin(theta) - dx*cos(theta);
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
	glm::mat4 view = glm::lookAt(pos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	ret.dir = Point(glm::inverse(projection*view)*glm::vec4(fx, fy, 1.f, 1.f)) - Point(pos.x, pos.y, pos.z);
	return ret;
}
