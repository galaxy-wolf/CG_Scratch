#pragma once
#include "..\CG_MATH_D3D\vector3.h"
#include "util.h"



class FrameBuffer
{
public:
	FrameBuffer(const unsigned int width, const unsigned int height);
	~FrameBuffer();

	FrameBuffer(const FrameBuffer &) = delete;
	FrameBuffer(const FrameBuffer&&) = delete;
	FrameBuffer operator=(const FrameBuffer &) = delete;

	void resize(const unsigned int width, const unsigned int height);

	void clear(const color4f color, const float depth);

	void drawPoint(const CG_MATH::vector3& pos, const color4f& color);
	void drawPointInNDC(const CG_MATH::vector3& pos, const color4f& color);

	void display();

	/// ���ƻ���ͼԪ�� ����ֻ����� Fragment ����
	void drawLine(const CG_MATH::vector3& p0, const color4f &color0, const CG_MATH::vector3 &p1, const color4f& color1);
	void drawLineInNDC(const CG_MATH::vector3& p0, const color4f &color0, const CG_MATH::vector3 &p1, const color4f& color1);
	void drawTriangle(const CG_MATH::vector3& v0, const color4f &color0,
					  const CG_MATH::vector3& v1, const color4f &color1, 
					  const CG_MATH::vector3& v2, const color4f &color2);

	void drawTriangleInNDC(const CG_MATH::vector3& v0, const color4f &color0,
		const CG_MATH::vector3& v1, const color4f &color1,
		const CG_MATH::vector3& v2, const color4f &color2);


private:
// ˽�з�����
	float edgeFunctionCW(const vector2 &A, const vector2 &B, const vector2 &P);
	float edgeFunctionCCW(const vector2 &A, const vector2 &B, const vector2 &P);

	CG_MATH::vector3 NDCspaceToRasterSpace(const CG_MATH::vector3& v)
	{
		return CG_MATH::vector3((v.x + 1.0f)*0.5f*m_width, (v.y + 1.0f)*0.5f*m_height, v.z);
	}

private:
	unsigned int m_width;
	unsigned int m_height;

	color4f * m_colorBuffer;
	float * m_depthBuffer;

};

