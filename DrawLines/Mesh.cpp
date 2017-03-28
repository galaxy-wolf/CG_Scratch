#include "Mesh.h"
#include "util.h"
#include <GL/glut.h>
using namespace CG_MATH;
using namespace std;


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::drawAsLine(const CG_MATH::Matrix4x4& MVP, FrameBuffer &fbo)
{
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadIdentity();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	for (int gID = 0; gID < m_groups.size(); ++gID)
	{
		const Group & group = m_groups[gID];
		const vector<vector3> & vertices = group.m_vertices;
		const vector<unsigned int> & indices = group.m_indices;
		const vector<color4f> &colors = group.m_vertexColors;

		for (int i = 0; i < indices.size(); i += 3)
		{
			vector3 v0 = vertices[indices[i]];
			vector3 v1 = vertices[indices[i+1]];
			vector3 v2 = vertices[indices[i+2]];

			const color4f & color0 = colors[indices[i]];
			const color4f & color1 = colors[indices[i+1]];
			const color4f & color2 = colors[indices[i+2]];

			// 裁剪？ 在齐次坐标中进行裁剪
			v0 = doMVPTransform(MVP, v0);
			v1 = doMVPTransform(MVP, v1);
			v2 = doMVPTransform(MVP, v2);

			/*printf("%f %f %f\n", v0.x*0.5f+0.5f, v0.y*0.5f + 0.5f, v0.z);
			printf("%f %f %f\n", v1.x*0.5f + 0.5f, v1.y*0.5f + 0.5f, v1.z);
			printf("%f %f %f\n\n", v2.x*0.5f + 0.5f, v2.y*0.5f + 0.5f, v2.z);*/

			if (1)
			{
				fbo.drawLineInNDC(v0, color0, v1, color1);
				fbo.drawLineInNDC(v1, color1, v2, color2);
				fbo.drawLineInNDC(v2, color2, v0, color0);
			}

			// 使用OpenGL 绘制
			if(0)
			{
				// openGL 的NDC z[-1, 1] , D3d z[0, 1]
				v0.z = v0.z*2.0f - 1.0f;
				v1.z = v1.z*2.0f - 1.0f;
				v2.z = v2.z*2.0f - 1.0f;

				glColor3f(1.0f, 0.0f, 0.0f);

				glBegin(GL_LINE_LOOP);
				//glBegin(GL_TRIANGLES);
				glColor3fv(&color0.r); glVertex3fv(&v0.x);
				glColor3fv(&color1.r); glVertex3fv(&v1.x);
				glColor3fv(&color2.r); glVertex3fv(&v2.x);
				glEnd();
			}
		}
	}
}

void Mesh::drawAsFace(const CG_MATH::Matrix4x4 & MVP, FrameBuffer & fbo)
{

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadIdentity();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	for (int gID = 0; gID < m_groups.size(); ++gID)
	{
		const Group & group = m_groups[gID];
		const vector<vector3> & vertices = group.m_vertices;
		const vector<unsigned int> & indices = group.m_indices;
		const vector<color4f> &colors = group.m_vertexColors;

		for (int i = 0; i < indices.size(); i += 3)
		{
			vector3 v0 = vertices[indices[i]];
			vector3 v1 = vertices[indices[i + 1]];
			vector3 v2 = vertices[indices[i + 2]];

			const color4f & color0 = colors[indices[i]];
			const color4f & color1 = colors[indices[i + 1]];
			const color4f & color2 = colors[indices[i + 2]];

			// 裁剪？ 在齐次坐标中进行裁剪
			v0 = doMVPTransform(MVP, v0);
			v1 = doMVPTransform(MVP, v1);
			v2 = doMVPTransform(MVP, v2);

			fbo.drawTriangleInNDC(v0, color0, v1, color1, v2, color2);

			if (0) 
			{
				v0.z = v0.z*2.0f - 1.0f;
				v1.z = v1.z*2.0f - 1.0f;
				v2.z = v2.z*2.0f - 1.0f;

				glColor3f(1.0f, 0.0f, 0.0f);

				glBegin(GL_TRIANGLES);
				//glBegin(GL_TRIANGLES);
				glColor3fv(&color0.r); glVertex3fv(&v0.x);
				glColor3fv(&color1.r); glVertex3fv(&v1.x);
				glColor3fv(&color2.r); glVertex3fv(&v2.x);
				glEnd();
			}
		}
	}
}
