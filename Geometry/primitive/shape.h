#ifndef RWE_GEOMETRY_PRIMITIVE_SHAPE
#define RWE_GEOMETRY_PRIMITIVE_SHAPE
#include "Frame/main/main.h"
#include "Geometry/primitive/arith.h"
#include "Geometry/primitive/ray.h"
#include "Geometry/primitive/box.h"

class Triangle {

};
class Square {
private:
	Point p1, p2, p3, p4;
	int widthPixel, heightPixel;
	Vector norm;
public:
	Square() {};
	Square(Point p1, Point p2, Point p3, Point p4, int width, int height) :
		p1(p1), p2(p2), p3(p3), p4(p4), widthPixel(width), heightPixel(height) {
		norm = cross(p2 - p1, p3 - p1);
	};
	~Square() {};

	Box bound();
	glm::vec2 intersect(Ray r);
	Point coord(glm::vec2 c);
};

#endif