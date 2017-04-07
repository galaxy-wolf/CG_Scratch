#pragma once

#include "Matrix4x3.h"

namespace CG_MATH
{
	class Matrix4x3;

	//----------------------------------------------------------------------------------------
	//Matrix4x4��
	//ʵ��4x4ת������

	class Matrix4x4
	{
	public:

		// ��������

		//�����ֵ
		
		float m11, m12, m13, m14;
		float m21, m22, m23, m24;
		float m31, m32, m33, m34;
		float m41, m42, m43, m44;

		// ��������

		//��Ϊ��λ����

		void identity();

		// ��3x4������
		
		Matrix4x4(const Matrix4x3 &a) {

			// ֱ�Ӹ�ֵ

			m11 = a.m11; m12 = a.m12; m13 = a.m13; m14 = 0.0f;
			m21 = a.m21; m22 = a.m22; m23 = a.m23; m24 = 0.0f;
			m31 = a.m31; m32 = a.m32; m33 = a.m33; m34 = 0.0f;
			m41 = a.tx;  m42 = a.ty;  m43 = a.tz;  m44 = 1.0f;
		}

		// Ĭ�Ϲ��캯��
		
		Matrix4x4() = default;

		// ����ͶӰ�任����

		void setupPerspective(float fov, float aspect, float near, float far);
		void setupFrustum(float left, float right, float bottom, float top, float near, float far);

	};

	// �����* �������Ӿ��󣬳˷���˳��������ң���任˳���෴��

	Matrix4x4 operator*(const Matrix4x4 &a, const Matrix4x4 &m);

	// �����*= ���ֺ�c++��׼�﷨һ�¡�

	Matrix4x4 &operator*= (Matrix4x4 &a, const Matrix4x4 &m);

	// ����������

	Matrix4x4 inverse(const Matrix4x4 &m);

}