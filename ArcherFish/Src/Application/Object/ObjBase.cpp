#include "ObjBase.h"

void ObjBase::SetTarget(const std::shared_ptr<KdGameObject>& _target)
{
	if (!_target) { return; }

	m_wpTarget = _target;
}

/* ==================== */
/* 当たり判定(球判定用) */
/* ==================== */
void ObjBase::UpdateCollision()
{
	// �@当たり判定(球判定)用の情報を作成
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0.0f, 0.7f, 0.0f);
	sphereInfo.m_sphere.Radius = 1.0f;
	sphereInfo.m_type = KdCollider::TypeBump;

	/* === デバック用(球) === */
	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// �AHIT判定対象オブジェクトに総当たり
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retBumpList;
				spGameObj->Intersects(sphereInfo, &retBumpList);

				for (auto& ret : retBumpList)
				{
					Math::Vector3 newPos = GetPos() + (ret.m_hitDir * ret.m_overlapDistance);
					SetPos(newPos);
				}
				// �B結果を使って座標を補完する
			}
		}
	}
}

float ObjBase::GetAngleDeg(Math::Vector3 _src, Math::Vector3 _dest)
{
	//手順1. 横方向の距離aと縦方向の距離bを減算で求める
	float a = _dest.x - _src.x;
	float b = _dest.z - _src.z;

	//手順2. atan2 関数を活用して角度を求める
	float rad = atan2(b, a);

	//手順3. 補正処理:必要に応じて、デグリー(度)に直す
	float deg = DirectX::XMConvertToDegrees(rad);

	//手順4.0〜360に補正
	if (deg < 0) { deg += 360; }

	return deg;
}
