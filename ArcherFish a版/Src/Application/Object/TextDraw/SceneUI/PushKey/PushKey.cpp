#include "PushKey.h"

void PushKey::Update()
{
	if (m_AlphC <= 1.0f) { m_AlphC += 0.015f; }
	else { m_AlphC -= 0.8f; }
}

void PushKey::PostUpdate()
{
	SetColor({ 1.0f, 1.0f, 1.0f, m_AlphC });
}
