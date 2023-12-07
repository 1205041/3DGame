#pragma once
#include "../ObjBase.h"

class CamBase :public ObjBase
{
public:
	CamBase() { Init(); }
	virtual ~CamBase() {}

	void Init()			override;
	void Update()		override {}
	void PostUpdate()	override;
	void PreDraw()		override;

	void SetTarget(const std::shared_ptr<KdGameObject>& _target);

	const std::shared_ptr<KdCamera>& GetCamera() { return m_spCamera; }

	// m_rot�̕␳
	const Math::Matrix GetRotCameraMatrix() const
	{
		return	Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_degAng.y),
			DirectX::XMConvertToRadians(m_degAng.x),
			DirectX::XMConvertToRadians(m_degAng.z)
		);
	}

	// �v���C���[�̃J�������
	const Math::Matrix GetRotPlayerMatrix() const
	{
		return Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_degAng.y),
			DirectX::XMConvertToRadians(m_degAng.x),
			DirectX::XMConvertToRadians(0.0f)
		);
	}
protected:
	// �J�����ϐ�
	std::weak_ptr<KdGameObject>	m_wpTarget;

	// �J������]�p�p�x
	Math::Vector3	m_degAng;
};
