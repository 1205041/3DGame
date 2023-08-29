#include "CameraBase.h"

void CameraBase::Init()
{
	if (!m_spCamera) { m_spCamera = std::make_shared<KdCamera>(); }

	m_DegAng = Math::Vector3::Zero;
}

void CameraBase::Update()
{
	if (!m_spCamera) { return; }

	m_spCamera->SetCameraMatrix(m_mWorld);
}

void CameraBase::PreDraw()
{
	if (!m_spCamera) { return; }

	m_spCamera->SetToShader();
}

void CameraBase::SetTarget(const std::shared_ptr<KdGameObject>& target)
{
	if (!target) { return; }

	m_wpTarget = target;
}

const Math::Matrix CameraBase::GetRotationMatrix() const
{
	/* èáî‘ÇÕY,X,Z */
	return Math::Matrix::CreateFromYawPitchRoll
	(
		DirectX::XMConvertToRadians(m_DegAng.y),
		DirectX::XMConvertToRadians(m_DegAng.x),
		DirectX::XMConvertToRadians(m_DegAng.z)
	);
}

const Math::Matrix CameraBase::GetRotationYMatrix() const
{
	return Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_DegAng.y));
}
