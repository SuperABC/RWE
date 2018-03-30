#ifndef RWE_GEOMETRY_PRIMITIVE_BOX
#define RWE_GEOMETRY_PRIMITIVE_BOX

#include "Frame/main/main.h"
#include "Geometry/primitive/arith.h"
#include "Geometry/primitive/ray.h"

class Box {
public:
	Point pMin, pMax;

	Box() {
		pMin = Point(INFINITY, INFINITY, INFINITY);
		pMax = Point(-INFINITY, -INFINITY, -INFINITY);
	}
	Box(const Point &p) : pMin(p), pMax(p) { }
	Box(const Point &p1, const Point &p2) {
		pMin = Point(min(p1.x, p2.x), min(p1.y, p2.y), min(p1.z, p2.z));
		pMax = Point(max(p1.x, p2.x), max(p1.y, p2.y), max(p1.z, p2.z));
	}

	const Point &operator[](int i) const;
	Point &operator[](int i);
	bool operator==(const Box &b) const {
		return b.pMin == pMin && b.pMax == pMax;
	}
	bool operator!=(const Box &b) const {
		return b.pMin != pMin || b.pMax != pMax;
	}

	bool overlap(const Box &b) const {
		bool x = (pMax.x >= b.pMin.x) && (pMin.x <= b.pMax.x);
		bool y = (pMax.y >= b.pMin.y) && (pMin.y <= b.pMax.y);
		bool z = (pMax.z >= b.pMin.z) && (pMin.z <= b.pMax.z);
		return (x && y && z);
	}
	bool inside(const Point &pt) const {
		return (pt.x >= pMin.x && pt.x <= pMax.x &&
			pt.y >= pMin.y && pt.y <= pMax.y &&
			pt.z >= pMin.z && pt.z <= pMax.z);
	}
	void expand(float delta) {
		pMin -= Vector(delta, delta, delta);
		pMax += Vector(delta, delta, delta);
	}
	float area() const {
		Vector d = pMax - pMin;
		return 2.f * (d.x * d.y + d.x * d.z + d.y * d.z);
	}
	float volume() const {
		Vector d = pMax - pMin;
		return d.x * d.y * d.z;
	}
	int longest() const {
		Vector diag = pMax - pMin;
		if (diag.x > diag.y && diag.x > diag.z)
			return 0;
		else if (diag.y > diag.z)
			return 1;
		else
			return 2;
	}
	Point interpolate(float tx, float ty, float tz) const {
		return Point(::interpolate<float>(tx, pMin.x, pMax.x),
			::interpolate<float>(ty, pMin.y, pMax.y),
			::interpolate<float>(tz, pMin.z, pMax.z));
	}
	Vector offset(const Point &p) const {
		return Vector((p.x - pMin.x) / (pMax.x - pMin.x),
			(p.y - pMin.y) / (pMax.y - pMin.y),
			(p.z - pMin.z) / (pMax.z - pMin.z));
	}
	void sphere(Point *c, float *rad) const;
	bool intersect(const Ray &ray, float *hitt0 = NULL, float *hitt1 = NULL) const;

	friend Box compound(const Box &b, const Point &p);
	friend Box compound(const Box &b, const Box &b2);
};

#endif