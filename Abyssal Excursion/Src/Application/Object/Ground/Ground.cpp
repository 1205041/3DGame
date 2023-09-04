#include "Ground.h"

void Ground::PostUpdate()
{
	// �g�k�s��
	scaleMat = Math::Matrix::CreateScale(0.3f, 0.3f, 0.3f);

	// ���W�s��
	transMat = Math::Matrix::CreateTranslation({ 0,-5.0f,0 });

	// �s�񍇐�(�r�q�s)
	m_mWorld = scaleMat * transMat;
}

// �A�e�̂���I�u�W�F�N�g(�s�����ȕ��̂�2D�L����)
void Ground::DrawLit()
{
	if (!m_spModel) { return; }
	// �|��(�n��)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void Ground::Init()
{
	if (!m_spModel)
	{
		//	�|��(�n��)������
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/Stage.gltf"));
	}
	
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("StageModel", m_spModel, KdCollider::TypeGround);
}
