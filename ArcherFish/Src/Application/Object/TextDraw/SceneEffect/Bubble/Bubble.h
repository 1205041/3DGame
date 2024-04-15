#pragma once
#include "../../TextDraw.h"

// –A‘”
#define BUBBLE_NUM 50

class Bubble :public TextDraw
{
public:
	Bubble() { Init(); }
	virtual ~Bubble() {}

	void Update()		override;
	void Init()			override;
	void DrawSprite()	override;

	const int GetBubbleNum()const { return BUBBLE_NUM; }
private:
	float		  m_radius	= 0.0f;
	Math::Vector2 m_move	= Math::Vector2::Zero;

	bool m_fill = true;	// “h‚è’×‚µ
	bool m_act	= true; // ‘¶‘±ƒtƒ‰ƒO
};