#include "Enter.h"

void Enter::Update()
{
	if (m_AlphC <= 1.0f) { m_AlphC += 0.02f; }
	else { m_AlphC -= 0.8f; }
}

void Enter::PostUpdate()
{
	m_color = { 1.0f,1.0f,1.0f,m_AlphC };
}

void Enter::Init()
{
	SetLoadText("Asset/Textures/SceneUI/Title/Key/Enter.png", { 0.0f,-150.0f }, { 0,0, 300, 60 });
}
