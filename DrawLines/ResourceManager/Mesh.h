#pragma once
#include <string>
#include <vector>

#include "../../CG_MATH_D3D/vector3.h"
#include "../../CG_MATH_D3D/Matrix4x3.h"
#include "../util.h"
#include "glm.h"

struct vertex
{
	vector4 pos;
	CG_MATH::vector3 normal;
	vector2 texcoord;
	color4f color;

	vertex(const float * const a):pos(a[0], a[1], a[2], 1.0f), normal(a[3], a[4], a[5]), texcoord(a[6], a[7]), color(0.0f, 0.0f, 0.0f, 1.0f){}
	vertex() :normal(CG_MATH::kZeroVector), texcoord(0.0f, 0.0f) {}

	vertex operator-(const vertex& a) const;
	vertex operator+(const vertex& a) const;
	vertex operator*(const float a) const;
};
vertex operator* (const float a, const vertex& v);


// 一个mesh的所有信息
class Mesh
{
public:

	// 构造函数
	Mesh(const std::string& path);
	~Mesh();

	// 禁止拷贝和拷贝构造
	Mesh(const Mesh &m) = delete;
	Mesh& operator=(Mesh &m) = delete;

	// Mesh 可以被移动，
	// 删除默认拷贝构造函数后， 也会删除移动构造函数，这里启用移动构造函数.
	Mesh(Mesh &&m) noexcept;

	//


	//公有变量


	// m_vertices 存储方式为：
	//|====================================
	//|		     | 3 float position       |
	//|		     --------------------------
	//| vertex 0 | 3 float normal         |
	//|          --------------------------
	//|          | 2 float textcoordinate |
	//|====================================
	//|		     | 3 float position       |
	//|		     --------------------------
	//| vertex 1 | 3 float normal         |
	//|          --------------------------
	//|          | 2 float textcoordinate |
	//|====================================
	// ....
	std::vector<float> m_vertices;
	std::vector<GLMmaterial> m_materials;

	std::vector<std::vector<unsigned int>> m_groupIndices;
	std::vector<int> m_groupMaterialID;

	CG_MATH::vector3 m_AABBmin, m_AABBmax;
	CG_MATH::vector3 m_modelCenter;

	CG_MATH::Matrix4x3 m_ObjectToWorldMatrix;

	unsigned int m_triangleNum;

	// 私有函数
private:
	void compileMesh(const GLMmodel * model);
};