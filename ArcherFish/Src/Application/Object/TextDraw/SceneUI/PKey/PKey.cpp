#include "PKey.h"

void PKey::Update()
{
	if (m_AlphC <= 1.0f) { m_AlphC += 0.015f; }
	else { m_AlphC -= 0.8f; }
}

void PKey::PostUpdate()
{
	m_color = { 1.0f,1.0f,1.0f,m_AlphC };
}

void PKey::Init()
{
	SetLoadText("Asset/Textures/SceneUI/Key/PKey.png", { 0,0, 300, 60 });
}
