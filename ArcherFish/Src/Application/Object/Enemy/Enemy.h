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
	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void SphereUpdateCollision();

	// �������t���O & �������t���O
	bool m_act = true;
	bool m_survive = true;

	// �����m�F & ��������
	bool m_lightAct = false;
	float m_lightTime = 0.0f;
};