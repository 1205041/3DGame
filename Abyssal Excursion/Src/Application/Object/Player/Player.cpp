#include "Player.h"

#include "../Camera/TPS/TPS.h"

// �X�V�֐�
void Player::Update()
{

	if (GetPos().y < -10) { m_isExpired = true; }
	if (m_isExpired)
	{
		KdAudioManager::Instance().StopAllSound();
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Lose);
	}

	// �J�������
	std::shared_ptr<CameraBase> spCamera = m_wpCamera.lock();
	if (spCamera) { camRotMat = spCamera->GetRotationYMatrix(); }

	// �L�����N�^�[�̈ړ����x
	m_moveVec = Math::Vector3::Zero;

//	if (GetAsyncKeyState('W') & 0x8000)
	{
		// �O��
		Math::Vector3 v = Math::Vector3::TransformNormal
		(Math::Vector3::Backward, camRotMat);

		m_moveVec += v;
	}
	
	m_moveVec.Normalize();
	
	m_nowPos += m_moveVec * m_moveSpd;
}

// �X�V��X�V�֐�
void Player::PostUpdate()
{
	// �L�����̍��W�s��
	m_mWorld = camRotMat * Math::Matrix::CreateTranslation(m_nowPos);
	
	UpdateCollision();
}

// �e�`��֐�
void Player::GenerateDepthMapFromLight()
{
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

// �|���`��֐�
void Player::DrawLit()
{
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

// �������֐�
void Player::Init()
{
	// �L���������l
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Player/Player.gltf"));
	}

	m_moveSpd = 0.05f;
	m_push = false;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerCollider", GetPos(), 0.25f, KdCollider::TypeBump);
}
