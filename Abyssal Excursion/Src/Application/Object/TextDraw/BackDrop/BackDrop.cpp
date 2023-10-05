#include "BackDrop.h"

void BackDrop::Init()
{
	SetText("Asset/Textures/BackDrop/BDimg.png");
	SetRcAngle({ 0,0,1280,720 });

	m_texPos = Math::Vector3::Zero;
	m_texMat = Math::Matrix::Identity;
}
