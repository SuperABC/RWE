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
