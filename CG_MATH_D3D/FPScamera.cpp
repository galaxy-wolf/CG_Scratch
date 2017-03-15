#include "FPScamera.h"
#include "EulerAngles.h"
#include "MathUtil.h"
#include "RotationMatrix.h"

//�ƶ�λ��

void FPScamera::move(float front, float left, float up) {

	RotationMatrix m;
	m.setup(dir);

	//��Ҫ�õ������������������ı�ʾ��
	// 1�� m �Ǵ��������굽��������ת����m��ת�ñ�ʾ������ת����
	// 2�� ������ת����������м�Ϊ���X Y Z ������������ı�ʾ��

	pos.x += front*m.m13 - left*m.m11 + up*m.m12;
	pos.y += front*m.m23 - left*m.m21 + up*m.m22;
	pos.z += front*m.m33 - left*m.m31 + up*m.m32;

}

//ת������
//��λΪ�Ƕ�
// heading ��y����ת pitch ��x����ת�� bank ��z����ת

void FPScamera::rotate2D(float heading, float pitch) {

	//�Ƕ�ת����
	heading = heading / 180.0f*kPi;
	pitch = pitch / 180.0f*kPi;


	// �޸�heading�� ���Ʒ�Χ��[-Pi, Pi]
	dir.heading += heading;
	dir.heading = wrapPi(dir.heading);

	// �޸�pitch�� ���Ʒ�Χ��[-pi/2, pi/2]
	dir.pitch += pitch;
	if (dir.pitch < -kPi * 0.45f)
		dir.pitch = -kPi * 0.45f;
	else if (dir.pitch > kPi * 0.45f)
		dir.pitch = kPi * 0.45f;

}

//��ȡ�������

Matrix4x3 FPScamera::getMatrix() {

	Matrix4x3 m;
	m.setupParentToLocal(pos, dir);

	return m;
}

