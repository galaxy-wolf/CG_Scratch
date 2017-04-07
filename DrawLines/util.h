#pragma once
#include "..\CG_MATH_D3D\vector3.h"
#include "..\CG_MATH_D3D\Matrix4x4.h"


struct vector2
{
	float x, y;
	vector2(float _x, float _y) :x(_x), y(_y) {}
	vector2(const CG_MATH::vector3& v) :x(v.x), y(v.y) {}
	void operator*= (float a) { x *= a; y *= a; }
};

struct vector4
{
	float x, y, z, w;
	vector4(float _x, float _y, float _z, float _w):x(_x), y(_y), z(_z), w(_w){}
	vector4(const vector3& a):x(a.x), y(a.y), z(a.z), w(1.0f){}
	vector3 div();
	void transform(CG_MATH::Matrix4x4& MVP);
};

struct color3f;

struct color4f {
	float r, g, b, a;

	color4f() = default;
	color4f(const float R, const float G, const float B, const float A) :r(R), g(G), b(B), a(A) {}
	color4f(const struct color3f& c);
};
color4f operator+(const color4f& c0, const color4f& c1);
color4f operator/(const color4f& c0, float a);
color4f operator*(const color4f& c0, float a);
color4f operator*(float a, const color4f& c0);

struct color3f {
	float r, g, b;

	color3f() = default;
	color3f(const float R, const float G, const float B):r(R), g(G), b(B){}
	color3f(const color4f& c):r(c.r), g(c.g), b(c.b){}
	color3f(const float addr[4]) :r(addr[0]), g(addr[1]), b(addr[2]){}

	color3f & operator= (const color4f c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
		return *this;
	}
};

color3f operator+ (const color3f&, const color3f&);
color3f operator- (const color3f&, const color3f&);
color3f operator* (const color3f&, const color3f&);
color3f operator/ (const color3f&, const float);
color3f operator* (const color3f&, const float);
color3f operator* (const float, const color3f&);

extern const color4f red;
extern const color4f green;

CG_MATH::vector3 doMVPTransform(const CG_MATH::Matrix4x4 &mat, const CG_MATH::vector3& v);

