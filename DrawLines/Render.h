#pragma once

#include <vector>
#include "vector3.h"
#include "util.h"
#include "..\CG_MATH_D3D\Matrix4x4.h"
#include "FrameBuffer.h"
#include "DistantLight.h"
#include "ResourceManager\Mesh.h"


struct RenderOption
{
public:
	CG_MATH::Matrix4x4 projectMatrix;
	CG_MATH::Matrix4x3 viewMatrix;
	CG_MATH::vector3 cameraPos;
	DistantLight light;


	RenderOption(const CG_MATH::Matrix4x4& project, const CG_MATH::Matrix4x3& view, const CG_MATH::vector3& camerPosition, const DistantLight & L) :
		projectMatrix(project), viewMatrix(view), cameraPos(camerPosition), light(L) {}
};

class Render
{

private:
	Render() = default;

	~Render() {}

public:

	// 禁止拷贝构造和赋值

	Render(const Render& t) = delete;
	Render& operator=(const Render& t) = delete;

	// 获得实例

	static Render& getInstance() {

		static Render Instance;

		return Instance;
	}

	void drawAsLine(const Mesh& m, const RenderOption& op, FrameBuffer &fbo) const;
	void drawAsFace(const Mesh& m, const RenderOption& op, FrameBuffer &fbo) const;
};



