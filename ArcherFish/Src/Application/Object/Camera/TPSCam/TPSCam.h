#pragma once
#include "../CamBase.h"

class Enemy;

class TPSCam :public CamBase
{
public:
	TPSCam() { Init(); }
	~TPSCam() {}

	void Init()			override;
	void Update()		override;
	void PostUpdate()	override;

	void SetEnemy(const std::shared_ptr<Enemy>& _enemy) { m_wpEnemy = _enemy; }

private:
	void UpdateRotateByMouse();

	// �e�����蔻��
	void ShotRayUpdateCollision();

	// �G
	std::weak_ptr<Enemy> m_wpEnemy;

	// �����蔻��p�ϐ�
	float			m_maxOverLap = 0.0f;
	bool			m_hit = false;
	Math::Vector3	m_hitDir = Math::Vector3::Zero;
};