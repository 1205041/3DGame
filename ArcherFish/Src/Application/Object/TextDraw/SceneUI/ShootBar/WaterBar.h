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

	const void SetWater(const int _water) { m_water = _water; }

	const int GetWaterMax()const { return m_waterMax; }
	const int GetWater()const { return m_water; }
private:
	/* ÉoÅ[ÇÃïœêî */
	KdTexture	m_fishText;
	KdTexture	m_text;
	const Math::Vector2		m_fishPixel		= { -570.0f,-300.0f };
	const Math::Vector2		m_pixel			= { -500.0f,-310.0f };
	const Math::Rectangle	m_fishRectAng	= { 0,0,151 ,123 };
	const Math::Rectangle	m_rectAng		= { 0,0,250 ,15 };
	const Math::Color		m_barColor		= { 0.5f,0.5f,0.5f };
	Math::Color				m_color			= kBlueColor;
	const Math::Vector2		m_pivot			= { 0.0f, 0.5f };

	const int	m_height		= 25;
	const int	m_waterMax		= 250;
	int			m_water			= m_waterMax;
	const float m_waterWarning	= m_waterMax * 0.5f;
	const float m_waterDanger	= m_waterMax * 0.3f;
};