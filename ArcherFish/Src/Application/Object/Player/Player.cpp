#include "Player.h"

#include "../Camera/TPSCam/TPSCam.h"
#include "../Enemy/Enemy.h"

// �X�V�֐�
void Player::Update()
{
	// �J�������
	std::shared_ptr<TPSCam> spCamera = m_wpCamera.lock();
	if (spCamera) { camRotMat = spCamera->GetRotPlayerMatrix(); }

	// �ړ�����
	m_moveVec = Math::Vector3::Zero;
	// �O��
	if (GetAsyncKeyState('W') & 0x8000) { GetVecNowMove(Math::Vector3::Backward, camRotMat); }
	// ����
	if (GetAsyncKeyState('D') & 0x8000) { GetVecNowMove(Math::Vector3::Down, camRotMat); }
	// ���
	if (GetAsyncKeyState('A') & 0x8000) { GetVecNowMove(Math::Vector3::Up, camRotMat); }

	// �ˌ�����
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { ShotRayUpdateCollision(); }

	// �ړ�
	m_nowPos += m_moveVec * m_moveSpd;
	if (m_nowPos.y >= 1.0f) { m_nowPos.y = 1.0f; }
}

// �X�V��X�V�֐�
void Player::PostUpdate()
{
	// �g�k�s��
	m_scaleMat = Math::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// ���W�s��
	m_transMat = Math::Matrix::CreateTranslation(m_nowPos);

	// �s�񍇐�(�r�q�s)
	m_mWorld = m_scaleMat * camRotMat * m_transMat;
	
	SphereUpdateCollision();
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

	// �����ʒu�Ƒ��x
	m_nowPos = { 0.0f,-1.0f,0.0f };
	m_moveSpd = 0.18f;

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
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retBumpList;
				spGameObj->Intersects(sphereInfo, &retBumpList);

				float			maxOverLap = 0.0f;
				bool			hit = false;
				Math::Vector3	dir = Math::Vector3::Zero;
				for (auto& ret : retBumpList)
				{
					if (maxOverLap < ret.m_overlapDistance)
					{
						maxOverLap = ret.m_overlapDistance;
						hit = true;
						dir = ret.m_hitDir;// �����Ԃ�����
					}
				}
				if (hit)
				{
					// �B���ʂ��g���č��W��⊮����
					dir.Normalize();

					// �����Ԃ�
					m_nowPos = GetPos() + (dir * maxOverLap);
					SetPos(m_nowPos);
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

	// �}�E�X�ʒu�̍����𓾂�
	POINT nowPos{};
	GetCursorPos(&nowPos);

	// �J�������
	std::shared_ptr<TPSCam> spCamera = m_wpCamera.lock();
	if (spCamera) 
	{ 
		spCamera->GetCamera()->GenerateRayInfoFromClientPos(
			nowPos,			// 2D���W
			rayInfo.m_pos,	// ���W
			rayInfo.m_dir,	// ����
			rayInfo.m_range // ���C�̒���
		);
	}
	rayInfo.m_type = KdCollider::TypeBump;	// �����蔻����������^�C�v��ݒ�

	/* === �f�o�b�N�p === */
//	m_debugWire.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

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
				float maxOverLap = 0.0f;
				bool  hit = false;
				for (auto& ret : retRayList)
				{
					// ���C���Ւf���I�[�o�[������������Ԓ������̂�T��
					if (maxOverLap < ret.m_overlapDistance) { hit = true; }
				}
				if (hit) // ���C���G�l�~�[�ɓ������Ă���
				{
					std::shared_ptr<Enemy> spEnemy = m_wpEnemy.lock();
					if (spEnemy) { spEnemy->SetActFlg(false); }
				}
			}
		}
	}
}
