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
	KdTexture	m_txt;
	const Math::Vector2		m_txtPixel		= { -570.0f,-300.0f };
	const Math::Rectangle	m_txtRectAng	= { 0,0,151 ,123 };
	
	const Math::Color		m_barColor = { 0.5f,0.5f,0.5f };

	const int	m_height		= 25;
	const int	m_waterMax		= 250;
	int			m_water			= m_waterMax;
	const float m_waterWarning	= m_waterMax * 0.5f;
	const float m_waterDanger	= m_waterMax * 0.3f;
};