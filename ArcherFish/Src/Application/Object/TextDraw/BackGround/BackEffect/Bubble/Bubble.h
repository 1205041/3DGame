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
	// –A‚Ìƒpƒ‰ƒ[ƒ^(‰~)
	Math::Vector2	m_pos = Math::Vector2::Zero;
	float			m_radius	= 1.0f;

	const int babbleNum = 100;// –A‚Ì‘”
};