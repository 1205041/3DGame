#include "Title2D.h"

void Title2D::Init()
{
	SetText("Asset/Textures/Title/Title.png");
	SetRcAngle({ 0,0,1280,720 });

	m_texPos = Math::Vector3::Zero;
	m_texMat = Math::Matrix::Identity;
}
