#pragma once
#include "../ObjBase.h"

class TPSCam;
class Enemy;

class Player :public ObjBase
{
public:
	Player() { Init(); }
	~Player() {}

	void Update()					override;
	void PostUpdate()				override;

	void GenerateDepthMapFromLight()override;
	void DrawLit()					override;

	void Init()						override;

	void SetCamera(const std::shared_ptr<TPSCam>& _camera) { m_wpCamera = _camera; }
	void SetEnemy(const std::shared_ptr<Enemy>& _enemy) { m_wpEnemy = _enemy; }
private:
	// 衝突判定とそれに伴う座標の更新
	void SphereUpdateCollision();

	// 弾当たり判定
	void ShotRayUpdateCollision();

	// ウィークポインタ
	std::weak_ptr<TPSCam> m_wpCamera;
	std::weak_ptr<Enemy> m_wpEnemy;
	
	// カメラ回転行列
	Math::Matrix camRotMat = Math::Matrix::Identity;
};