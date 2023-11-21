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
	// Õ“Ë”»’è‚Æ‚»‚ê‚É”º‚¤À•W‚ÌXV
	void SphereUpdateCollision();

	bool m_act = true;

	// “–‚½‚è”»’è•Ï”
	float			m_maxOverLap = 0.0f;
	bool			m_hit		 = false;
	Math::Vector3	m_hitDir	 = Math::Vector3::Zero;
};