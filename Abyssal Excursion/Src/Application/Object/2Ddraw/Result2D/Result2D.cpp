#include "Result2D.h"

void Result2D::Init()
{
	SetText("Asset/Textures/Result/win.png");
	SetRcAngle({ 0,0,1280,720 });

	m_texPos = Math::Vector3::Zero;
	m_texMat = Math::Matrix::Identity;
}
