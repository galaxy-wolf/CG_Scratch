#pragma once

namespace CG_MATH
{

	class vector3;
	class EulerAngles;
	class Quaternion;
	class RotationMatrix;

	//--------------------------------------------------------------------------------------
	//Matrix4*3��
	// ʵ��3*4ת�������ܹ�����κ�3D����任

	class Matrix4x3
	{
	public:
		
		// ��������

		// �����ֵ
		//ǰ3�а������Ա任�����һ�а���ƽ��

		float m11, m12, m13;
		float m21, m22, m23;
		float m31, m32, m33;
		float tx, ty, tz;

		// ��������

		//��Ϊ��λ����

		void identity();

		// ֱ�ӷ���ƽ�Ʋ���

		void zeroTanslation();
		void setTranslation(const vector3 &d);
		void setupTanslation(const vector3 &d);

		// ����ִ�и��ռ� <->�ֲ��ռ�任�ľ��󣬼ζ��ֲ��ռ���ָ����λ�úͷ�λ�� �÷�λ������ʹ��ŷ���ǻ���ת�����ʾ�ġ�

		void setupLocalToParent(const vector3 &pos, const EulerAngles &orient);
		void setupLocalToParent(const vector3 &pos, const RotationMatrix &orient);
		void setupParentToLocal(const vector3 &pos, const EulerAngles &orient);
		void setupParentToLocal(const vector3 &pos, const RotationMatrix &orient);

		// ��������������ת�ľ���

		void setupRotate(int axis, float theta);

		// ��������������ת�ľ���

		void setupRotate(const vector3 &axis, float theta);

		//������ת���󣬽�λ������Ԫ����ʽ����

		void fromQuaternion(const Quaternion &q);

		// ���������������ŵľ���

		void setupScale(const vector3 &s);

		// ���������������ŵľ���

		void setupScaleAlongAxis(const vector3 &axis, float k);

		// �����б����

		void setupShear(int axis, float s, float t);

		// ����ͶӰ���� ͶӰƽ���ԭ��

		void setupProject(const vector3 &n);

		// ���췴�����

		void setupReflect(int axis, float k = 0.0f);

		// ����������ƽ�淴��ľ���

		void setupReflect(const vector3 &n);

	};

	// �����* �����任������Ӿ��󣬳˷���˳��������ң���任˳���෴��

	vector3 operator*(const vector3 &p, const Matrix4x3 &m);
	Matrix4x3 operator*(const Matrix4x3 &a, const Matrix4x3 &b);

	// �����*= ���ֺ�c++��׼�﷨��һ����

	vector3 &operator*=(vector3 &p, const Matrix4x3 &m);
	Matrix4x3 &operator*=(Matrix4x3 &a, const Matrix4x3 &m);


	// ����3x3 ���ֵ�����ʽ
	float determinant(const Matrix4x3 &m);

	//����������

	Matrix4x3 inverse(const Matrix4x3 &m);

	// ��ȡ�����ƽ�Ʋ���

	vector3 getTranslation(const Matrix4x3 &m);

	// �Ӿֲ�����->������ �� ������->�ֲ����� ȡλ��/��λ

	vector3 getPositionFromParentToLocalMatrix(const Matrix4x3 &m);
	vector3 getPositionFromLocalToParentMatrix(const Matrix4x3 &m);

}


