#include "Enemy.h"

void Enemy::Update()
{
	if (m_act)
	{
		// キャラクターの移動速度
		m_nowPos = GetPos();
	}
}

void Enemy::PostUpdate()
{
	if (m_act)
	{
		m_transMat = Math::Matrix::CreateTranslation(m_nowPos);

		// キャラの座標行列
		m_mWorld = m_transMat;

		SphereUpdateCollision();
	}
}

void Enemy::GenerateDepthMapFromLight()
{
	if (m_act)
	{
		if (!m_spModelWork) { return; }
		KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelWork, m_mWorld);
	}
}

void Enemy::DrawLit()
{
	if (m_act)
	{
		if (!m_spModelWork) { return; }
		KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelWork, m_mWorld);
	}
}

void Enemy::Init()
{
	// キャラ初期値
	if (!m_spModelWork)
	{
		m_spModelWork = std::make_shared<KdModelWork>();
		m_spModelWork->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Enemy/SkyEnemy.gltf"));
	}

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("EnemyColl", GetPos(), 0.25f, KdCollider::TypeBump);
}

/* ==================== */
/* 当たり判定(球判定用) */
/* ==================== */
void Enemy::SphereUpdateCollision()
{

	// ①当たり判定(球判定)用の情報を作成
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = GetPos();
	sphereInfo.m_sphere.Radius = 1.0f;
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

				m_maxOverLap = 0.0f;
				m_hit = false;
				m_hitDir = Math::Vector3::Zero;
				for (auto& ret : retBumpList)
				{
					if (m_maxOverLap < ret.m_overlapDistance)
					{
						m_maxOverLap = ret.m_overlapDistance;
						m_hit = true;
						m_hitDir = ret.m_hitDir;// 押し返す方向
					}
				}
				if (m_hit)
				{
					// ③結果を使って座標を補完する
					m_hitDir.Normalize();

					// 押し返し
					newPos = GetPos() + (m_hitDir * m_maxOverLap);
					SetPos(newPos);
				}
			}
		}
	}
}
