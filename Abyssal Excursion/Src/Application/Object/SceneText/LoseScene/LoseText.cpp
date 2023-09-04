#include "LoseText.h"

void LoseText::PostUpdate()
{
	// �g�k�s��
	scaleMat = Math::Matrix::CreateScale(10.0f, 8.0f, 10.0f);

	// ���W�s��
	transMat = Math::Matrix::CreateTranslation({ 0,0,1.0f });

	// �s�񍇐�(�r�q�s)
	m_mWorld = scaleMat * transMat;
}

void LoseText::DrawUnLit()
{
	if (!m_spPoly) { return; }

	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void LoseText::Init()
{
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/Result/lose.png"));
	}
}