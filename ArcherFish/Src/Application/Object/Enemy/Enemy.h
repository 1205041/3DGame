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
	void SetSurviveFlg(const bool _flg) { m_survive = _flg; }
private:
	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void SphereUpdateCollision();

	// �������t���O & �������t���O
	bool m_act = true;
	bool m_survive = true;

	// �����蔻��ϐ�
	float			m_maxOverLap = 0.0f;
	bool			m_hit		 = false;
	Math::Vector3	m_hitDir	 = Math::Vector3::Zero;
};