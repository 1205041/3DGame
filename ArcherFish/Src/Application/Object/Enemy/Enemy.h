#pragma once
#include "../ObjBase.h"

class Enemy :public ObjBase
{
public:
	Enemy() { Init(); }
	virtual ~Enemy() {}

	void Update()		override;
	void PostUpdate()	override;

	void DrawLit()		override;
	void Init()			override;

	void SetActFlg(const bool _flg) { m_act = _flg; }
	const bool GetActFlg() const { return m_act; }

	void SetAliveFlg(const bool _flg) { m_alive = _flg; }
	const bool GetAliveFlg() const { return m_alive; }

	void SetLightAct(const bool _flg) { m_lightAct = _flg; }
	const bool GetLightAct() const { return m_lightAct; }

	const int GetEnemyNum() const { return m_EnemyNum; }
private:
	// 衝突判定とそれに伴う座標の更新
	void SphereUpdateCollision();

	// 獲物総数
	static const int m_EnemyNum = 5;

	// 活動中フラグ & 生存中フラグ
	bool m_act = true;
	bool m_alive = true;

	// 発光確認 & 発光時間
	bool m_lightAct = false;
	float m_lightTime = 0.0f;
};