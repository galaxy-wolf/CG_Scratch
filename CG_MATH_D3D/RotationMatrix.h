#pragma once

namespace CG_MATH
{
	class vector3;
	class EulerAngles;
	class Quaternion;

	//-------------------------------------------------------------------------------
	// RoattionMatrix ��
	//ʵ����һ���򵥵�3x3 ���� ��������ת
	//�������Ϊ�����ģ� �ڱ任ʱָ������


	class RotationMatrix
	{
	public:
		
		// ��������

		//�����9��ֵ

		float m11, m12, m13;
		float m21, m22, m23;
		float m31, m32, m33;

		//��������

		//��Ϊ��λ����

		void identity();

		//����ָ���ķ�λ�������

		void setup(const EulerAngles &orientation);

		// ������Ԫ��������󣬼������Ԫ����������ָ������ı任

		void fromInertialToObjectQuaternion(const Quaternion &q);
		void fromObjectToInertialQuaternion(const Quaternion &q);

		// ִ����ת

		vector3 inertialToObject(const vector3 &v) const;
		vector3 objectToInertial(const vector3 &v) const;
	};

}

