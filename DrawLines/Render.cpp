#include "Render.h"
#include "util.h"
#include <GL/glut.h>
#include "DistantLight.h"
#include "ResourceManager\MeshManager.h"

using namespace CG_MATH;
using namespace std;

vector3 reflect(const vector3& L, const vector3& N)
{
	return 2 * (L*N)*N - L;
}

color3f PhongReflectionModel(
	const DistantLight& light,
	const color3f& ka,
	const color3f& kd,
	const color3f& ks,
	const float shiness,
	const vector3& worldPos,
	const vector3& worldNormal,
	const vector3& cameraPos
)
{
	color3f diffuse, specular;
	vector3 V = cameraPos - worldPos;
	V.normalize();
	vector3 R = reflect(-light.dir, worldNormal);

	diffuse = light.id * fmaxf(0, worldNormal * (-light.dir));
	specular = light.is * powf(fmaxf(0, R * V), shiness);

	return ka * light.ia + kd * diffuse + ks * specular;
}

void clipAndDrawLine(vertex a, vertex b, FrameBuffer &fbo)
{
	bool f1, f2;

	// x = -1;
	f1 = a.pos.w + a.pos.x > 0;
	f2 = b.pos.w + b.pos.x > 0;

	if (!f1 && !f2) // 改直线被剔除
		return;

	if (!f1 && f2) // 使用交点Q替换a
	{
		float t = (a.x+a.w)
	}
	else if (f1 && !f2) // 使用交点Q替换b
	{

	}



	// x = 1;
	if (!f1 && !f2) // 改直线被剔除
		return;

	if (!f1 && f2) // 使用交点Q替换a
	{

	}
	else if (f1 && !f2) // 使用交点Q替换b
	{

	}


	// y = -1;
	if (!f1 && !f2) // 改直线被剔除
		return;

	if (!f1 && f2) // 使用交点Q替换a
	{

	}
	else if (f1 && !f2) // 使用交点Q替换b
	{

	}


	// y = 1;
	if (!f1 && !f2) // 改直线被剔除
		return;

	if (!f1 && f2) // 使用交点Q替换a
	{

	}
	else if (f1 && !f2) // 使用交点Q替换b
	{

	}
	// z = -1;
	if (!f1 && !f2) // 改直线被剔除
		return;

	if (!f1 && f2) // 使用交点Q替换a
	{

	}
	else if (f1 && !f2) // 使用交点Q替换b
	{

	}
	// z = 1;
	if (!f1 && !f2) // 改直线被剔除
		return;

	if (!f1 && f2) // 使用交点Q替换a
	{

	}
	else if (f1 && !f2) // 使用交点Q替换b
	{

	}
	fbo.drawLineInNDC(a.pos.div(), a.color, b.pos.div(), b.color);
}

void Render::drawAsLine(const Mesh& mesh, const RenderOption& op, FrameBuffer &fbo) const
{
	MeshManager & MM = MeshManager::getInstance();

	for (int gid = 0; gid < mesh.m_groupIndices.size(); ++gid)
	{
		const std::vector<unsigned int>& indices = mesh.m_groupIndices[gid];
		const std::vector<float>& vertices = mesh.m_vertices;
		const GLMmaterial& material = mesh.m_materials[mesh.m_groupMaterialID[gid]];

		for (int vid = 0; vid < indices.size(); vid += 3)
		{
			vertex v0(&vertices[indices[vid] * MM.m_vtxSize]);
			vertex v1(&vertices[indices[vid + 1] * MM.m_vtxSize]);
			vertex v2(&vertices[indices[vid + 2] * MM.m_vtxSize]);

			v0.color = 
				PhongReflectionModel(
					op.light,
					material.ambient, material.diffuse, material.specular, material.shininess,
					v0.pos.div()*mesh.m_ObjectToWorldMatrix,
					v0.normal*mesh.m_ObjectToWorldMatrix,
					op.cameraPos);

			v1.color = 
				PhongReflectionModel(
					op.light,
					material.ambient, material.diffuse, material.specular, material.shininess,
					v1.pos.div()*mesh.m_ObjectToWorldMatrix,
					v1.normal*mesh.m_ObjectToWorldMatrix,
					op.cameraPos);

			v2.color =
				PhongReflectionModel(
					op.light,
					material.ambient, material.diffuse, material.specular, material.shininess,
					v2.pos.div()*mesh.m_ObjectToWorldMatrix,
					v2.normal*mesh.m_ObjectToWorldMatrix,
					op.cameraPos);

			Matrix4x4 VP = op.viewMatrix*op.projectMatrix;

			v0.pos.transform(VP);
			v1.pos.transform(VP);
			v2.pos.transform(VP);

			clipAndDrawLine(v0, v1, fbo);
			clipAndDrawLine(v1, v2, fbo);
			clipAndDrawLine(v2, v0, fbo);

			//fbo.drawLineInNDC(v0.pos, color0, v1.pos, color1);
			//fbo.drawLineInNDC(v1.pos, color1, v2.pos, color2);
			//fbo.drawLineInNDC(v2.pos, color2, v0.pos, color0);
			
		}
	}
}

void Render::drawAsFace(const Mesh& mesh, const RenderOption& op, FrameBuffer &fbo) const 
{

	MeshManager & MM = MeshManager::getInstance();

	for (int gid = 0; gid < mesh.m_groupIndices.size(); ++gid)
	{
		const std::vector<unsigned int>& indices = mesh.m_groupIndices[gid];
		const std::vector<float>& vertices = mesh.m_vertices;
		const GLMmaterial& material = mesh.m_materials[mesh.m_groupMaterialID[gid]];

		for (int vid = 0; vid < indices.size(); vid += 3)
		{
			vertex v0(&vertices[indices[vid] * MM.m_vtxSize]);
			vertex v1(&vertices[indices[vid + 1] * MM.m_vtxSize]);
			vertex v2(&vertices[indices[vid + 2] * MM.m_vtxSize]);

			color4f color0 =
				PhongReflectionModel(
					op.light,
					material.ambient, material.diffuse, material.specular, material.shininess,
					v0.pos*mesh.m_ObjectToWorldMatrix,
					v0.normal*mesh.m_ObjectToWorldMatrix,
					op.cameraPos);

			color4f color1 =
				PhongReflectionModel(
					op.light,
					material.ambient, material.diffuse, material.specular, material.shininess,
					v1.pos*mesh.m_ObjectToWorldMatrix,
					v1.normal*mesh.m_ObjectToWorldMatrix,
					op.cameraPos);

			color4f color2 =
				PhongReflectionModel(
					op.light,
					material.ambient, material.diffuse, material.specular, material.shininess,
					v2.pos*mesh.m_ObjectToWorldMatrix,
					v2.normal*mesh.m_ObjectToWorldMatrix,
					op.cameraPos);

			// 裁剪？ 在齐次坐标中进行裁剪

			Matrix4x4 VP = op.viewMatrix*op.projectMatrix;

			v0.pos = doMVPTransform(VP, v0.pos);
			v1.pos = doMVPTransform(VP, v1.pos);
			v2.pos = doMVPTransform(VP, v2.pos);

			fbo.drawTriangleInNDC(v0.pos, color0, v1.pos, color1, v2.pos, color2);
		}
	}

}
