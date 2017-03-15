#pragma once

#include <vector>
#include "vector3.h"
#include "util.h"
#include "..\CG_MATH_D3D\Matrix4x4.h"
#include "FrameBuffer.h"

struct Group
{
public:
	std::vector<CG_MATH::vector3> m_vertices;
	std::vector<color4f> m_vertexColors;
	std::vector<unsigned int> m_indices;

	// phone shading params
	color4f m_ambient;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void drawAsLine(const CG_MATH::Matrix4x4& MVP, FrameBuffer &fbo);
	std::vector<Group> m_groups;
};



