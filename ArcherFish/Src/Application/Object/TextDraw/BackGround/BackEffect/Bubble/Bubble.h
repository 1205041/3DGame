#pragma once
#include "../../../TextDraw.h"

class Bubble:public TextDraw
{
public:
	Bubble() {}
	~Bubble() {}

	void DrawSprite()	override;
	void Update()		override;
	void PostUpdate()	override;
	void Init()			override;
private:
	// �A�̃p�����[�^(�~)
	Math::Vector2	m_pos;
	float			m_radius	= 0.0f;
};