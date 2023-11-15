#include "SphereGround.h"

void SphereGround::PostUpdate()
{
	// gksñ
	m_scaleMat = Math::Matrix::CreateScale(30.0f, 30.0f, 30.0f);
	
	// ÀWsñ
	m_transMat = Math::Matrix::CreateTranslation({ 0,0.0f,0 });

	// sñ¬(rqs)
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
	// @½è»è(»è)pÌîñðì¬
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0.0f, 10.0f, 0.0f);
	sphereInfo.m_sphere.Radius = 53.0f;
	sphereInfo.m_type = KdCollider::TypeBump;

	/* === fobNp() === */
	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// AHIT»èÎÛIuWFNgÉ½è
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
						hitDir = ret.m_hitDir;// µÔ·ûü
					}
				}
				if (hit)
				{
					// BÊðgÁÄÀWðâ®·é
					hitDir.Normalize();

					// µÔµ
					newPos = GetPos() + (hitDir * maxOverLap);
					SetPos(newPos);
				}
			}
		}
	}
}
