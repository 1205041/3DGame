#include "ObjBase.h"

void ObjBase::SetTarget(const std::shared_ptr<KdGameObject>& _target)
{
	if (!_target) { return; }

	m_wpTarget = _target;
}

float ObjBase::GetAngleDeg(Math::Vector3 _src, Math::Vector3 _dest)
{
	//č1. ĄűüĚŁaĆcűüĚŁbđ¸ZĹßé
	float a = _dest.x - _src.x;
	float b = _dest.z - _src.z;

	//č2. atan2 ÖđpľÄpxđßé
	float rad = atan2(b, a);

	//č3. âł:KvÉśÄAfO[(x)Éźˇ
	float deg = DirectX::XMConvertToDegrees(rad);

	//č4.0`360Éâł
	if (deg < 0) { deg += 360; }

	return deg;
}
