#pragma once
#include "../ObjBase.h"

class TextDraw :public ObjBase
{
public:
	TextDraw() { Init(); }
	virtual ~TextDraw() {}

	virtual void DrawSprite()	override;
	virtual void Update()		override{}
	virtual void PostUpdate()	override{}
	virtual void Init()			override{}

	void SetLoadText(const std::string& _tex) { m_tex.Load(_tex); }
	void SetPixel(const Math::Vector2& _pixel) { m_pixel = _pixel; }
	void SetRectAng(const Math::Rectangle& _rect) { m_rectAng = _rect; }
	void SetColor(const Math::Color& _color) { m_color = _color; }
	void SetPivot(const Math::Vector2& _pivot) { m_pivot = _pivot; }

private:
	// テクスチャ表示
	KdTexture		m_tex;
	Math::Vector2	m_pixel		= Math::Vector2::Zero;
	Math::Rectangle	m_rectAng	= { 0,0,0,0 };
	Math::Color		m_color		= kWhiteColor;
	Math::Vector2	m_pivot		= { 0.5, 0.5f };
};