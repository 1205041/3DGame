#include "CamBase.h"

void CamBase::Init()
{
	if (!m_spCamera) { m_spCamera = std::make_shared<KdCamera>(); }
	m_degAng = Math::Vector3::Zero;
}

void CamBase::Update()
{
	if (!m_spCamera) { return; }
//	m_spCamera->GenerateRayInfoFromClientPos()
}

void CamBase::PostUpdate()
{
	if (!m_spCamera) { return; }
	m_spCamera->SetCameraMatrix(m_mWorld);
}

void CamBase::PreDraw()
{
	if (!m_spCamera) { return; }
	m_spCamera->SetToShader();
}

void CamBase::SetTarget(const std::shared_ptr<KdGameObject>& _target)
{
	if (!_target) { return; }
	m_wpTarget = _target;
}
