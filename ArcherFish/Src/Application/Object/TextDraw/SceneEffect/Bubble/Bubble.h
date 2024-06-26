#pragma once
#include "../../TextDraw.h"

#define BUBBLE_NUM 50

class Bubble :public TextDraw
{
public:
	Bubble() { Init(); }
	virtual ~Bubble() {}

	void Update()		override;
	void Init()			override;
	void DrawSprite()	override;

private:
	float		  m_radius	= 0.0f;
	Math::Vector2 m_move	= Math::Vector2::Zero;

	bool m_fill = true;	// 塗り潰し
	bool m_act	= true; // 存続フラグ
};