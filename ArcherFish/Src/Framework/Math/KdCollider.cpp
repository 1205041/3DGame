#include "KdCollider.h"

/* =！=！=！= */
// KdCollider
/* =！=！=！= */

/* = = = = = = = =  = = = = = */
/* 当たり判定形状の登録関数群 */
/* = = = = = = = =  = = = = = */
// 当たり判定形状登録関数
void KdCollider::RegisterCollisionShape(std::string_view _name, std::unique_ptr<KdCollisionShape> _spShape)
{
	if (!_spShape) { return; }
	m_collisionShapes.emplace(_name.data(), std::move(_spShape));
}

// 球情報の当たり判定
void KdCollider::RegisterCollisionShape(std::string_view _name, const DirectX::BoundingSphere& _sphere, UINT _type)
{
	RegisterCollisionShape(_name, std::make_unique<KdSphereCollision>(_sphere, _type));
}

// ローカル座標と半径の当たり判定
void KdCollider::RegisterCollisionShape(std::string_view _name, const Math::Vector3& _localPos, float _radius, UINT _type)
{
	RegisterCollisionShape(_name, std::make_unique<KdSphereCollision>(_localPos, _radius, _type));
}

// スマポのモデルデータの当たり判定
void KdCollider::RegisterCollisionShape(std::string_view _name, const std::shared_ptr<KdModelData>& _model, UINT _type)
{
	RegisterCollisionShape(_name, std::make_unique<KdModelCollision>(_model, _type));
}

// 生ポのモデルデータの当たり判定
void KdCollider::RegisterCollisionShape(std::string_view _name, KdModelData* _model, UINT _type)
{
	RegisterCollisionShape(_name, std::make_unique<KdModelCollision>(std::shared_ptr<KdModelData>(_model), _type));
}

// スマポのモデルワークの当たり判定
void KdCollider::RegisterCollisionShape(std::string_view _name, const std::shared_ptr<KdModelWork>& _model, UINT _type)
{
	RegisterCollisionShape(_name, std::make_unique<KdModelCollision>(_model, _type));
}

// 生ポのモデルワークの当たり判定
void KdCollider::RegisterCollisionShape(std::string_view _name, KdModelWork* _model, UINT _type)
{
	RegisterCollisionShape(_name, std::make_unique<KdModelCollision>(std::shared_ptr<KdModelWork>(_model), _type));
}

// スマポのポリゴンの当たり判定
void KdCollider::RegisterCollisionShape(std::string_view _name, const std::shared_ptr<KdPolygon> _polygon, UINT _type)
{
	RegisterCollisionShape(_name, std::make_unique<KdPolygonCollision>(_polygon, _type));
}

