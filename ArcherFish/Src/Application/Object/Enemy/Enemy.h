#pragma once
#include "../ObjBase.h"

class Enemy :public ObjBase
{
public:
	Enemy() { Init(); }
	~Enemy() {}

	void Update()					override;
	void PostUpdate()				override;

	void GenerateDepthMapFromLight()override;
	void DrawLit()					override;

	void Init()						override;

	void SetActFlg(const bool _flg) { m_act = _flg; }
private:
	// 衝突判定とそれに伴う座標の更新
	void SphereUpdateCollision();

	bool m_act = true;

	// 当たり判定変数
	float			m_maxOverLap = 0.0f;
	bool			m_hit		 = false;
	Math::Vector3	m_hitDir	 = Math::Vector3::Zero;
};