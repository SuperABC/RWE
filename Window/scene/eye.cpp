#include "Frame/main/main.h"
#include "eye.h"

Eye::Eye() {
	this->pos = glm::vec3(0, 0, 4);
	this->dir = glm::vec3(0, 0, -4);
	this->up = glm::vec3(0, 1, 0);
	this->theta = float(PI / 2);
	this->phy = float(PI / 2);
	this->dist = 8.f;
}
Eye::Eye(glm::vec3 pos) {
	this->pos = pos;
	this->dir = glm::vec3(0, 0, -1);
	this->up = glm::vec3(0, 1, 0);
	this->theta = float(PI / 2);
	this->phy = float(PI / 2);
	this->dist = 8.f;
}
Eye::~Eye() {

}

glm::mat4 Eye::view() {
	return glm::lookAt(this->pos, this->pos + this->dir, this->up);
}
void Eye::rotate(float angx, float angy) {
	this->theta += angx;
	this->phy = clamp<float>(0.01f, float(PI - 0.01), this->phy - angy);
	this->pos.x = dist*sin(phy)*cos(theta);
	this->pos.y = dist*cos(phy);
	this->pos.z = dist*sin(phy)*sin(theta);
	this->dir.x = -this->pos.x;
	this->dir.y = -this->pos.y;
	this->dir.z = -this->pos.z;
}
void Eye::zoom(float prop) {
	dist = clamp<float>(.25f, 64.f, dist*prop);
	this->pos.x = dist*sin(phy)*cos(theta);
	this->pos.y = dist*cos(phy);
	this->pos.z = dist*sin(phy)*sin(theta);
	this->dir.x = -this->pos.x;
	this->dir.y = -this->pos.y;
	this->dir.z = -this->pos.z;
}
Ray Eye::ray(int x, int y) {
	Ray ret = Ray();
	ret.src = Point(pos);

	float fx = 2 * float(x - SCR_WIDTH / 2) / SCR_WIDTH;
	float fy = 2 * float(SCR_HEIGHT/2 - y)/SCR_HEIGHT;
	glm::mat4 projection = glm::perspective(45.f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(pos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	ret.dir = Point(glm::inverse(view)*glm::inverse(projection)*glm::vec4(fx, fy, 1.f, 1.f))-Point(pos.x, pos.y, pos.z);
	return ret;
}
