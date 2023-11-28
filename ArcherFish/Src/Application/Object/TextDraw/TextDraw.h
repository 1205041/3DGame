#pragma once
#include "../ObjBase.h"

class TextDraw :public ObjBase
{
public:
	TextDraw() { Init(); }
	~TextDraw() {}

	void SetLoadText(const std::string _tex, const Math::Rectangle _recAng);

	void SetColor(const Math::Color _color) { m_color = _color; }

	virtual void DrawSprite()	override;
	virtual void Update()		override{}
	virtual void PostUpdate()	override{}
	virtual void Init()			override{}

	void SetTextPixel(const Math::Vector2& _pixel) { m_pixel = _pixel; }
protected:
	Math::Color		m_color = { 1.0f,1.0f,1.0f,1.0f };

private:
	// テクスチャ表示
	KdTexture		m_tex;
	Math::Vector2	m_pixel		= Math::Vector2::Zero;
	Math::Rectangle	m_recAngle	= { 0,0,0,0 };
};