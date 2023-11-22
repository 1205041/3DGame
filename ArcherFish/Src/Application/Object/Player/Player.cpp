#include "Player.h"

#include "../Camera/TPSCam/TPSCam.h"
#include "../Enemy/Enemy.h"

// �X�V�֐�
void Player::Update()
{
	m_moveVec = Math::Vector3::Zero;

	// �J�������
	std::shared_ptr<TPSCam> spCamera = m_wpCamera.lock();
	if (spCamera) { camRotMat = spCamera->GetRotationYMatrix(); }

	if (GetAsyncKeyState('W') & 0x8000)
	{
		// �O��
		Math::Vector3 v = Math::Vector3::TransformNormal
			(Math::Vector3::Backward, camRotMat);
		m_moveVec += v;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		// ����
		Math::Vector3 v = Math::Vector3::TransformNormal
			(Math::Vector3::Down, camRotMat);
		m_moveVec += v;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		// ���
		Math::Vector3 v = Math::Vector3::TransformNormal
			(Math::Vector3::Up, camRotMat);
		m_moveVec += v;
	}
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		ShotRayUpdateCollision();
	}

	// ���K��
	m_moveVec.Normalize();

	// �ړ�
	m_pos += m_moveVec * m_moveSpd;
	if (m_pos.y > 0) { m_pos.y = 0; }
}

// �X�V��X�V�֐�
void Player::PostUpdate()
{
	// �g�k�s��
	m_scaleMat = Math::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// ��]�s��
//	m_rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_mousePos.y));

	// ���W�s��
	m_transMat = Math::Matrix::CreateTranslation(m_pos);

	// �s�񍇐�(�r�q�s)
	m_mWorld = m_scaleMat * m_rotMat * camRotMat * m_transMat;
	
	SphereUpdateCollision();
}

// �e�`��֐�
void Player::GenerateDepthMapFromLight()
{
	if (!m_spModelWork) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelWork, m_mWorld);
}

// �|���`��֐�
void Player::DrawLit()
{
	if (!m_spModelWork) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelWork, m_mWorld);
}

// �������֐�
void Player::Init()
{
	// �L���������l
	if (!m_spModelWork)
	{
		m_spModelWork = std::make_shared<KdModelWork>();
		m_spModelWork->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Player/Player.gltf"));
	}

	m_moveSpd = 0.1f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerColl", GetPos(), 0.25f, KdCollider::TypeBump);
}

/* ==================== */
/* �����蔻��(������p) */
/* ==================== */
void Player::SphereUpdateCollision()
{
	// �@�����蔻��(������)�p�̏����쐬
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = GetPos();
	sphereInfo.m_sphere.Radius = 1.0f;
	sphereInfo.m_type = KdCollider::TypeBump || KdCollider::TypeGround;

	/* === �f�o�b�N�p(��) === */
	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// �AHIT����ΏۃI�u�W�F�N�g�ɑ�������
	Math::Vector3 newPos = Math::Vector3::Zero;
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retBumpList;
				spGameObj->Intersects(sphereInfo, &retBumpList);

				m_maxOverLap = 0.0f;
				m_hit = false;
				m_hitDir = Math::Vector3::Zero;
				for (auto& ret : retBumpList)
				{
					if (m_maxOverLap < ret.m_overlapDistance)
					{
						m_maxOverLap = ret.m_overlapDistance;
						m_hit = true;
						m_hitDir = ret.m_hitDir;// �����Ԃ�����
					}
				}
				if (m_hit)
				{
					// �B���ʂ��g���č��W��⊮����
					m_hitDir.Normalize();

					// �����Ԃ�
					newPos = GetPos() + (m_hitDir * m_maxOverLap);
					SetPos(newPos);
				}
			}
		}
	}
}

/* ==================== */
/* �e���˃��C�����蔻�� */
/* ==================== */
void Player::ShotRayUpdateCollision()
{	
	// �@�����蔻��(���C����)�p�̏����쐬
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = GetPos() + Math::Vector3{ 0.0f,0.3f,0.0f };		// ���C�̔��ˈʒu��ݒ�
	rayInfo.m_type = KdCollider::TypeDamageLine;	// �����蔻����������^�C�v��ݒ�
	GetCursorPos(&m_FixMousePos);

	// �J�������
	std::shared_ptr<TPSCam> spCamera = m_wpCamera.lock();
	if (spCamera)
	{
		spCamera->GetCamera()->GenerateRayInfoFromClientPos(
			m_FixMousePos,	// 2D���W
			rayInfo.m_pos,	// ���W
			rayInfo.m_dir,	// ����
			rayInfo.m_range	// ���C�̒���
		);
	}

	// dir		���@���烌�e�B�N���Ɍ������Ă̕���
	//			(�����@�̈ʒu�����΂����ꍇ)
	// range	�œK�ȃ��C�̒���(���ʂ����Ȃ�)
	//			���擾����Ă���

	/* === �f�o�b�N�p === */
	m_debugWire.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

	// �AHIT����ΏۃI�u�W�F�N�g�ɑ�������
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retRayList;
				spGameObj->Intersects(rayInfo, &retRayList);

				// �B���ʂ��g���č��W��⊮����
				// ���C�ɓ����������X�g�����ԋ߂��I�u�W�F�N�g�����o
				m_maxOverLap = 0.0f;
				m_hit = false;
				for (auto& ret : retRayList)
				{
					// ���C���Ւf���I�[�o�[������������Ԓ������̂�T��
					if (m_maxOverLap < ret.m_overlapDistance)
					{
						m_maxOverLap = ret.m_overlapDistance;
						m_hit = true;
					}
				}
				// ��������ɓ������Ă���
				if (m_hit) 
				{ 
					std::shared_ptr<Enemy> spEnemy = m_wpEnemy.lock();
					if (spEnemy) { spEnemy->SetActFlg(false); }
				}
			}
		}
	}
}
