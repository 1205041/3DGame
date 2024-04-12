#include "ObjBase.h"

void ObjBase::SetTarget(const std::shared_ptr<KdGameObject>& _target)
{
	if (!_target) { return; }

	m_wpTarget = _target;
}

const Math::Vector3 ObjBase::GetVecNowMove(Math::Vector3 _vec, Math::Matrix _mat)
{
	Math::Vector3 v = Math::Vector3::TransformNormal(_vec, _mat);

	m_moveVec += v;
	m_moveVec.Normalize();	// ê≥ãKâª

	return m_moveVec;
}
