#pragma once
#include "../../../TextDraw.h"

class Enter :public TextDraw
{
public:
	Enter() { Init(); }
	~Enter() {}

	void Update()		override;
	void PostUpdate()	override;
//	void DrawSprite()	override;
	void Init()			override;
private:

	// 色チェンジ
	float m_colorC = 0.1f;

	// 四角の変数
//	Math::Vector2	m_pos	= Math::Vector2::Zero;
//	Math::Vector2	m_half	= Math::Vector2::Zero;
//	Math::Color		m_col	= { 1.0f,1.0f,1.0f,1.0f };

//	POINT m_point;
};