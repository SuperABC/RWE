#include "Geometry/primitive/shape.h"

glm::vec2 Square::intersect(Ray r) {
	float t = ((p1.x-r.src.x)*norm.x + (p1.y-r.src.y)*norm.y + (p1.z-r.src.z)*norm.z) /
		(norm.x* r.dir.x + norm.y* r.dir.y + norm.z* r.dir.z);
	Point ints = Point(r(t));

	Vector dir = ints - p1;
	int dirx = (int)(dot(dir, p2 - p1) / ((p2 - p1).square())*widthPixel);
	int diry = (int)(dot(dir, p3 - p1) / ((p3 - p1).square())*heightPixel);

	return glm::vec2(dirx, diry);
}
glm::mat4 Square::model() {
	glm::mat4 ret = glm::mat4();
	return ret;
}
