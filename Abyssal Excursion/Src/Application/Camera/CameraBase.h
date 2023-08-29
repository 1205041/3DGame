#pragma once

class CameraBase : public KdGameObject
{
public:
	CameraBase() {}
	virtual ~CameraBase()	override {}

	void Init()				override;
	void Update()			override;
	void PreDraw()			override;

	void SetTarget(const std::shared_ptr<KdGameObject>& target);

	const std::shared_ptr<const KdCamera> WorkCamera() const { return m_spCamera; }

	const Math::Matrix GetRotationMatrix() const;
	const Math::Matrix GetRotationYMatrix() const;

protected:
	std::shared_ptr<KdCamera>	m_spCamera = nullptr;
	std::weak_ptr<KdGameObject>	m_wpTarget;

	Math::Matrix		m_LocalPos;
	Math::Matrix		m_Rotation;

	// �J������]�s��
	Math::Vector3		m_DegAng;
};