#include "Player.h"

#include "../Camera/TPSCam/TPSCam.h"

// 更新関数
void Player::Update()
{
	m_moveVec = Math::Vector3::Zero;

	// カメラ情報
	std::shared_ptr<TPSCam> spCamera = m_wpCamera.lock();
	if (spCamera) { camRotMat = spCamera->GetRotationYMatrix(); }

	if (GetAsyncKeyState('W') & 0x8000)
	{
		// 前方
		Math::Vector3 v = Math::Vector3::TransformNormal
			(Math::Vector3::Backward, camRotMat);
		m_moveVec += v;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		// 下方
		Math::Vector3 v = Math::Vector3::TransformNormal
			(Math::Vector3::Down, camRotMat);
		m_moveVec += v;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		// 上方
		Math::Vector3 v = Math::Vector3::TransformNormal
			(Math::Vector3::Up, camRotMat);
		m_moveVec += v;
	}

	// 正規化
	m_moveVec.Normalize();

	// 移動
	m_pos += m_moveVec * m_moveSpd;
	if (m_pos.y >= 1.0f) { m_pos.y = 1.0f; }
}

// 更新後更新関数
void Player::PostUpdate()
{
	// 拡縮行列
	m_scaleMat = Math::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// 回転行列
//	m_rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_mousePos.y));

	// 座標行列
	m_transMat = Math::Matrix::CreateTranslation(m_pos);

	// 行列合成(ＳＲＴ)
	m_mWorld = m_scaleMat * m_rotMat * camRotMat * m_transMat;
	
	SphereUpdateCollision();
}

// 影描画関数
void Player::GenerateDepthMapFromLight()
{
	if (!m_spModelWork) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelWork, m_mWorld);
}

// 板ポリ描画関数
void Player::DrawLit()
{
	if (!m_spModelWork) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModelWork, m_mWorld);
}

// 初期化関数
void Player::Init()
{
	// キャラ初期値
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
/* 当たり判定(球判定用) */
/* ==================== */
void Player::SphereUpdateCollision()
{
	// ①当たり判定(球判定)用の情報を作成
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = GetPos();
	sphereInfo.m_sphere.Radius = 1.0f;
	sphereInfo.m_type = KdCollider::TypeBump || KdCollider::TypeGround;

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
