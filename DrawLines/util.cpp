#include "util.h"
using namespace CG_MATH;


const color4f red = color4f(1.0f, 0.0f, 0.0f, 1.0f);
const color4f green = color4f(0.0f, 1.0f, 0.0f, 1.0f);

color4f::color4f(const struct color3f& c) :r(c.r), g(c.g), b(c.b), a(1.0f) {}

vector2 operator*(const float a, const vector2 & v)
{
	return vector2(a*v.x, a*v.y);
}

vector4 operator*(const float a, const vector4 & v)
{
	return vector4(a*v.x, a*v.y, a*v.z, a*v.w);
}

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
	return color4f(c0.r*a, c0.g*a, c0.b*a, c0.a*a);
}

color4f operator*(float a, const color4f & c0)
{
	return c0*a;
}

color3f operator+(const color3f &c0, const color3f &c1)
{
	return color3f(
		c0.r+c1.r,
		c0.g+c1.g,
		c0.b+c1.b
	);
}

color3f operator-(const color3f &c0, const color3f &c1)
{
	return color3f(
		c0.r - c1.r,
		c0.g - c1.g,
		c0.b - c1.b
	);
}

color3f operator*(const color3f &c0, const color3f &c1)
{
	return color3f(
		c0.r * c1.r,
		c0.g * c1.g,
		c0.b * c1.b
	);
}

color3f operator/(const color3f &c, const float a)
{
	return color3f(
		c.r / a,
		c.g / a,
		c.b / a
	);
}

color3f operator*(const color3f &c, const float a)
{
	return color3f(
		c.r * a,
		c.g * a,
		c.b * a
	);
}

color3f operator*(const float a, const color3f &c)
{
	return c*a;
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

vector3 vector4::div()
{
	return vector3(x/w, y/w, z/w);
}

void vector4::transform(CG_MATH::Matrix4x4 & MVP)
{
	float _x = x, _y = y, _z = z, _w = w;
	x = _x * MVP.m11 + _y * MVP.m21 + _z * MVP.m31 + _w * MVP.m41;
	y = _x * MVP.m12 + _y * MVP.m22 + _z * MVP.m32 + _w * MVP.m42;
	z = _x * MVP.m13 + _y * MVP.m23 + _z * MVP.m33 + _w * MVP.m43;
	w = _x * MVP.m14 + _y * MVP.m24 + _z * MVP.m34 + _w * MVP.m44;
}
