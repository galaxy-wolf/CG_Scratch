#pragma once
#include "..\CG_MATH_D3D\vector3.h"
#include "..\CG_MATH_D3D\Matrix4x4.h"

struct point2f {
	float x, y;

	point2f() = default;
	point2f(const CG_MATH::vector3 & v3) :x(v3.x), y(v3.y) {};

	point2f(const float _x, const float _y) : x(_x), y(_y) {};
};

struct color4f {
	float r, g, b, a;

	color4f() = default;
	color4f(const float R, const float G, const float B, const float A) :r(R), g(G), b(B), a(A) {}
};
color4f operator+(const color4f& c0, const color4f& c1);
color4f operator/(const color4f& c0, float a);
color4f operator*(const color4f& c0, float a);
color4f operator*(float a, const color4f& c0);

extern const color4f red;
extern const color4f green;

CG_MATH::vector3 doMVPTransform(const CG_MATH::Matrix4x4 &mat, const CG_MATH::vector3& v);

