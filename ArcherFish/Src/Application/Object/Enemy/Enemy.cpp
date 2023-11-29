#include "Enemy.h"

void Enemy::Update()
{
	// �L�����N�^�[�̈ړ����x
	m_nowPos = GetPos();

	m_moveVec = Math::Vector3::Zero;
	if (m_act)
	{
		// �O��
		{
			Math::Vector3 vec = Math::Vector3::TransformNormal
				(Math::Vector3::Backward, m_transMat);
			m_moveVec += vec;
		}
	}
	else 
	{
		{
			// ����
			Math::Vector3 vec = Math::Vector3::TransformNormal
			(Math::Vector3::Down, m_transMat);
			m_moveVec += vec;
		}

		if (GetAsyncKeyState('E') & 0x8000)
		{
			m_act = true;
			m_survive = true;
		}

//		m_survive = false;
	}

	// �ړ�
	m_nowPos += m_moveVec * m_moveSpd;
	if (m_nowPos.z >= 50.0f) { m_nowPos.z = -50.0f; }
	if (m_nowPos.y <= -50.0f) { m_nowPos.y = 50.0f; }
}

void Enemy::PostUpdate()
{
	SphereUpdateCollision();

	m_scaleMat = Math::Matrix::CreateScale(3.0f);
	m_transMat = Math::Matrix::CreateTranslation(m_nowPos);

	// �L�����̍��W�s��
	m_mWorld = m_scaleMat * m_transMat;
}

void Enemy::GenerateDepthMapFromLight()
{
	if (m_survive)
	{
		if (!m_spModelWork) { return; }
		KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelWork, m_mWorld);
	}
}

void Enemy::DrawLit()
{
	if (m_survive)
	{
		if (!m_spModelWork) { return; }
		KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelWork, m_mWorld);
	}
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

				float		  maxOverLap = 0.0f;
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
					newPos = GetPos() + (dir * maxOverLap);
					SetPos(newPos);
				}
			}
		}
	}
}
