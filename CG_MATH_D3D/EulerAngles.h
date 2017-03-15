#pragma once
namespace CG_MATH
{
	class Quaternion;
	class Matrix4x3;
	class RotationMatrix;

	//--------------------------------------------------------
	// class EulerAngles
	//
	// �������ڱ�ʾheading-pitch-bank ŷ����ϵͳ
	//heading +y, pitch +x, bank +z

	class EulerAngles
	{
	public:
	// ��������

		// ֱ�ӵı�ʾ��ʽ
		// �û��ȱ��������Ƕ�

		float heading;
		float pitch;
		float bank;
		
	// ��������

		EulerAngles() {}
		EulerAngles(float h, float p, float b):
		heading(h), pitch(p), bank(b){}
		void identity() { pitch = bank = heading = 0.0f; }

		// �任Ϊ "���Ƽ�" ŷ����

		void  canonize();

		// ����Ԫ��ת����ŷ���ǡ�
		// �������Ԫ������Ϊ ����-���� �� ����-���� ��Ԫ����
		void fromObjectToInertialQuternion(const Quaternion &q);
		void fromInertialToObjectQuaternion(const Quaternion &q);

		// �Ӿ���ת����ŷ����
		//����������Ϊ ����-���� ������-���� ת������
		//ƽ�Ʋ��ֱ����ԣ����Ҽ��������������
		void fromObjectToWorldMatrix(const Matrix4x3 &m);
		void fromWorldToObjectMatrix(const Matrix4x3 &m);

		// ����ת����ŷ����

		void fromRotationMatrix(const RotationMatrix &m);
	};

	extern const EulerAngles kEulerAnglesIdentity;
}
