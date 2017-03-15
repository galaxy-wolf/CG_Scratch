#include "vector3.h"
#include "RotationMatrix.h"
#include "MathUtil.h"
#include "Quaternion.h"
#include "EulerAngles.h"

namespace CG_MATH
{


	//////////////////////////////////////////////////////////////////////////////////////
	//
	// RotationMatrix ��
	//
	//-----------------------------------------------------------------------------------
	//
	//������ʹ����Ӧ�ú�����Ҫ���ľ������֯��ʽ
	//��Ȼ��������������˵Ӧ��ʹһ�����鶼�Ե÷ǳ�ֱ��
	//��������Ϊ��ת�����������������
	//�þ�������ǹ��Ե�����ı任�����Ҫִ�����嵽���Եı任��Ӧ�ó�������ת��
	//Ҳ����˵��
	//
	//���Ե�����ı任
	//
	//				|m11  m12  m13|   
	// |ix iy iz| * |m21  m22  m23| = |ox oy oz|
	//				|m31  m32  m33|   
	//
	//���嵽���Եı任
	//
	//				|m11  m21  m31|   
	// |ix iy iz| * |m12  m22  m32| = |ox oy oz|
	//	            |m13  m23  m33|   
	//
	/////////////////////////////////////////////////////////////////////////////////////////

	//---------------------------------------------------------------------------------------
	//RotationMatrix::identity
	//��Ϊ��λ����

	void RotationMatrix::identity() {
		m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
		m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
		m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
	}

	//---------------------------------------------------------------------------------------
	//RoatationMatrix::setup
	//��ŷ���ǲ����������
	//

	void RotationMatrix::setup(const EulerAngles &orientation) {

		// ����Ƕȵ�sin ��cosֵ

		float sh,ch,  sp,cp,  sb,cb;
		sinCos(&sh, &ch, orientation.heading);
		sinCos(&sp, &cp, orientation.pitch);
		sinCos(&sb, &cb, orientation.bank);

		// ������

		m11 = ch*cb + sh*sp*sb;
		m12 = -ch*sb + sh*sp*cb;
		m13 = sh*cp;

		m21 = cp*sb;
		m22 = cp*cb;
		m23 = -sp;

		m31 = -sh*cb + ch*sp*sb;
		m32 = sh*sb + ch*sp*cb;
		m33 = ch*cp;
	}

	//--------------------------------------------------------------------------------
	// RotationMatrix::fromInertialToObjectQuaternion
	// ���ݹ���-������ת��Ԫ���������
	//
	
	void RotationMatrix::fromInertialToObjectQuaternion(const Quaternion &q) {

		// �����󣬻����Ż��Ŀ��ܣ���Ϊ����ӱ��ʽ����ͬ�ģ� ���ǰ��Ż�����������������

		m11 = 1.0f - 2.0f*(q.y*q.y + q.z*q.z);
		m12 = 2.0f*(q.x*q.y + q.w*q.z);
		m13 = 2.0f*(q.x*q.z - q.w*q.y);

		m21 = 2.0f*(q.x*q.y - q.w*q.z);
		m22 = 1.0f - 2.0f*(q.x*q.x + q.z*q.z);
		m23 = 2.0f*(q.y*q.z + q.w*q.x);

		m31 = 2.0f*(q.x*q.z + q.w*q.y);
		m32 = 2.0f*(q.y*q.z - q.w*q.x);
		m33 = 1.0f - 2.0f*(q.x*q.x + q.y*q.y);

	}

	//------------------------------------------------------------------------------------------------
	// RotationMatrix::fromObjectToInertialQuaternion
	// ��������-������ת��Ԫ���������
	//

	void RotationMatrix::fromObjectToInertialQuaternion(const Quaternion &q) {

		// �����󣬻����Ż��Ŀ��ܣ���Ϊ����ӱ��ʽ����ͬ�ģ� ���ǰ��Ż�����������������

		m11 = 1.0f - 2.0f*(q.y*q.y + q.z*q.z);
		m12 = 2.0f*(q.x*q.y - q.w*q.z);
		m13 = 2.0f*(q.x*q.z + q.w*q.y);

		m21 = 2.0f*(q.x*q.y + q.w*q.z);
		m22 = 1.0f - 2.0f*(q.x*q.x + q.z*q.z);
		m23 = 2.0f*(q.y*q.z - q.w*q.x);

		m31 = 2.0f*(q.x*q.z - q.w*q.y);
		m32 = 2.0f*(q.y*q.z + q.w*q.x);
		m33 = 1.0f - 2.0f*(q.x*q.x + q.y*q.y);
	}

	//------------------------------------------------------------------------------
	//RotationMatrix::inertialToObject
	// ������������-����任

	vector3 RotationMatrix::inertialToObject(const vector3 &v) const {

		// �ԡ���׼��ʽִ�о���*������

		return vector3(
			v.x*m11 + v.y*m21 + v.z*m31,
			v.x*m12 + v.y*m22 + v.z*m32,
			v.x*m13 + v.y*m23 + v.z*m33
		);

	}

	//----------------------------------------------------------------------------------
	// RotationMatrix::objectToInertial
	//������������-���Ա任

	vector3 RotationMatrix::objectToInertial(const vector3 &v) const {

		// ��ת��

		return vector3(
			v.x*m11 + v.y*m12 + v.z*m13,
			v.x*m21 + v.y*m22 + v.z*m23,
			v.x*m31 + v.y*m32 + v.z*m33
		);

	}
}