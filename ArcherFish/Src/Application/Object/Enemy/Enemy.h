#pragma once
#include "../ObjBase.h"

class Enemy :public ObjBase
{
public:
	Enemy() { Init(); }
	virtual ~Enemy() {}

	void Update()					override;
	void PostUpdate()				override;

	void DrawLit()					override;
	void Init()						override;

	void SetActFlg(const bool _flg) { m_act = _flg; }
	void SetSurviveFlg(const bool _flg) { m_survive = _flg; }
	const bool GetActFlg() const { return m_act; }
	const bool GetSurviveFlg() const { return m_survive; }
private:
	// 衝突判定とそれに伴う座標の更新
	void SphereUpdateCollision();

	// 活動中フラグ & 生存中フラグ
	bool m_act = true;
	bool m_survive = true;

	// 発光確認 & 発光時間
	bool m_lightAct = false;
	float m_lightTime = 0.0f;
};