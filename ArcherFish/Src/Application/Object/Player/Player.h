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

private:
	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void SphereUpdateCollision();

	// �J����
	std::weak_ptr<TPSCam> m_wpCamera;
	Math::Matrix camRotMat = Math::Matrix::Identity;
};