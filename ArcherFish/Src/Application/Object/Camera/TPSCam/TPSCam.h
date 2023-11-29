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

	// ’e“–‚½‚è”»’è
	void ShotRayUpdateCollision();

	// “G
	std::weak_ptr<Enemy> m_wpEnemy;
};