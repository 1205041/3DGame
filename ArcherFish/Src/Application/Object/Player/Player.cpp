#include "Player.h"

#include "../Camera/TPS/TPS.h"

// �X�V�֐�
void Player::Update()
{
	m_moveVec = Math::Vector3::Zero;

	// �J�������
	std::shared_ptr<CameraBase> spCamera = m_wpCamera.lock();
	if (spCamera) { camRotMat = spCamera->GetRotationYMatrix(); }

	if (GetAsyncKeyState('W') & 0x8000)
	{
		// �O��
		Math::Vector3 v = Math::Vector3::TransformNormal
			(Math::Vector3::Backward, camRotMat);
		m_moveVec += v;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		// ����
		Math::Vector3 v = Math::Vector3::TransformNormal
			(Math::Vector3::Down, camRotMat);
		m_moveVec += v;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		// ���
		Math::Vector3 v = Math::Vector3::TransformNormal
			(Math::Vector3::Up, camRotMat);
		m_moveVec += v;
	}

	// ���K��
	m_moveVec.Normalize();

	// �ړ�
	m_pos += m_moveVec * m_moveSpd;
}

// �X�V��X�V�֐�
void Player::PostUpdate()
{
	// �g�k�s��
	m_scaleMat = Math::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// ��]�s��
	m_rotMat = Math::Matrix::CreateRotationY
	(DirectX::XMConvertToRadians(m_ang));

	// ���W�s��
	m_transMat = Math::Matrix::CreateTranslation(m_pos);

	// �s�񍇐�(�r�q�s)
	m_mWorld = m_scaleMat * m_rotMat * camRotMat * m_transMat;
	
//	RayUpdateCollision();
	SphereUpdateCollision();
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

	m_moveSpd = 0.2f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerColl", GetPos(), 0.25f, KdCollider::TypeBump);
}

void Player::Shot()
{
	Math::Vector3	dir = Math::Vector3::Zero;
	float			range = 0.0f;
	m_wpCamera.lock()->WorkCamera()->GenerateRayInfoFromClientPos
	(
		{ 1280 / 2,720 / 2 },		// 2D���W
		m_wpCamera.lock()->GetPos(),// �J�����̍��W
		dir,						// ����
		range						// ���C�̒���
	);
}
