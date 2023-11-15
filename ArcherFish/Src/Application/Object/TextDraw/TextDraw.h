#pragma once
#include "../ObjBase.h"

class TextDraw :public ObjBase
{
public:
	TextDraw() { Init(); }
	~TextDraw() {}

	void Update()		override;
	void PostUpdate()	override;
	void DrawSprite()	override;
	void Init()			override;

	void SetLoadText(const std::string _tex, const Math::Vector2 _pixel, const Math::Rectangle _recAng);

	void SetColor(const Math::Color _color) { m_color = _color; }
private:
	// テクスチャ表示
	KdTexture		m_tex;
	Math::Vector2	m_pixel		= Math::Vector2::Zero;
	Math::Rectangle	m_recAngle	= { 0,0,0,0 };
	Math::Color		m_color		= { 1.0f,1.0f,1.0f,1.0f };

	// 色チェンジ
	float m_AlphC = 0.1f;
};