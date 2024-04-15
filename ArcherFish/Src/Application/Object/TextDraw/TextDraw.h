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

	void SetLoadText(const std::string& _tex) { m_text.Load(_tex); }
	void SetPixel(const Math::Vector2& _pixel) { m_textPixel = _pixel; }
	void SetRectAng(const Math::Rectangle& _rect) { m_textRectAng = _rect; }
	void SetColor(const Math::Color& _color) { m_textColor = _color; }
	void SetPivot(const Math::Vector2& _pivot) { m_textPivot = _pivot; }
protected:
	KdTexture		m_tex;
	Math::Vector2	m_pixel = Math::Vector2::Zero;
	Math::Rectangle	m_rectAng = { 0,0,0,0 };
	Math::Color		m_color = kWhiteColor;
	Math::Vector2	m_pivot = { 0.5f, 0.5f };

private:
	KdTexture		m_text;
	Math::Vector2	m_textPixel = Math::Vector2::Zero;
	Math::Rectangle	m_textRectAng = { 0,0,0,0 };
	Math::Color		m_textColor = kWhiteColor;
	Math::Vector2	m_textPivot = { 0.5f, 0.5f };
	
};