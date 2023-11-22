#include "Player.h"

#include "../Camera/TPSCam/TPSCam.h"
#include "../Enemy/Enemy.h"

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
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		ShotRayUpdateCollision();
	}

	// 正規化
	m_moveVec.Normalize();

	// 移動
	m_pos += m_moveVec * m_moveSpd;
	if (m_pos.y > 0) { m_pos.y = 0; }
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

/* ==================== */
/* 弾発射レイ当たり判定 */
/* ==================== */
void Player::ShotRayUpdateCollision()
{	
	// ①当たり判定(レイ判定)用の情報を作成
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = GetPos() + Math::Vector3{ 0.0f,0.3f,0.0f };		// レイの発射位置を設定
	rayInfo.m_type = KdCollider::TypeDamageLine;	// 当たり判定をしたいタイプを設定
	GetCursorPos(&m_FixMousePos);

	// カメラ情報
	std::shared_ptr<TPSCam> spCamera = m_wpCamera.lock();
	if (spCamera)
	{
		spCamera->GetCamera()->GenerateRayInfoFromClientPos(
			m_FixMousePos,	// 2D座標
			rayInfo.m_pos,	// 座標
			rayInfo.m_dir,	// 方向
			rayInfo.m_range	// レイの長さ
		);
	}

	// dir		自機からレティクルに向かっての方向
	//			(※自機の位置から飛ばした場合)
	// range	最適なレイの長さ(無駄が少ない)
	//			が取得されている

	/* === デバック用 === */
	m_debugWire.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

	// ②HIT判定対象オブジェクトに総当たり
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retRayList;
				spGameObj->Intersects(rayInfo, &retRayList);

				// ③結果を使って座標を補完する
				// レイに当たったリストから一番近いオブジェクトを検出
				m_maxOverLap = 0.0f;
				m_hit = false;
				for (auto& ret : retRayList)
				{
					// レイを遮断しオーバーした長さが一番長いものを探す
					if (m_maxOverLap < ret.m_overlapDistance)
					{
						m_maxOverLap = ret.m_overlapDistance;
						m_hit = true;
					}
				}
				// 何かしらに当たっている
				if (m_hit) 
				{ 
					std::shared_ptr<Enemy> spEnemy = m_wpEnemy.lock();
					if (spEnemy) { spEnemy->SetActFlg(false); }
				}
			}
		}
	}
}
