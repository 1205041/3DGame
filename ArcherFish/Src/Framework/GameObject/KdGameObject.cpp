#include "KdGameObject.h"

void KdGameObject::SetScale(float _scale)
{
	Math::Vector3 scale(_scale);

	SetScale(scale);
}

void KdGameObject::SetScale(const Math::Vector3& _scale)
{
	Math::Vector3 vecX = m_mWorld.Right();
	Math::Vector3 vecY = m_mWorld.Up();
	Math::Vector3 vecZ = m_mWorld.Backward(); 
	
	vecX.Normalize();
	vecY.Normalize();
	vecZ.Normalize();

	m_mWorld.Right(vecX * _scale.x);
	m_mWorld.Up(vecY * _scale.y);
	m_mWorld.Backward(vecZ * _scale.z);
}

Math::Vector3 KdGameObject::GetScale() const
{
	return Math::Vector3(m_mWorld.Right().Length(), m_mWorld.Up().Length(), m_mWorld.Backward().Length());
}

void KdGameObject::CalcDistSqrFromCamera(const Math::Vector3& _camPos)
{
	m_distSqrFromCamera = (m_mWorld.Translation() - _camPos).LengthSquared();
}

bool KdGameObject::Intersects(const KdCollider::SphereInfo& _targetShape, std::list<KdCollider::CollisionResult>* _pResults)
{
	if (!m_pCollider) { return false; }

	return m_pCollider->Intersects(_targetShape, m_mWorld, _pResults);
}

bool KdGameObject::Intersects(const KdCollider::RayInfo& _targetShape, std::list<KdCollider::CollisionResult>* _pResults)
{
	if (!m_pCollider) { return false; }

	return m_pCollider->Intersects(_targetShape, m_mWorld, _pResults);
}
