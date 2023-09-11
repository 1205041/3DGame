#pragma once
#include "../ObjBase.h"

class twoDdraw :public ObjBase
{
public:
	twoDdraw() { Init(); }
	~twoDdraw() {}

	// �X�V�Ɋւ���֐�
	void PostUpdate()	override;
	void DrawSprite()	override;
	void Init()			override;

	virtual void SetTexture(std::string _fileName) { m_tex.Load(_fileName); }
	virtual void SetRectAngle(Math::Rectangle _rcAngle) { m_rcAngle = _rcAngle; }
private:
	// �e�N�X�`���\��
	KdTexture m_tex;
	Math::Rectangle m_rcAngle;
	Math::Vector3 m_texPos	= Math::Vector3::Zero;
	Math::Matrix m_texMat	= Math::Matrix::Identity;
};