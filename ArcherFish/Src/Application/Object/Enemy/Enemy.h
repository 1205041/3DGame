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
	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void SphereUpdateCollision();

	// �l������
	static const int m_EnemyNum = 5;

	// �������t���O & �������t���O
	bool m_act = true;
	bool m_alive = true;

	// �����m�F & ��������
	bool m_lightAct = false;
	float m_lightTime = 0.0f;
};