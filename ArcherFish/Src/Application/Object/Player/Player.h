#pragma once
#include "../ObjBase.h"

class TPSCam;

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

	float GetNowYPos() const { return m_pos.y; }
private:
	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void SphereUpdateCollision();

	// �J����
	Math::Matrix camRotMat = Math::Matrix::Identity;
	std::weak_ptr<TPSCam> m_wpCamera;

	// �����ʒu
	Math::Vector3	m_pos = { 0.0f,-1.0f,0.0f };

	// �����蔻��p�ϐ�
	float			m_maxOverLap = 0.0f;
	bool			m_hit = false;
	Math::Vector3	m_hitDir = Math::Vector3::Zero;
};