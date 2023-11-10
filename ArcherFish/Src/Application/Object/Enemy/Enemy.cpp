#include "Enemy.h"

void Enemy::Update()
{
	// �L�����N�^�[�̈ړ����x
	m_nowPos = GetPos();
}

void Enemy::PostUpdate()
{
	m_transMat = Math::Matrix::CreateTranslation(m_nowPos);

	// �L�����̍��W�s��
	m_mWorld = m_transMat;

//	RayUpdateCollision();
	UpdateCollision();
}

void Enemy::GenerateDepthMapFromLight()
{
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void Enemy::DrawLit()
{
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void Enemy::Init()
{
	// �L���������l
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Enemy/SkyEnemy.gltf"));
	}

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("EnemyColl", GetPos(), 0.25f, KdCollider::TypeBump);
}
