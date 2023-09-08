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

private:
	// �e�N�X�`���\��
	KdTexture m_tex;
	Math::Vector3 m_texPos	= Math::Vector3::Zero;
	Math::Matrix m_texMat	= Math::Matrix::Identity;
};