#pragma once
#include "../ObjBase.h"

class CameraBase;
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

	void SetCamera(const std::shared_ptr<CameraBase>& _camera) { m_wpCamera = _camera; }
	void SetEnemy(const std::shared_ptr<Enemy>& _enemy) { m_wpEnemy = _enemy; }

private:
	// �e�����蔻��
	void ShotRayUpdateCollision();

	// �J����
	Math::Matrix camRotMat = Math::Matrix::Identity;
	std::weak_ptr<CameraBase> m_wpCamera;

	// �G
	std::weak_ptr<Enemy> m_wpEnemy;

	// �����ʒu
	Math::Vector3	m_pos = { 0.0f,-6.0f,0.0f };

	// �����蔻��p�ϐ�
	Math::Vector3	hitPos = Math::Vector3::Zero;

	// �}�E�X���W

};