#pragma once
#include "../ObjBase.h"

class TPSC;

class Player :public ObjBase
{
public:
	Player() {}
	~Player() {}

	void Update()	override;
	void DrawLit()	override;
	void Init()		override;

	void SetCamera(const std::shared_ptr<TPSC>& _camera) { m_wpCamera = _camera; }

private:
	std::weak_ptr<TPSC>			m_wpCamera;

	// ‰Á‘¬“x
	float m_zRollSpd = 0.0f;
	float m_xRollSpd = 0.0f;

	const float m_zero = 0.0f;

	const float m_zMax = 5.0f;
	const float m_zMin = -5.0f;

	const float m_xMax = 5.0f;
	const float m_xMin = -5.0f;

	const float m_speed = 0.02f;
};