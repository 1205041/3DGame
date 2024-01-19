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
	int m_waterMax = 500;
	int m_water = m_waterMax;
	float m_waterWarning = m_waterMax * 0.5f;
	float m_waterDanger = m_waterMax * 0.3f;

	Math::Vector2 pixel = { 0.0f,-330.0f };
	int height = 15;

	Math::Color barColor = { 0.5f,0.5f,0.5f };
	Math::Color blue = { 0.0f,0.0f,1.0f };
	Math::Color yellow = { 1.0f,1.0f,0.0f };
	Math::Color red = { 1.0f,0.0f,0.0f };
	Math::Color color = blue;
};