#include "FPScamera.h"
#include "EulerAngles.h"
#include "MathUtil.h"
#include "RotationMatrix.h"

//移动位置

void FPScamera::move(float front, float left, float up) {

	RotationMatrix m;
	m.setup(dir);

	//需要得到相机坐标在世界坐标的表示，
	// 1， m 是从世界坐标到相机坐标的转换，m的转置表示物体旋转矩阵。
	// 2， 物体旋转矩阵的三个行即为相机X Y Z 轴在世界坐标的表示。

	pos.x += front*m.m13 - left*m.m11 + up*m.m12;
	pos.y += front*m.m23 - left*m.m21 + up*m.m22;
	pos.z += front*m.m33 - left*m.m31 + up*m.m32;

}

//转动方向
//单位为角度
// heading 绕y轴旋转 pitch 绕x轴旋转， bank 绕z轴旋转

void FPScamera::rotate2D(float heading, float pitch) {

	//角度转弧度
	heading = heading / 180.0f*kPi;
	pitch = pitch / 180.0f*kPi;


	// 修改heading， 限制范围在[-Pi, Pi]
	dir.heading += heading;
	dir.heading = wrapPi(dir.heading);

	// 修改pitch， 限制范围在[-pi/2, pi/2]
	dir.pitch += pitch;
	if (dir.pitch < -kPi * 0.45f)
		dir.pitch = -kPi * 0.45f;
	else if (dir.pitch > kPi * 0.45f)
		dir.pitch = kPi * 0.45f;

}

//获取相机矩阵

Matrix4x3 FPScamera::getMatrix() {

	Matrix4x3 m;
	m.setupParentToLocal(pos, dir);

	return m;
}

