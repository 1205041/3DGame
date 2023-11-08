#include "TPS.h"

void TPS::Init()
{
	// 基準点(ターゲット)の目線の位置
	m_transMat = Math::Matrix::CreateTranslation(Math::Vector3(0.5f, 1.5f, -3.0f));

	// カーソル初期化
	m_FixMousePos.x = 1280 / 2;
	m_FixMousePos.y = 720 / 2;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	CameraBase::Init();
}

void TPS::PostUpdate()
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

	UpdateCollision();

	CameraBase::Update();
}

void TPS::UpdateRotateByMouse()
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
}

void TPS::UpdateCollision()
{
	const std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
	if (!spTarget) { return; }

	// ①レイ判定の情報作成
	KdCollider::RayInfo rayInfo;
	
	// レイの発射位置を設定
	rayInfo.m_pos = spTarget->GetPos();

	// 段差の許容範囲を設定
	rayInfo.m_pos.y += 2.0f;

	// レイの発射方向を設定
	rayInfo.m_dir = GetPos() - rayInfo.m_pos;
	rayInfo.m_dir.Normalize();/* 方向情報のみを抽出 */

	// レイの長さ
	rayInfo.m_range = (GetPos() - rayInfo.m_pos).Length();

	// 当たり判定をしたいタイプを設定
	rayInfo.m_type = KdCollider::TypeGround;

	// ②HIT判定対象オブジェクトに総当たり
	std::shared_ptr<KdGameObject> spGameObj = m_wpHitObject.lock();
	if (spGameObj)
	{
		std::list<KdCollider::CollisionResult> retRayList;
		spGameObj->Intersects(rayInfo, &retRayList);

		// ③結果を使って座標を補完する
		// レイに当たったリストから一番近いオブジェクトを検出
		maxOverLap	= 0.0f;
		hit			= false;
		hitPos		= Math::Vector3::Zero;
		for (auto& ret : retRayList)
		{
			// レイを遮断し、オーバーした長さが一番長いものを探す
			if (maxOverLap < ret.m_overlapDistance)
			{
				maxOverLap = ret.m_overlapDistance;
				hitPos = ret.m_hitPos;
				hit = true;
			}
		}
		// 当たった
		if (hit) { SetPos(hitPos); }
	}
}
