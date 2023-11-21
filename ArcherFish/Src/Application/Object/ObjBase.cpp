#include "ObjBase.h"

void ObjBase::SetTarget(const std::shared_ptr<KdGameObject>& _target)
{
	if (!_target) { return; }

	m_wpTarget = _target;
}

float ObjBase::GetAngleDeg(Math::Vector3 _src, Math::Vector3 _dest)
{
	//�菇1. �������̋���a�Əc�����̋���b�����Z�ŋ��߂�
	float a = _dest.x - _src.x;
	float b = _dest.z - _src.z;

	//�菇2. atan2 �֐������p���Ċp�x�����߂�
	float rad = atan2(b, a);

	//�菇3. �␳����:�K�v�ɉ����āA�f�O���[(�x)�ɒ���
	float deg = DirectX::XMConvertToDegrees(rad);

	//�菇4.0�`360�ɕ␳
	if (deg < 0) { deg += 360; }

	return deg;
}