// 生ポのポリゴンの当たり判定
void KdCollider::RegisterCollisionShape(std::string_view _name, KdPolygon* _polygon, UINT _type)
{
	RegisterCollisionShape(_name, std::make_unique<KdPolygonCollision>(std::shared_ptr<KdPolygon>(_polygon), _type));
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* 球vsコライダーに登録された任意の形状の当たり判定 */
// ・球に合わせて何のために当たり判定をするのか type を渡す必要がある
// ・第3引数に詳細結果の受け取る機能が付いている
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
bool KdCollider::Intersects(const SphereInfo& _targetShape, const Math::Matrix& _ownerMatrix, std::list<KdCollider::CollisionResult>* _pResults) const
{
	// 当たり判定無効のタイプの場合は返る
	if (_targetShape.m_type & m_disableType) { return false; }

	bool isHit = false;

	for (auto& collisionShape : m_collisionShapes)
	{
		// 用途が一致していない当たり判定形状はスキップ
		if (!(_targetShape.m_type & collisionShape.second->GetType())) { continue; }

		KdCollider::CollisionResult tmpRes;
		KdCollider::CollisionResult* pTmpRes = _pResults ? &tmpRes : nullptr;

		if (collisionShape.second->Intersects(_targetShape.m_sphere, _ownerMatrix, pTmpRes))
		{
			isHit = true;

			// 詳細な衝突結果を必要としない場合は1つでも接触して返す
			if (!_pResults) { break; }
			_pResults->push_back(tmpRes);
		}
	}

	return isHit;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/
/* レイvsコライダーに登録された任意の形状の当たり判定 */
// ・レイに合わせて何のために当たり判定をするのか type を渡す必要がある
// ・第3引数に詳細結果の受け取る機能が付いている
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/
bool KdCollider::Intersects(const RayInfo& _targetShape, const Math::Matrix& _ownerMatrix, std::list<KdCollider::CollisionResult>* _pResults) const
{
	// 当たり判定無効のタイプの場合は返る
	if (_targetShape.m_type & m_disableType) { return false; }

	// レイの方向ベクトルが存在しない場合は判定不能なのでそのまま返る
	if (!_targetShape.m_dir.LengthSquared())
	{
		assert(0 && "KdCollider::Intersects：レイの方向ベクトルが存在していないため、正しく判定できません");

		return false; 
	}

	bool isHit = false;

	for (auto& collisionShape : m_collisionShapes)
	{
		// 用途が一致していない当たり判定形状はスキップ
		if (!(_targetShape.m_type & collisionShape.second->GetType())) { continue; }

		KdCollider::CollisionResult tmpRes;
		KdCollider::CollisionResult* pTmpRes = _pResults ? &tmpRes : nullptr;

		if (collisionShape.second->Intersects(_targetShape, _ownerMatrix, pTmpRes))
		{
			isHit = true;

			// 詳細な衝突結果を必要としない場合は1つでも接触して返す
			if (!_pResults) { break; }
			_pResults->push_back(tmpRes);
		}
	}

	return isHit;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = */
// 任意のCollisionShapeを検索して有効/無効を切り替える
/* = = = = = = = = = = = = = = = = = = = = = = = = = = */
void KdCollider::SetEnable(std::string_view _name, bool _flag)
{
	auto targetCol = m_collisionShapes.find(_name.data());
	if (targetCol != m_collisionShapes.end()) { targetCol->second->SetEnable(_flag); }
}

/* = = = = = = = = = = = = = = = = = = */
// 特定のタイプの有効/無効を切り替える
/* = = = = = = = = = = = = = = = = = = */
void KdCollider::SetEnable(int _type, bool _flag)
{
	if (_flag) { m_disableType &= ~_type; }	// 有効にしたい
	else { m_disableType |= _type; }		// 無効にしたい
}

void KdCollider::SetEnableAll(bool _flag)
{
	for (auto& col : m_collisionShapes) { col.second->SetEnable(_flag); }
}

/* =！=！=！=！=！=！=！=！= */
/* === SphereCollision === */
// ・球形の形状
/* =！=！=！=！=！=！=！=！= */

/* = = = = = = = = = = = = = = = = = = = = = = = = = */
/* 球vs球の当たり判定 */
// ・判定回数は 1 回　
// 　計算自体も軽く最も軽量な当たり判定　
// 　計算回数も固定なので処理効率は安定
// ・片方の球の判定を0にすれば単純な距離判定も作れる
/* = = = = = = = = = = = = = = = = = = = = = = = = = */
bool KdSphereCollision::Intersects(const DirectX::BoundingSphere& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes)
{
	if (!m_enable) { return false; }

	DirectX::BoundingSphere myShape;

	m_shape.Transform(myShape, _world);

	// 球同士の当たり判定
	bool isHit = myShape.Intersects(_target);

	// 詳細リザルトが必要無ければ即結果を返す
	if (!_pRes) { return isHit; }

	// 当たった時のみ計算
	if (isHit)
	{
		// お互いに当たらない最小距離
		float needDistance = _target.Radius + myShape.Radius;

		// 自身から相手に向かう方向ベクトル
		_pRes->m_hitDir = (Math::Vector3(_target.Center) - Math::Vector3(myShape.Center));
		float betweenDistance = _pRes->m_hitDir.Length();

		// 重なり量 = お互い当たらない最小距離 - お互いの実際距離
		_pRes->m_overlapDistance = needDistance - betweenDistance;
		_pRes->m_hitDir.Normalize();

		// 当たった座標はお互いの球の衝突の中心点
		_pRes->m_hitPos = myShape.Center + _pRes->m_hitDir * (myShape.Radius + _pRes->m_overlapDistance * 0.5f);
	}

	return isHit;
}

/* = = = = = = = = = = = = = = = = = = */
/* 球vsレイの当たり判定 */
// ・判定回数は 1 回　
// 　計算回数が固定なので処理効率は安定
/* = = = = = = = = = = = = = = = = = = */
bool KdSphereCollision::Intersects(const KdCollider::RayInfo& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes)
{
	if (!m_enable) { return false; }

	DirectX::BoundingSphere myShape;

	m_shape.Transform(myShape, _world);

	float hitDistance = 0.0f;

	bool isHit = myShape.Intersects(_target.m_pos, _target.m_dir, hitDistance);

	// 判定限界距離を加味
	isHit &= (_target.m_range >= hitDistance);

	// 詳細リザルトが必要無ければ即結果を返す
	if (!_pRes) { return isHit; }

	// 当たった時のみ計算
	if (isHit)
	{
		// レイ発射位置 + レイの当たった位置までのベクトル 
		_pRes->m_hitPos = _target.m_pos + _target.m_dir * hitDistance;
		_pRes->m_hitDir = _target.m_dir * (-1);
		_pRes->m_overlapDistance = _target.m_range - hitDistance;
	}

	return isHit;
}


/* =！=！=！=！=！=！=！= */
/* === ModelCollision === */
// ・3Dメッシュの形状
/* =！=！=！=！=！=！=！= */

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* 球vsモデルの当たり判定 */
// ・判定回数は メッシュの個数 x 各メッシュのポリゴン数 
// 　計算回数がモデルのデータ依存のため処理効率は不安定
// ・単純に計算回数が多くなる可能性があるため重くなりがち
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
bool KdModelCollision::Intersects(const DirectX::BoundingSphere& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes)
{
	// 当たり判定が無効 or 形状が解放済みなら判定せず返る
	if (!m_enable || !m_shape) { return false; }

	std::shared_ptr<KdModelData> spModelData = m_shape->GetData();

	// データが無ければ判定不能なので返る
	if (!spModelData) { return false; }

	const std::vector<KdModelData::Node>& dataNodes = spModelData->GetOriginalNodes();
	const std::vector<KdModelWork::Node>& workNodes = m_shape->GetNodes();

	// 各メッシュに押される用の球・押される毎に座標を更新する必要がある
	DirectX::BoundingSphere pushedSphere = _target;
	// 計算用にFloat3 → Vectorへ変換
	Math::Vector3 pushedSphereCenter = DirectX::XMLoadFloat3(&pushedSphere.Center);

	bool isHit = false;

	Math::Vector3 hitPos;

	// 当たり判定ノードとのみ当たり判定
	for (int index : spModelData->GetCollisionMeshNodeIndices())
	{
		const KdModelData::Node& dataNode = dataNodes[index];
		const KdModelWork::Node& workNode = workNodes[index];

		// あり得ないはずだが一応チェック
		if (!dataNode.m_spMesh) { continue; }

		CollisionMeshResult tmpResult;
		CollisionMeshResult* pTmpResult = _pRes ? &tmpResult : nullptr;

		// メッシュと球形の当たり判定実行
		if (!MeshIntersect(*dataNode.m_spMesh, pushedSphere, workNode.m_worldTransform * _world, pTmpResult)) { continue; }

		// 詳細リザルトが必要無ければ即結果を返す
		if (!_pRes) { return true; }

		isHit = true;

		// 重なった分押し戻す
		pushedSphereCenter = DirectX::XMVectorAdd(pushedSphereCenter, DirectX::XMVectorScale(tmpResult.m_hitDir, tmpResult.m_overlapDistance));

		DirectX::XMStoreFloat3(&pushedSphere.Center, pushedSphereCenter);

		// とりあえず当たった座標で更新
		hitPos = tmpResult.m_hitPos;
	}

	if (_pRes && isHit)
	{
		// 最後に当たった座標が使用される
		_pRes->m_hitPos = hitPos;

		// 複数のメッシュに押された最終的な位置 - 移動前の位置 = 押し出しベクトル
		_pRes->m_hitDir = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&pushedSphere.Center), DirectX::XMLoadFloat3(&_target.Center));
		_pRes->m_overlapDistance = DirectX::XMVector3Length(_pRes->m_hitDir).m128_f32[0];
		_pRes->m_hitDir = DirectX::XMVector3Normalize(_pRes->m_hitDir);
	}

	return isHit;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* レイvsモデルの当たり判定 */
// ・判定回数は メッシュの個数 x 各メッシュのポリゴン数 
// 　計算回数がモデルのデータ依存のため処理効率は不安定
// ・単純に計算回数が多くなる可能性があるため重くなりがち
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
bool KdModelCollision::Intersects(const KdCollider::RayInfo& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes)
{
	// 当たり判定が無効 or 形状が解放済みなら判定せず返る
	if (!m_enable || !m_shape) { return false; }

	std::shared_ptr<KdModelData> spModelData = m_shape->GetData();

	// データが無ければ判定不能なので返る
	if (!spModelData) { return false; }

	CollisionMeshResult nearestResult;

	bool isHit = false;

	const std::vector<KdModelData::Node>& dataNodes = spModelData->GetOriginalNodes();
	const std::vector<KdModelWork::Node>& workNodes = m_shape->GetNodes();

	for (int index : spModelData->GetCollisionMeshNodeIndices())
	{
		const KdModelData::Node& dataNode = dataNodes[index];
		const KdModelWork::Node& workNode = workNodes[index];

		if (!dataNode.m_spMesh) { continue; }

		CollisionMeshResult tmpResult;
		CollisionMeshResult* pTmpResult = _pRes ? &tmpResult : nullptr;

		if (!MeshIntersect(*dataNode.m_spMesh, _target.m_pos, _target.m_dir, _target.m_range, workNode.m_worldTransform * _world, pTmpResult)) { continue; }

		// 詳細リザルトが必要無ければ即結果を返す
		if (!_pRes) { return true; }

		isHit = true;

		if (tmpResult.m_overlapDistance > nearestResult.m_overlapDistance) { nearestResult = tmpResult; }
	}

	if (_pRes && isHit)
	{
		// 最も近くで当たったヒット情報をコピーする
		_pRes->m_hitPos = nearestResult.m_hitPos;
		_pRes->m_hitDir = nearestResult.m_hitDir;
		_pRes->m_overlapDistance = nearestResult.m_overlapDistance;
	}

	return isHit;
}


/* =！=！=！=！=！=！=！=！=！=！=！=！= */
/* === PolygonCollision === */
// ・多角形ポリゴン(頂点の集合体)の形状
/* =！=！=！=！=！=！=！=！=！=！=！=！= */

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* 球vs多角形ポリゴン(頂点の集合体)の当たり判定 */
// ・判定回数は ポリゴンの個数 
// 　計算回数がポリゴンデータ依存のため処理効率は不安定
// ・単純に計算回数が多くなる可能性があるため重くなりがち
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
bool KdPolygonCollision::Intersects(const DirectX::BoundingSphere& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes)
{
	// 当たり判定が無効 or 形状が解放済みなら判定せず返る
	if (!m_enable || !m_shape) { return false; }

	CollisionMeshResult result;
	CollisionMeshResult* pTmpResult = _pRes ? &result : nullptr;

	// メッシュと球形の当たり判定実行：当たっていなければ無条件に返る
	if (!PolygonsIntersect(*m_shape, _target, _world, pTmpResult)) { return false; }

	if (_pRes)
	{
		_pRes->m_hitPos = result.m_hitPos;
		_pRes->m_hitDir = result.m_hitDir;
		_pRes->m_overlapDistance = result.m_overlapDistance;
	}

	return true;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/*  レイvs多角形ポリゴン(頂点の集合体)の当たり判定 */
// ・判定回数は ポリゴンの個数 
// 　計算回数がポリゴンデータ依存のため処理効率は不安定
// ・単純に計算回数が多くなる可能性があるため重くなりがち
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
bool KdPolygonCollision::Intersects(const KdCollider::RayInfo& _target, const Math::Matrix& _world, KdCollider::CollisionResult* _pRes)
{
	// 当たり判定が無効 or 形状が解放済みなら判定せず返る
	if (!m_enable || !m_shape) { return false; }
	
	CollisionMeshResult result;
	CollisionMeshResult* pTmpResult = _pRes ? &result : nullptr;

	// 当たっていなければ無条件に返る
	if (!PolygonsIntersect(*m_shape, _target.m_pos, _target.m_dir, _target.m_range, _world, pTmpResult)) { return false; }

	if (_pRes)
	{
		_pRes->m_hitPos = result.m_hitPos;
		_pRes->m_hitDir = result.m_hitDir;
		_pRes->m_overlapDistance = result.m_overlapDistance;
	}

	return true;
}