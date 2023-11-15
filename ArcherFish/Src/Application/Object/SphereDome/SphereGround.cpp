#include "SphereGround.h"

void SphereGround::PostUpdate()
{
	// �g�k�s��
	m_scaleMat = Math::Matrix::CreateScale(30.0f, 30.0f, 30.0f);
	
	// ���W�s��
	m_transMat = Math::Matrix::CreateTranslation({ 0,0.0f,0 });

	// �s�񍇐�(�r�q�s)
	m_mWorld = m_scaleMat * m_transMat;

	SkySphereCollision();
}

void SphereGround::DrawUnLit()
{
	if (!m_spModelData) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelData, m_mWorld);
}

void SphereGround::Init()
{
	if (!m_spModelData)
	{
		m_spModelData = std::make_shared<KdModelData>();
		m_spModelData->Load("Asset/Models/SphereDome/SphereGround/SphereGround.gltf");
	}

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("SkySpColl", m_spModelData, KdCollider::TypeBump);
}

void SphereGround::SkySphereCollision()
{
	// �@�����蔻��(������)�p�̏����쐬
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0.0f, 10.0f, 0.0f);
	sphereInfo.m_sphere.Radius = 53.0f;
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

				maxOverLap = 0.0f;
				hit = false;
				hitDir = Math::Vector3::Zero;
				for (auto& ret : retBumpList)
				{
					if (maxOverLap < ret.m_overlapDistance)
					{
						maxOverLap = ret.m_overlapDistance;
						hit = true;
						hitDir = ret.m_hitDir;// �����Ԃ�����
					}
				}
				if (hit)
				{
					// �B���ʂ��g���č��W��⊮����
					hitDir.Normalize();

					// �����Ԃ�
					newPos = GetPos() + (hitDir * maxOverLap);
					SetPos(newPos);
				}
			}
		}
	}
}
