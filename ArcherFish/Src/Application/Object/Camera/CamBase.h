#pragma once
#include "../ObjBase.h"

class CamBase :public ObjBase
{
public:
	CamBase() { Init(); }
	~CamBase() {}

	void Init()			override;
	void Update()		override {}
	void PostUpdate()	override;
	void PreDraw()		override;

	void SetTarget(const std::shared_ptr<KdGameObject>& _target);

	const std::shared_ptr<KdCamera>& GetCamera() { return m_spCamera; }

	// m_rotの補正
	const Math::Matrix GetRotationMatrix() const
	{
		return	Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_degAng.y),
			DirectX::XMConvertToRadians(m_degAng.x),
			DirectX::XMConvertToRadians(m_degAng.z)
		);
	}

	// プレイヤーのカメラ情報
	const Math::Matrix GetRotationYMatrix() const
	{
		return	Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_degAng.y));
	}

protected:
	// カメラ変数
	std::weak_ptr<KdGameObject>	m_wpTarget;

	// カメラ回転用角度
	Math::Vector3	m_degAng;
};
