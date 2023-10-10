#include "TitleLogo.h"

void TitleLogo::Update()
{
}

void TitleLogo::Init()
{
	SetText("Asset/Textures/SceneUI/Title/TitleLg.png");
	SetRcAngle({ 0, 0, 700, 100 });
	SetColor({ 1.0f,1.0f,1.0f,1.0f });

	m_texPos = Math::Vector3::Zero;
	m_texMat = Math::Matrix::Identity;
}
