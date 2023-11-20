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
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
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
	
	CharaSphereUpdateCollision();
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

	m_moveSpd = 0.2f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerColl", GetPos(), 0.25f, KdCollider::TypeBump);
}

/* ==================== */
/* 弾発射レイ当たり判定 */
/* ==================== */
void Player::ShotRayUpdateCollision()
{
//	std::shared_ptr<Enemy> spEnemy = m_wpEnemy.lock();
//	if (spEnemy) { spEnemy->SetActFlg(false); }

//	std::shared_ptr<CamBase> spCamera = m_wpCamera.lock();
//	if (spCamera) {}

	// ①当たり判定(レイ判定)用の情報を作成
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = GetPos() + Math::Vector3{ 0.0f,0.4f,0.0f };		// レイの発射位置を設定
	rayInfo.m_dir = Math::Vector3::Backward;		// レイの発射方向を設定
	rayInfo.m_type = KdCollider::TypeDamageLine;	// 当たり判定をしたいタイプを設定

	// 射程(射程の許容範囲)
	const float	bulletRange = 10.0f;
	rayInfo.m_range = bulletRange;// レイの長さを設定

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
				maxOverLap = 0.0f;
				hit = false;
				hitPos = Math::Vector3::Zero;
				for (auto& ret : retRayList)
				{
					// レイを遮断しオーバーした長さが一番長いものを探す
					if (maxOverLap < ret.m_overlapDistance)
					{
						maxOverLap = ret.m_overlapDistance;
						hitPos = ret.m_hitPos;
						hit = true;
					}
				}
				// 何かしらに当たっている
				if (hit) { SetPos(hitPos); }
			}
		}
	}
}
