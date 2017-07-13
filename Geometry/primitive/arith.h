#ifndef RWE_GEOMETRY_PRIMITIVE_ARITH
#define RWE_GEOMETRY_PRIMITIVE_ARITH
#include "Frame/main/main.h"

class Point;
class Vector;
class Box;
class Ray;

class Vector {
public:
	float x, y, z;

	Vector() { x = y = z = 0.f; }
	Vector(float xx, float yy, float zz)
		: x(xx), y(yy), z(zz) {};
	explicit Vector(const Point &p);
	Vector(const Vector &v) {}

	Vector &operator=(const Vector &v) {
		x = v.x; y = v.y; z = v.z;
		return *this;
	}
	Vector operator+(const Vector &v) const {
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector& operator+=(const Vector &v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector operator-(const Vector &v) const {
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	Vector& operator-=(const Vector &v) {
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Vector operator*(float f) const { return Vector(f*x, f*y, f*z); }
	Vector &operator*=(float f) {
		x *= f; y *= f; z *= f;
		return *this;
	}
	Vector operator/(float f) const {
		float inv = 1.f / f;
		return Vector(x * inv, y * inv, z * inv);
	}
	Vector &operator/=(float f) {
		float inv = 1.f / f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}
	Vector operator-() const { return Vector(-x, -y, -z); }
	float operator[](int i) const {
		if (i >= 0 && i <= 2)
			return (&x)[i];
		else return 0.f;
	}
	bool operator==(const Vector &v) const {
		return x == v.x && y == v.y && z == v.z;
	}
	bool operator!=(const Vector &v) const {
		return x != v.x || y != v.y || z != v.z;
	}

	float square() const { return x*x + y*y + z*z; }
	float length() const { return sqrtf(square()); }
	Vector normal() const { return *this / square(); }
	Vector &normalize() { return *this /= square(); }
};

class Point {
public:
	float x, y, z;

	Point() { x = y = z = 0.f; }
	Point(float xx, float yy, float zz)
		: x(xx), y(yy), z(zz) {
	}
	Point(const Point &p) {
		x = p.x; y = p.y; z = p.z;
	}

	Point &operator=(const Point &p) {
		x = p.x; y = p.y; z = p.z;
		return *this;
	}
	Point operator+(const Vector &v) const {
		return Point(x + v.x, y + v.y, z + v.z);
	}
	Point &operator+=(const Vector &v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector operator-(const Point &p) const {
		return Vector(x - p.x, y - p.y, z - p.z);
	}
	Point operator-(const Vector &v) const {
		return Point(x - v.x, y - v.y, z - v.z);
	}
	Point &operator-=(const Vector &v) {
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Point &operator+=(const Point &p) {
		x += p.x; y += p.y; z += p.z;
		return *this;
	}
	Point operator+(const Point &p) const {
		return Point(x + p.x, y + p.y, z + p.z);
	}
	Point operator* (float f) const {
		return Point(f*x, f*y, f*z);
	}
	Point &operator*=(float f) {
		x *= f; y *= f; z *= f;
		return *this;
	}
	Point operator/ (float f) const {
		float inv = 1.f / f;
		return Point(inv*x, inv*y, inv*z);
	}
	Point &operator/=(float f) {
		float inv = 1.f / f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}
	float operator[](int i) const {
		if (i >= 0 && i <= 2)
			return (&x)[i];
		else return 0.f;
	}
	bool operator==(const Point &p) const {
		return x == p.x && y == p.y && z == p.z;
	}
	bool operator!=(const Point &p) const {
		return x != p.x || y != p.y || z != p.z;
	}
};

#endif