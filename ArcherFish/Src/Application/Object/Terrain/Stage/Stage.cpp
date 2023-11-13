#include "Stage.h"

void Stage::PostUpdate()
{
	// �g�k�s��
	m_scaleMat = Math::Matrix::CreateScale(10.0f, 1.0f, 10.0f);
	/* �g�k���� */
	// �X�J�C�X�t�B�A�F�n�� = 149.0f�F10.0f�Œ��x�ǂ��T�C�Y
	// �Ȃ̂Œn�ʂ̃T�C�Y��15�{���X�J�C�X�t�B�A�̃T�C�Y
	
	// ���W�s��
	m_transMat = Math::Matrix::CreateTranslation({ 0,-5.0f,0 });

	// �s�񍇐�(�r�q�s)
	m_mWorld = m_scaleMat * m_transMat;
}

void Stage::DrawLit()
{
	if (!m_spModelData) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelData, m_mWorld);
}

void Stage::Init()
{
	if (!m_spModelData)
	{
		//	���f��(�n��)������
		m_spModelData = std::make_shared<KdModelData>();
		m_spModelData->Load("Asset/Models/Terrain/Stage/Stage.gltf");
	}

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("StageColl", m_spModelData, KdCollider::TypeBump);
}
