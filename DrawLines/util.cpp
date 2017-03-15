#include "util.h"
using namespace CG_MATH;


const color4f red = color4f(1.0f, 0.0f, 0.0f, 1.0f);
const color4f green = color4f(0.0f, 1.0f, 0.0f, 1.0f);

color4f operator+(const color4f & c0, const color4f & c1)
{
	return color4f(c0.r+c1.r, c0.g+c1.g, c0.b+c1.b, c0.a+c1.a);
}

color4f operator/(const color4f & c0, float a)
{
	return color4f(c0.r/a, c0.g/a, c0.b/a, c0.a/a);
}

color4f operator*(const color4f & c0, float a)
{
	return color4f(c0.r * a, c0.g * a, c0.b * a, c0.a * a);
}

color4f operator*(float a, const color4f & c0)
{
	return c0*a;
}

vector3 doMVPTransform(const Matrix4x4 &mat, const vector3& v)
{
	vector3 res(0.0f, 0.0f, 0.0f);

	res.x = v.x * mat.m11 + v.y * mat.m21 + v.z * mat.m31 + mat.m41;
	res.y = v.x * mat.m12 + v.y * mat.m22 + v.z * mat.m32 + mat.m42;
	res.z = v.x * mat.m13 + v.y * mat.m23 + v.z * mat.m33 + mat.m43;
	float w = v.x * mat.m14 + v.y * mat.m24 + v.z * mat.m34 + mat.m44;

	return res / w;
}