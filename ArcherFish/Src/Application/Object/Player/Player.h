#pragma once
#include "../ObjBase.h"

class CameraBase;

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

private:
	// �e����
	void ShotRayUpdateCollision();

	// �J����
	Math::Matrix camRotMat = Math::Matrix::Identity;
	std::weak_ptr<CameraBase> m_wpCamera;

	// �����ʒu
	Math::Vector3	m_pos = { 0.0f,-6.0f,0.0f };

};