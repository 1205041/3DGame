#pragma once
#include "../ObjBase.h"

class TextDraw :public ObjBase
{
public:
	TextDraw() { Init(); }
	~TextDraw() {}

	virtual void	Update()		override {}
	virtual void	PostUpdate()	override {}
	void			DrawSprite()	override;
	virtual void	Init()			override {}

protected:
	void SetText(const std::string _tex) { m_tex.Load(_tex); }
	void SetPixel(const Math::Vector2 _pixel) { m_pixel = _pixel; }
	void SetRecAng(const Math::Rectangle _recAng) { m_recAngle = _recAng; }
	void SetColor(const Math::Color _color) { m_color = _color; }

private:
	// テクスチャ表示
	KdTexture		m_tex;
	Math::Vector2	m_pixel		= Math::Vector2::Zero;
	Math::Rectangle	m_recAngle	= { 0,0,0,0 };
	Math::Color		m_color		= { 1.0f,1.0f,1.0f,1.0f };
};