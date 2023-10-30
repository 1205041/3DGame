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
	void Shot();

	// �J����
	std::weak_ptr<CameraBase> m_wpCamera;
	Math::Matrix camRotMat = Math::Matrix::Identity;
};