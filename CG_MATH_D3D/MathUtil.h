#pragma once
#include <math.h>
namespace CG_MATH
{
	const float kPi = 3.14159265f;
	const float k2Pi = kPi * 2.0f;
	const float kPiOver2 = kPi / 2.0f;
	const float k1OverPi = 1.0f / kPi;
	const float k1Over2Pi = 1.0f / k2Pi;
	// ͨ�������ʵ���2Pi �������Ƕ�������-pi ��pi �������ڡ�
	extern float wrapPi(float theta);
	// ����ȫ�� �����Ǻ���
	extern float safeAcos(float x);
	//����sin �� cos ֵ
	//��ĳЩƽ̨�ϣ������Ҫ������ֵ�� ͬʱ����Ҫ�ȷֿ�����졣
	inline void sinCos(float *returnSin, float *returnCos, float theta)
	{
		//Ϊ�˼򵥣� ����ֻʹ�ñ�׼���Ǻ���
		//ע����ĳЩƽ̨�Ͽ������ø���һЩ��
		*returnSin = sinf(theta);
		*returnCos = cosf(theta);
	}

}

