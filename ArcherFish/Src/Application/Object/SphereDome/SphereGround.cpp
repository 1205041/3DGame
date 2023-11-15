#include "SphereGround.h"

void SphereGround::PostUpdate()
{
	// 拡縮行列
	m_scaleMat = Math::Matrix::CreateScale(30.0f, 30.0f, 30.0f);
	
	// 座標行列
	m_transMat = Math::Matrix::CreateTranslation({ 0,0.0f,0 });

	// 行列合成(ＳＲＴ)
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
	// ①当たり判定(球判定)用の情報を作成
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0.0f, 10.0f, 0.0f);
	sphereInfo.m_sphere.Radius = 53.0f;
	sphereInfo.m_type = KdCollider::TypeBump;

	/* === デバック用(球) === */
	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// ②HIT判定対象オブジェクトに総当たり
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
						hitDir = ret.m_hitDir;// 押し返す方向
					}
				}
				if (hit)
				{
					// ③結果を使って座標を補完する
					hitDir.Normalize();

					// 押し返し
					newPos = GetPos() + (hitDir * maxOverLap);
					SetPos(newPos);
				}
			}
		}
	}
}
