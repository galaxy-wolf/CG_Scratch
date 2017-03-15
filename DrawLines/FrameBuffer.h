#pragma once
#include "vector3.h"
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
	void drawPointInNormalizedCoord(const CG_MATH::vector3& pos, const color4f& color);

	void display();

	/// 绘制基本图元； 这里只是完成 Fragment 工作
	void drawLine(const CG_MATH::vector3& p0, const color4f &color0, const CG_MATH::vector3 &p1, const color4f& color1);
	void drawLineInNormalizedCoord(const CG_MATH::vector3& p0, const color4f &color0, const CG_MATH::vector3 &p1, const color4f& color1);
	//void drawTriangle();

private:
	unsigned int m_width;
	unsigned int m_height;

	color4f * m_colorBuffer;
	float * m_depthBuffer;

};

