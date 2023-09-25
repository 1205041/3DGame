#pragma once
#include "../ObjBase.h"

class twoDdraw :public ObjBase
{
public:
	twoDdraw() { Init(); }
	~twoDdraw() {}

	virtual void PostUpdate()	override {}
	virtual void DrawSprite()	override {}
	virtual void Init()			override {}

protected:
	// テクスチャ表示
	KdTexture m_tex;
	Math::Rectangle m_rcAngle;
	Math::Vector3 m_texPos	= Math::Vector3::Zero;
	Math::Matrix m_texMat	= Math::Matrix::Identity;
};