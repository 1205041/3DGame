#pragma once
#include "../ObjBase.h"

class TextBase :public ObjBase
{
public:
	TextBase() { Init(); }
	~TextBase() {}

	void PostUpdate()	override;
	void DrawSprite()	override;
	virtual void Init()	override {}

protected:
	void SetText(const std::string _fileName) { m_tex.Load(_fileName); }
	void SetPixel(const Math::Vector2 _pixel) { m_pixel = _pixel; }
	void SetRcAngle(const Math::Rectangle _rcAngle) { m_rcAngle = _rcAngle; }
	void SetColor(const DirectX::SimpleMath::Color _color) { m_color = _color; }

	// テクスチャ表示
	KdTexture					m_tex;
	Math::Vector2				m_pixel = Math::Vector2::Zero;
	Math::Rectangle				m_rcAngle;
	DirectX::SimpleMath::Color	m_color = { 1.0f,1.0f,1.0f,1.0f };

	// テクスチャ座標
	Math::Vector3 m_texPos	= Math::Vector3::Zero;
	Math::Matrix m_texMat	= Math::Matrix::Identity;
};