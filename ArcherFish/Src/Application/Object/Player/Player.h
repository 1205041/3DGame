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
	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void SphereUpdateCollision();

	// �e�����蔻��
	void ShotRayUpdateCollision();

	// �J����
	Math::Matrix camRotMat = Math::Matrix::Identity;
	std::weak_ptr<TPSCam> m_wpCamera;

	// �G
	std::weak_ptr<Enemy> m_wpEnemy;

	// �����ʒu
	Math::Vector3	m_pos = { 0.0f,-1.0f,0.0f };

	// �����蔻��p�ϐ�
	float			m_maxOverLap = 0.0f;
	bool			m_hit = false;
	Math::Vector3	m_hitDir = Math::Vector3::Zero;

};