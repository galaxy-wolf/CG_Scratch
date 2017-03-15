#include "FrameBuffer.h"
#include <assert.h>
#include <GL/glut.h>
#include "util.h"

using namespace CG_MATH;

FrameBuffer::FrameBuffer(const unsigned int width, const unsigned int height):m_width(width),m_height(height)
{
	assert(width != 0 && height != 0);
	
	m_colorBuffer = new color4f[width*height];
	m_depthBuffer = new float[width*height];
}

void FrameBuffer::resize(const unsigned int width, const unsigned int height)
{
	assert(width != 0 && height != 0);

	// free buffer
	if (m_colorBuffer)
	{
		delete[] m_colorBuffer;
		m_colorBuffer = NULL;
	}

	if (m_depthBuffer)
	{
		delete[] m_depthBuffer;
		m_depthBuffer = NULL;
	}


	
	m_width = width;
	m_height = height;

	m_colorBuffer = new color4f[width*height];
	m_depthBuffer = new float[width*height];
}

void FrameBuffer::clear(const color4f color, const float depth)
{
	unsigned int p = 0;
	for (unsigned int i = 0; i < m_height; ++i)
	{
		for (unsigned int j=0; j < m_width; ++j)
		{
			m_colorBuffer[p] = color;
			m_depthBuffer[p++] = depth;
		}
	}
}

void FrameBuffer::drawPoint(const CG_MATH::vector3& pos, const color4f& color)
{
	if (pos.x >= m_width || pos.y >= m_height || pos.x <0.0f || pos.y <0.0f || pos.z < 0.0f)
		return;

	unsigned int index = static_cast<unsigned int>(floorf(pos.x)) + static_cast<unsigned int>(floorf(pos.y)) * m_width;

	// 深度剔除
	if (m_depthBuffer[index] < pos.z)
		return;

	m_colorBuffer[index] = color;
	m_depthBuffer[index] = pos.z;
}

void FrameBuffer::drawPointInNormalizedCoord(const CG_MATH::vector3& pos, const color4f& color)
{
	drawPoint(vector3((pos.x + 1)*0.5f*m_width, (pos.y + 1)*0.5f*m_height, pos.z), color);
}

void FrameBuffer::display()
{
	assert(m_width >= 0 && m_height >= 0 && m_colorBuffer);

	glDrawPixels(m_width, m_height, GL_RGBA, GL_FLOAT, m_colorBuffer);
}

FrameBuffer::~FrameBuffer()
{
	if (m_colorBuffer)
	{
		delete[] m_colorBuffer;
		m_colorBuffer = NULL;
	}

	if (m_depthBuffer)
	{
		delete[] m_depthBuffer;
		m_depthBuffer = NULL;
	}
}

// 2DDA  !没有对颜色和深度进行插值。
void FrameBuffer::drawLine(const vector3 & p0, const color4f & color0, const vector3 & p1, const color4f & color1)
{
	int stepX = fabs(floorf(p1.x) - floorf(p0.x));
	int stepY = fabs(floorf(p1.y) - floorf(p0.y));

	int step = stepX > stepY ? stepX : stepY;

	float dx = (p1.x - p0.x) / step;
	float dy = (p1.y - p0.y) / step;

	float x = p0.x, y = p0.y;

	for (int i = 0; i <= step; ++i) {
		// 线性插值
		float f1 = float(i) / step;
		float f0 = float(step-i) / step;
		float z = 1.0f / (1.0f / p0.z * f0 + 1.0f / p1.z * f1);
		color4f c = (f0*color0 / p0.z + f1*color1 / p1.z) *z;
		drawPoint(vector3(x, y, z), c);
		x += dx;
		y += dy;
	}
}

void FrameBuffer::drawLineInNormalizedCoord(const vector3 & p0, const color4f & color0, const vector3 & p1, const color4f & color1) 
{
	drawLine(vector3((p0.x + 1.0f)*0.5f*m_width, (p0.y + 1.0f)*0.5f*m_height, p0.z), color0, vector3((p1.x + 1.0f)*0.5f*m_width, (p1.y + 1.0f)*0.5f*m_height, p1.z), color1);
}


