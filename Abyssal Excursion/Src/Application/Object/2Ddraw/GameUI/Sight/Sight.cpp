#include "Sight.h"

void Sight::Init()
{
	SetText("Asset/Textures/GameUI/SightTP.png");
	SetRcAngle({ 0, 0, 32, 32 });
	SetColor({ 1.0f,1.0f,1.0f,0.5f });

	m_texPos = Math::Vector3::Zero;
	m_texMat = Math::Matrix::Identity;
}
