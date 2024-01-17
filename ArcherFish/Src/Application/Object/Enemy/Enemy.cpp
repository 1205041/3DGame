#include "Enemy.h"

void Enemy::Update()
{
	if (m_survive)
	{
		// 現在位置と移動速度
		m_nowPos = GetPos();
		m_moveVec = Math::Vector3::Zero;

		if (m_act)
		{
			// 前方
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

		// 座標更新
		m_nowPos += m_moveVec * m_moveSpd;
		if (m_nowPos.z >= 50.0f) { m_nowPos.z = -50.0f; }
		if (m_nowPos.y <= -50.0f) { m_nowPos.y = 50.0f; }

		// 発光時間
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

		// キャラの座標行列
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
	// キャラ初期値
	if (!m_spModelWork)
	{
		m_spModelWork = std::make_shared<KdModelWork>();
		m_spModelWork->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Enemy/SkyEnemy.gltf"));
	}

	// 速度
	m_moveSpd = 0.1f;

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
	sphereInfo.m_sphere.Radius = 0.5f;
	sphereInfo.m_type = KdCollider::TypeBump;

	/* === デバック用(球) === */
//	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

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

				float  maxOverLap = 0.0f;
				bool		  hit = false;
				Math::Vector3 dir = Math::Vector3::Zero;
				for (auto& ret : retBumpList)
				{
					if (maxOverLap < ret.m_overlapDistance)
					{
						maxOverLap = ret.m_overlapDistance;
						hit = true;
						dir = ret.m_hitDir;// 押し返す方向
					}
				}
				if (hit)
				{
					// ③結果を使って座標を補完する
					dir.Normalize();

					// 押し返し
					m_nowPos = GetPos() + (dir * maxOverLap);
					SetPos(m_nowPos);

					// 敵捕食判定
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
