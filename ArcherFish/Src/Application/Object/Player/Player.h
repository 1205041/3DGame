#pragma once
#include "../ObjBase.h"

class TPSCam;
class Enemy;
class WaterBar;

class Player :public ObjBase
{
public:
	Player() { Init(); }
	virtual ~Player() {}

	void Update()		override;
	void PostUpdate()	override;

	void DrawLit()		override;
	void Init()			override;

	void SetCamera(const std::shared_ptr<TPSCam>& _camera) { m_wpCamera = _camera; }
	void SetEnemy(const std::shared_ptr<Enemy>& _enemy) { m_wpEnemy = _enemy; }
	void SetWaterBar(const std::shared_ptr<WaterBar>& _waterBar) { m_wpWaterBar = _waterBar; }
private:
	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void SphereUpdateCollision();

	// �e�����蔻��
	void ShotRayUpdateCollision();

	// �E�B�[�N�|�C���^
	std::weak_ptr<TPSCam>	m_wpCamera;
	std::weak_ptr<Enemy>	m_wpEnemy;
	std::weak_ptr<WaterBar> m_wpWaterBar;
	
	// �J������]�s��
	Math::Matrix camRotMat = Math::Matrix::Identity;
};