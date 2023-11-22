#pragma once
#include "../ObjBase.h"

class CamBase :public ObjBase
{
public:
	CamBase() { Init(); }
	~CamBase() {}

	void Init()			override;
	void PostUpdate()	override;
	void PreDraw()		override;

	void SetTarget(const std::shared_ptr<KdGameObject>& _target);

	const std::shared_ptr<KdCamera>& GetCamera() { return m_spCamera; }

	// m_rot�̕␳
	const Math::Matrix GetRotationMatrix() const
	{
		return	Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_degAng.y),
			DirectX::XMConvertToRadians(m_degAng.x),
			DirectX::XMConvertToRadians(m_degAng.z)
		);
	}

	// �v���C���[�̃J�������
	const Math::Matrix GetRotationYMatrix() const
	{
		return	Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_degAng.y));
	}

protected:
	// �J�����ϐ�
	std::weak_ptr<KdGameObject>	m_wpTarget;

	// �J������]�p�p�x
	Math::Vector3	m_degAng;
};