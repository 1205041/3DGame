#include "Enter.h"

void Enter::Update()
{
	if (m_colorC <= 1.0f) { m_colorC += 0.02f; }
	else { m_colorC = 0.1f; }
}

void Enter::PostUpdate()
{
	SetColor({ 1.0f,1.0f,1.0f,m_colorC });
}

void Enter::Init()
{
	// テクスチャ表示
	SetText("Asset/Textures/SceneUI/Title/Key/Enter.png");
	SetPixel({ 0.0f,-100.0f });
	SetRecAng({ 0,0, 300, 60 });
}
