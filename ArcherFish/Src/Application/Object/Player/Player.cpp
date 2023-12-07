#include "Player.h"

#include "../Camera/TPSCam/TPSCam.h"
#include "../Enemy/Enemy.h"

// 更新関数
void Player::Update()
{
	// カメラ情報
	std::shared_ptr<TPSCam> spCamera = m_wpCamera.lock();
	if (spCamera) { camRotMat = spCamera->GetRotPlayerMatrix(); }

	// 移動処理
	m_moveVec = Math::Vector3::Zero;
	// 前方
	if (GetAsyncKeyState('W') & 0x8000) { GetVecNowMove(Math::Vector3::Backward, camRotMat); }
	// 下方
	if (GetAsyncKeyState('D') & 0x8000) { GetVecNowMove(Math::Vector3::Down, camRotMat); }
	// 上方
	if (GetAsyncKeyState('A') & 0x8000) { GetVecNowMove(Math::Vector3::Up, camRotMat); }

	// 射撃処理
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { ShotRayUpdateCollision(); }

	// 移動
	m_nowPos += m_moveVec * m_moveSpd;
	if (m_nowPos.y >= 1.0f) { m_nowPos.y = 1.0f; }
}

// 更新後更新関数
void Player::PostUpdate()
{
	// 拡縮行列
	m_scaleMat = Math::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// 座標行列
	m_transMat = Math::Matrix::CreateTranslation(m_nowPos);

	// 行列合成(ＳＲＴ)
	m_mWorld = m_scaleMat * camRotMat * m_transMat;
	
	SphereUpdateCollision();
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

	// 初期位置と速度
	m_nowPos = { 0.0f,-1.0f,0.0f };
	m_moveSpd = 0.18f;

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
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retBumpList;
				spGameObj->Intersects(sphereInfo, &retBumpList);

				float			maxOverLap = 0.0f;
				bool			hit = false;
				Math::Vector3	dir = Math::Vector3::Zero;
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

	// マウス位置の差分を得る
	POINT nowPos{};
	GetCursorPos(&nowPos);

	// カメラ情報
	std::shared_ptr<TPSCam> spCamera = m_wpCamera.lock();
	if (spCamera) 
	{ 
		spCamera->GetCamera()->GenerateRayInfoFromClientPos(
			nowPos,			// 2D座標
			rayInfo.m_pos,	// 座標
			rayInfo.m_dir,	// 方向
			rayInfo.m_range // レイの長さ
		);
	}
	rayInfo.m_type = KdCollider::TypeBump;	// 当たり判定をしたいタイプを設定

	/* === デバック用 === */
//	m_debugWire.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

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
				float maxOverLap = 0.0f;
				bool  hit = false;
				for (auto& ret : retRayList)
				{
					// レイを遮断しオーバーした長さが一番長いものを探す
					if (maxOverLap < ret.m_overlapDistance) { hit = true; }
				}
				if (hit) // レイがエネミーに当たっている
				{
					std::shared_ptr<Enemy> spEnemy = m_wpEnemy.lock();
					if (spEnemy) { spEnemy->SetActFlg(false); }
				}
			}
		}
	}
}
