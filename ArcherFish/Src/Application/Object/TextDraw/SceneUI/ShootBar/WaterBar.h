#pragma once
#include "../../TextDraw.h"

class WaterBar :public TextDraw
{
public:
	WaterBar() { Init(); }
	virtual ~WaterBar() {}

	void Update()		override;
	void Init()			override;
	void DrawSprite()	override;

private:
	KdTexture		m_text;
	Math::Vector2	m_pixel	   = { 225.0f,-330.0f };
	int				m_waterMax = 250;
	int				m_water	   = m_waterMax;
	int				m_height   = 25;
	Math::Rectangle m_rectAng  = { 0,0,250 ,15 };
	Math::Color		m_barColor = { 0.5f,0.5f,0.5f };
	Math::Color		m_color	   = kBlueColor;
	Math::Vector2	m_pivot    = { 0.0f, 0.5f };

	float m_waterWarning = m_waterMax * 0.5f;
	float m_waterDanger = m_waterMax * 0.3f;
};