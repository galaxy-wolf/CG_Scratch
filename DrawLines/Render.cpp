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

void clipAndDrawLine(vertex A, vertex C, FrameBuffer &fbo)
{

	float tIn = 0.0f, tOut = 1.0f, tHit;
	float aBC[6], cBC[6];
	int aOutcode = 0, cOutcode = 0;

	aBC[0] = A.pos.w + A.pos.x;
	aBC[1] = A.pos.w - A.pos.x;
	aBC[2] = A.pos.w + A.pos.y;
	aBC[3] = A.pos.w - A.pos.y;
	aBC[4] = A.pos.w + A.pos.z;
	aBC[5] = A.pos.w - A.pos.z;

	cBC[0] = C.pos.w + C.pos.x;
	cBC[1] = C.pos.w - C.pos.x;
	cBC[2] = C.pos.w + C.pos.y;
	cBC[3] = C.pos.w - C.pos.y;
	cBC[4] = C.pos.w + C.pos.z;
	cBC[5] = C.pos.w - C.pos.z;

	for (int i = 0; i < 6; ++i)
	{
		aOutcode |= (aBC[i] > 0 ? 0 : 1) << i;
		cOutcode |= (cBC[i] > 0 ? 0 : 1) << i;
	}
	
	if ((aOutcode & cOutcode) != 0) // trivial reject
		return;

	if ((aOutcode | cOutcode) == 0) // trivial accept
	{
		fbo.drawLineInNDC(A.pos.div(), A.color, C.pos.div(), C.color);
		return;
	}

	for (int i = 0; i < 6; ++i) // clip against each plane
	{
		if (cBC[i] < 0) // exit: C is outside
		{
			tHit = aBC[i] / (aBC[i] - cBC[i]);
			tOut = fminf(tOut, tHit);
		}
		else if (aBC[i] < 0) // enters: A is outside
		{
			tHit = aBC[i] / (aBC[i] - cBC[i]);
			tIn = fmaxf(tIn, tHit);
		}
		if (tIn > tOut) return; // CI is empty; early out
	}
	
	vertex tempA = A, tempC = C;

	if (aOutcode != 0) // A is out: tIn has changed;
	{
		A = tempA + tIn * (tempC - tempA);
	}

	if (cOutcode != 0) // C is out: tOut has changed;
	{
		C = tempA + tOut * (tempC - tempA);
	}

	fbo.drawLineInNDC(A.pos.div(), A.color, C.pos.div(), C.color);
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
		}
	}
}

void Render::drawAsFace(const Mesh& mesh, const RenderOption& op, FrameBuffer &fbo) const 
{

	//MeshManager & MM = MeshManager::getInstance();

	//for (int gid = 0; gid < mesh.m_groupIndices.size(); ++gid)
	//{
	//	const std::vector<unsigned int>& indices = mesh.m_groupIndices[gid];
	//	const std::vector<float>& vertices = mesh.m_vertices;
	//	const GLMmaterial& material = mesh.m_materials[mesh.m_groupMaterialID[gid]];

	//	for (int vid = 0; vid < indices.size(); vid += 3)
	//	{
	//		vertex v0(&vertices[indices[vid] * MM.m_vtxSize]);
	//		vertex v1(&vertices[indices[vid + 1] * MM.m_vtxSize]);
	//		vertex v2(&vertices[indices[vid + 2] * MM.m_vtxSize]);

	//		color4f color0 =
	//			PhongReflectionModel(
	//				op.light,
	//				material.ambient, material.diffuse, material.specular, material.shininess,
	//				v0.pos*mesh.m_ObjectToWorldMatrix,
	//				v0.normal*mesh.m_ObjectToWorldMatrix,
	//				op.cameraPos);

	//		color4f color1 =
	//			PhongReflectionModel(
	//				op.light,
	//				material.ambient, material.diffuse, material.specular, material.shininess,
	//				v1.pos*mesh.m_ObjectToWorldMatrix,
	//				v1.normal*mesh.m_ObjectToWorldMatrix,
	//				op.cameraPos);

	//		color4f color2 =
	//			PhongReflectionModel(
	//				op.light,
	//				material.ambient, material.diffuse, material.specular, material.shininess,
	//				v2.pos*mesh.m_ObjectToWorldMatrix,
	//				v2.normal*mesh.m_ObjectToWorldMatrix,
	//				op.cameraPos);

	//		// 裁剪？ 在齐次坐标中进行裁剪

	//		Matrix4x4 VP = op.viewMatrix*op.projectMatrix;

	//		v0.pos = doMVPTransform(VP, v0.pos);
	//		v1.pos = doMVPTransform(VP, v1.pos);
	//		v2.pos = doMVPTransform(VP, v2.pos);

	//		fbo.drawTriangleInNDC(v0.pos, color0, v1.pos, color1, v2.pos, color2);
	//	}
	//}

}
