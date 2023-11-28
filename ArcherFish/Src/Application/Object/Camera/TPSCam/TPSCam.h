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

	// 弾当たり判定
	void ShotRayUpdateCollision();

	// 敵
	std::weak_ptr<Enemy> m_wpEnemy;

	// 当たり判定用変数
	float			m_maxOverLap = 0.0f;
	bool			m_hit = false;
	Math::Vector3	m_hitDir = Math::Vector3::Zero;

};