#include "TPSCam.h"

#include "../../Enemy/Enemy.h"

void TPSCam::Init()
{
	// 基準点(ターゲット)の目線の位置
	m_transMat = Math::Matrix::CreateTranslation(Math::Vector3(0.8f, 1.5f, -3.5f));

	// カーソル初期化
	KdCSVData windowData("Asset/Data/WindowSettings.csv");
	const std::vector<std::string>& sizeData = windowData.GetLine(0);

	m_FixMousePos.x = atoi(sizeData[0].c_str()) / 2;
	m_FixMousePos.y = atoi(sizeData[1].c_str()) / 2;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	CamBase::Init();
}

void TPSCam::Update()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		ShotRayUpdateCollision();
	}
}

void TPSCam::PostUpdate()
{
	// ターゲットの行列
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		const std::shared_ptr<KdGameObject>& spTarget = m_wpTarget.lock();
		if (spTarget) { targetMat = Math::Matrix::CreateTranslation(spTarget->GetPos()); }
	}

	// 行列合成
	UpdateRotateByMouse();
	m_rotMat = GetRotationMatrix();

	m_mWorld = m_transMat * m_rotMat * targetMat;

	CamBase::PostUpdate();
}

void TPSCam::UpdateRotateByMouse()
{
	// マウス位置の差分を得る
	POINT nowPos{};
	GetCursorPos(&nowPos);

	POINT mouseMove{};
	mouseMove.x = nowPos.x - m_FixMousePos.x;
	mouseMove.y = nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// カメラを回転させる為に各軸の回転角度を設定する
	m_degAng.x += mouseMove.y * 0.15f;
	m_degAng.y += mouseMove.x * 0.15f;

	// 回転制御
	m_degAng.x = std::clamp(m_degAng.x, -FLT_MAX, FLT_MAX);
	m_degAng.y = std::clamp(m_degAng.y, -FLT_MAX, FLT_MAX);
}

/* ==================== */
/* 弾発射レイ当たり判定 */
/* ==================== */
void TPSCam::ShotRayUpdateCollision()
{
	// �@当たり判定(レイ判定)用の情報を作成
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = GetPos();	// レイの発射位置を設定

	// マウス位置の差分を得る
	POINT nowPos{};
	GetCursorPos(&nowPos);

	// カメラ情報
	GetCamera()->GenerateRayInfoFromClientPos(
		nowPos,	// 2D座標
		rayInfo.m_pos,	// 座標
		rayInfo.m_dir,	// 方向
		rayInfo.m_range // レイの長さ
	);
	
	rayInfo.m_type = KdCollider::TypeBump;	// 当たり判定をしたいタイプを設定

	/* === デバック用 === */
//	m_debugWire.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

	// �AHIT判定対象オブジェクトに総当たり
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retRayList;
				spGameObj->Intersects(rayInfo, &retRayList);

				// �B結果を使って座標を補完する
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
