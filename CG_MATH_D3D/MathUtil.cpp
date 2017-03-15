#include "MathUtil.h"

namespace CG_MATH
{
	//---------------------------------------------------------
	// ͨ�������ʵ���2Pi���������Ƕ�������-pi ��pi �������ڡ�
	float wrapPi(float theta)
	{
		theta += kPi;
		theta -= floorf(theta * k1Over2Pi) *k2Pi;
		theta -= kPi;
		return theta;
	}

	//----------------------------------------------------------
	// safeAcos
	// ��acos��x����ͬ�� �����x������Χ��������ӽ�����Чֵ
	// ����ֵ��0 �� pi֮�䣬��c�����б�׼acos������ͬ��
	float safeAcos(float x)
	{
		// ���߽�����

		if (x <= -1.0f)
		{
			return kPi;
		}
		if (x >= 1.0f)
		{
			return 0.0f;
		}

		return acosf(x);
	}
}