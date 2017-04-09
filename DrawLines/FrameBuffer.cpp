#include "FrameBuffer.h"
#include <assert.h>
#include <GL/glut.h>
#include "util.h"
#include "ResourceManager\TextureManger.h"

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

void FrameBuffer::drawPointInNDC(const CG_MATH::vector3& pos, const color4f& color)
{
	drawPoint(NDCspaceToRasterSpace(pos), color);
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

// 2DDA  
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

void FrameBuffer::drawLineInNDC(const vector3 & p0, const color4f & color0, const vector3 & p1, const color4f & color1) 
{
	return drawLine(NDCspaceToRasterSpace(p0), color0, NDCspaceToRasterSpace(p1), color1);
}

float FrameBuffer::edgeFunctionCW(const vector2 & A, const vector2 & B, const vector2 & P)
{
	return (P.x - A.x) * (B.y - A.y) - (P.y - A.y) * (B.x - A.x);
}

float FrameBuffer::edgeFunctionCCW(const vector2 & A, const vector2 & B, const vector2 & P)
{
	return -1.0f * edgeFunctionCW(A, B, P);
}

/// 画三角形
void FrameBuffer::drawTriangleInNDC(const CG_MATH::vector3& v0, const color4f &color0, const vector2& tc0,
	const CG_MATH::vector3& v1, const color4f &color1, const vector2& tc1,
	const CG_MATH::vector3& v2, const color4f &color2, const vector2& tc2, const char * texName)
{
	return drawTriangle(
			NDCspaceToRasterSpace(v0), color0, tc0,
			NDCspaceToRasterSpace(v1), color1, tc1,
			NDCspaceToRasterSpace(v2), color2, tc2,
			texName
		);
}

void minAndMax(float &min, float &max, const float a, const float b, const float c)
{
	if (a < b)
	{
		min = a;
		max = b;		
	}
	else
	{
		min = b;
		max = a;
	}

	if (c < min)
		min = c;
	else if (c > max)
		max = c;
}

void FrameBuffer::drawTriangle(const CG_MATH::vector3& v0, const color4f &color0, const vector2& tc0,
	const CG_MATH::vector3& v1, const color4f &color1, const vector2& tc1,
	const CG_MATH::vector3& v2, const color4f &color2, const vector2& tc2,
	const char * texName) 
{

	// 求三角形覆盖的2D包围盒。考虑了 x=1，x=-1, y=1, y=-1 四个裁剪面的裁剪。
	float xMin = 0, xMax = m_width, yMin = 0, yMax = m_height;

	minAndMax(xMin, xMax, v0.x, v1.x, v2.x);
	minAndMax(yMin, yMax, v0.y, v1.y, v2.y);

	xMin = fmaxf(0, xMin);
	yMin = fmaxf(0, yMin);
	xMax = fminf(xMax, m_width);
	yMax = fminf(yMax, m_height);

	if (xMin >= xMax || yMin >= yMax)
		return;


	float area = edgeFunctionCW(v0, v1, v2);
	vector2 edge0 = v2 - v1;
	vector2 edge1 = v0 - v2;
	vector2 edge2 = v1 - v0;

	
	// 0:CW
	// 1:CCW
	int windingOrder = 0; 

	if (area == 0)
		return;

	if (area < 0)
	{
		windingOrder = 1;
		area *= -1.0f;
		edge0 *= -1.0f;
		edge1 *= -1.0f;
		edge2 *= -1.0f;
	}

	color4f color0OverZ0 = color0 / v0.z;
	color4f color1OverZ1 = color1 / v1.z;
	color4f color2OverZ2 = color2 / v2.z;

	if (tc0.x > 1.0f || tc0.x < 0.0f || tc0.y > 1.0f || tc0.y < 0.0f)
		std::cout << tc0.x << " " << tc0.y<<std::endl;
	if (tc1.x > 1.0f || tc1.x < 0.0f || tc1.y > 1.0f || tc1.y < 0.0f)
		std::cout << tc1.x << " " << tc1.y << std::endl;
	if (tc2.x > 1.0f || tc2.x < 0.0f || tc2.y > 1.0f || tc2.y < 0.0f)
		std::cout << tc2.x << " " << tc2.y << std::endl;

	vector2 tc0OverZ0 = tc0 / v0.z;
	vector2 tc1OverZ1 = tc1 / v1.z;
	vector2 tc2OverZ2 = tc2 / v2.z;

	float oneOverZ0 = 1.0f / v0.z;
	float oneOverZ1 = 1.0f / v1.z;
	float oneOverZ2 = 1.0f / v2.z;

	for (int y = floorf(yMin); y < ceilf(yMax); ++y) {
		for (int x = floorf(xMin); x < ceilf(xMax); ++x)
		{
			vector2 p(x + 0.5f, y + 0.5f);
			float w0, w1, w2;

			w0 = edgeFunctionCW(v1, v2, p);
			w1 = edgeFunctionCW(v2, v0, p);
			w2 = edgeFunctionCW(v0, v1, p);

			if (1 == windingOrder) {// CCW
				w0 *= -1.0f;
				w1 *= -1.0f;
				w2 *= -1.0f;
			}

			bool overlaps = true;

			// If the point is on the edge, test if it is a top or left edge, 
			// otherwise test if  the edge function is positive
			overlaps &= (w0 == 0 ? ((edge0.y == 0 && edge0.x > 0) || edge0.y > 0) : (w0 > 0));
			overlaps &= (w1 == 0 ? ((edge1.y == 0 && edge1.x > 0) || edge1.y > 0) : (w1 > 0));
			overlaps &= (w2 == 0 ? ((edge2.y == 0 && edge2.x > 0) || edge2.y > 0) : (w2 > 0));

			if (overlaps) {
				w0 /= area;
				w1 /= area;
				w2 /= area;

				float z = 1.0f/(oneOverZ0*w0 + oneOverZ1* w1 + oneOverZ2*w2);
				color4f color = (color0OverZ0*w0 + color1OverZ1*w1 + color2OverZ2*w2) *z;

				if (texName[0] != '\0') // 如果有纹理，读取纹理，并和颜色值相乘，得到最终颜色。
				{
					vector2 tc = (tc0OverZ0*w0 + tc1OverZ1*w1 + tc2OverZ2*w2) *z;
					color3f texColor;

					tc.x = fminf(fmaxf(0.0f, tc.x), 1.0f);
					tc.y = fminf(fmaxf(0.0f, tc.y), 1.0f);

					IplImage* image = TextureManager::getInstance().getImage(texName);
					int posX = (tc.x) * (image->width - 1);
					int posY = (1.0f-tc.y) * (image->height - 1);

					/*if (tc.x > 1.0f || tc.x < 0.0f || tc.y > 1.0f || tc.y < 0.0f)
					{
						std::cout << "tc " << tc.x << " " << tc.y << std::endl;
						std::cout << w0 <<" " <<w1<<" "<<w2<<" " << w0 + w1 + w2<<std::endl;
						std::cout << v0.z << " " << v1.z << " " << v2.z << " " << z << std::endl;
						std::cout << "tc0 " << tc0.x << " " << tc0.y << std::endl;
						std::cout << "tc1 " << tc1.x << " " << tc1.y << std::endl;
						std::cout << "tc2 " << tc2.x << " " << tc2.y << std::endl;
						std::cout << std::endl;
					}*/

					texColor.b = (unsigned char)(image->imageData[((posY*image->width) + posX) * 3 + 0]) / 255.0f;
					texColor.g = (unsigned char)(image->imageData[((posY*image->width) + posX) * 3 + 1]) / 255.0f;
					texColor.r = (unsigned char)(image->imageData[((posY*image->width) + posX) * 3 + 2]) / 255.0f;

					color = texColor * color;
				}
				
				drawPoint(vector3(p.x, p.y, z), color);
			}

		}
	}
	
}