#include "Enemy.h"

void Enemy::Update()
{
	if (m_survive)
	{
		// ���݈ʒu�ƈړ����x
		m_nowPos = GetPos();
		m_moveVec = Math::Vector3::Zero;

		if (m_act)
		{
			// �O��
			GetVecNowMove(Math::Vector3::Backward, m_transMat);
			m_moveSpd = 0.1f;
		}
		else
		{
			GetVecNowMove(Math::Vector3::Down, m_transMat);
			m_moveSpd = 0.05f;

			if (m_lightTime > 5.0f) { m_lightAct = true; }
			else { m_lightAct = false; }
		}

		if (KdInputManager::Instance().GetButtonState("EnemyFlg"))
		{
			m_act = true;
			m_survive = true;
		}

		// ���W�X�V
		m_nowPos += m_moveVec * m_moveSpd;
		if (m_nowPos.z >= 50.0f) { m_nowPos.z = -50.0f; }
		if (m_nowPos.y <= -50.0f) { m_nowPos.y = 50.0f; }

		// ��������
		m_lightTime += 0.5f;
		if (m_lightTime > 10.0f) { m_lightTime = 0.0f; }
	}
}

void Enemy::PostUpdate()
{
	if (m_survive)
	{
		SphereUpdateCollision();

		if (!m_act)
		{
			m_scaleMat = Math::Matrix::CreateScale(1.0f);
			m_rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90.0f));
		}
		else
		{
			m_scaleMat = Math::Matrix::CreateScale(2.5f);
			m_rotMat = Math::Matrix::Identity;
		}

		m_transMat = Math::Matrix::CreateTranslation(m_nowPos);

		// �L�����̍��W�s��
		m_mWorld = m_scaleMat * m_rotMat * m_transMat;
	}
}

void Enemy::DrawLit()
{
	if (!m_act)
	{
		KdShaderManager::Instance().m_HD2DShader.SetColorEnable(m_lightAct);
	}

	if (m_survive)
	{
		if (!m_spModelWork) { return; }
		KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelWork, m_mWorld);
	}
	
	KdShaderManager::Instance().m_HD2DShader.SetColorEnable(false);
}

void Enemy::Init()
{
	// �L���������l
	if (!m_spModelWork)
	{
		m_spModelWork = std::make_shared<KdModelWork>();
		m_spModelWork->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Enemy/SkyEnemy.gltf"));
	}

	// ���x
	m_moveSpd = 0.1f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("EnemyColl", GetPos(), 0.25f, KdCollider::TypeBump);
}

/* ==================== */
/* �����蔻��(������p) */
/* ==================== */
void Enemy::SphereUpdateCollision()
{
	// �@�����蔻��(������)�p�̏����쐬
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = GetPos();
	sphereInfo.m_sphere.Radius = 0.5f;
	sphereInfo.m_type = KdCollider::TypeBump;

	/* === �f�o�b�N�p(��) === */
//	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

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

				float  maxOverLap = 0.0f;
				bool		  hit = false;
				Math::Vector3 dir = Math::Vector3::Zero;
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

					// �G�ߐH����
					if (!m_act) 
					{ 
						m_survive = false;
						KdAudioManager::Instance().Play("Asset/Sounds/SE/Predation/Predation.wav", false);
					}
				}
			}
		}
	}
}
