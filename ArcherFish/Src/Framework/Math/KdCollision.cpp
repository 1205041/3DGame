#include "KdCollision.h"
using namespace DirectX;

/* =！=！=！=！=！= */
/* レイの当たり判定 */
/* =！=！=！=！=！= */

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* レイの情報を逆行列化する */
/* ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== */
// ・レイとポリゴンを判定する際に全ての頂点を行列移動させると
// 　ポリゴン数によって処理コストが変わるため非常に不安定
// ・レイの情報は1つしかないためレイだけを逆行列化する事で
// 　処理の安定化＋1度しか計算が行われないため最大の効率化にもなる
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
static void InvertRayInfo(DirectX::XMVECTOR& _rayPosInv, DirectX::XMVECTOR& _rayDirInv, float& _rayRangeInv, float& _scaleInv, 
	const DirectX::XMMATRIX& _matrix, const DirectX::XMVECTOR& _rayPos, const DirectX::XMVECTOR& _rayDir, float _rayRange)
{
	// ターゲットの逆行列でレイを変換
	DirectX::XMMATRIX invMat = XMMatrixInverse(0, _matrix);

	// レイの判定開始位置を逆変換
	_rayPosInv = XMVector3TransformCoord(_rayPos, invMat);

	// レイの方向を逆変換
	_rayDirInv = XMVector3TransformNormal(_rayDir, invMat);

	// 拡大率を逆変換
	_scaleInv = DirectX::XMVector3Length(_rayDirInv).m128_f32[0];

	// レイの方向ベクトルの長さ=拡大率で判定限界距離を補正
	// ※逆行列に拡縮が入っていると、レイの長さが変わるため
	// レイが当たった座標からの距離に拡縮が反映されてしまうので
	// 判定用の最大距離にも拡縮を反映させておく
	_rayRangeInv = _rayRange * _scaleInv;

	// 方角レイとして正しく扱うためには長さが１でなければならないので正規化
	_rayDirInv = DirectX::XMVector3Normalize(_rayDirInv);
}

/* = = = = = = = = = = = = = = = = = = = = = = = */
/* レイとの当たり判定結果をリザルトにセットする */
/* = = = = = = = = = = = = = = = = = = = = = = = */
static void SetRayResult(CollisionMeshResult& _result, bool _isHit, float _closestDist, 
	const DirectX::XMVECTOR& _rayPos, const DirectX::XMVECTOR& _rayDir, float _rayRange)
{
	// リザルトに結果を格納
	_result.m_hit = _isHit;
	_result.m_hitPos = DirectX::XMVectorAdd(_rayPos, DirectX::XMVectorScale(_rayDir, _closestDist));
	_result.m_hitDir = DirectX::XMVectorScale(_rayDir, -1);
	_result.m_overlapDistance = _rayRange - _closestDist;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* レイ対ポリゴン(KdMesh以外の任意の多角形ポリゴン)の当たり判定本体 */
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
bool PolygonsIntersect(const KdPolygon& _poly, const DirectX::XMVECTOR& _rayPos, const DirectX::XMVECTOR& _rayDir,
	float _rayRange, const DirectX::XMMATRIX& _matrix, CollisionMeshResult* _pResult)
{
	//--------------------------------------------------------
	// レイの逆行列化
	//--------------------------------------------------------
	DirectX::XMVECTOR rayPosInv, rayDirInv;
	float rayRangeInv = 0.0f;
	float scaleInv = 0.0f;

	InvertRayInfo(rayPosInv, rayDirInv, rayRangeInv, scaleInv, _matrix, _rayPos, _rayDir, _rayRange);

	//--------------------------------------------------------
	// レイ vs 全ての面
	//--------------------------------------------------------

	// ヒット判定
	bool isHit = false;
	float closestDist = FLT_MAX;

	// 頂点リスト取得
	std::vector<Math::Vector3> positions;
	_poly.GetPositions(positions);
	size_t faceNum = positions.size() - 2;

	// 全ての面(三角形)
	for (UINT faceIdx = 0; faceIdx < faceNum; ++faceIdx)
	{
		// レイと三角形の判定
		float hitDist = FLT_MAX;
		if (!DirectX::TriangleTests::Intersects(rayPosInv, rayDirInv, positions[faceIdx], positions[faceIdx + 1], positions[faceIdx + 2], hitDist)) { continue; }

		// レイの判定範囲外なら無視
		if (hitDist > rayRangeInv) { continue; }

		// CollisionResult無しなら結果は関係ないので当たった時点で返る
		if (!_pResult) { return isHit; }

		// 最短距離の更新判定処理
		closestDist = std::min(hitDist, closestDist);

		isHit = true;
	}

	if (_pResult && isHit) { SetRayResult(*_pResult, isHit, closestDist / scaleInv, _rayPos, _rayDir, _rayRange); }

	return isHit;
}

/* = = = = = = = = = = = = = = = = */
/* レイ対メッシュの当たり判定本体 */
/* = = = = = = = = = = = = = = = = */
bool MeshIntersect(const KdMesh& _mesh, const DirectX::XMVECTOR& _rayPos, const DirectX::XMVECTOR& _rayDir,
	float _rayRange, const DirectX::XMMATRIX& _matrix, CollisionMeshResult* _pResult)
{
	//--------------------------------------------------------
	// ブロードフェイズ
	// 　比較的軽量なAABB vs レイな判定で、
	// 　あきらかにヒットしない場合は、これ以降の判定を中止
	//--------------------------------------------------------
	{
		// AABB vs レイ
		float AABBdist = FLT_MAX;
		DirectX::BoundingBox aabb;
		_mesh.GetBoundingBox().Transform(aabb, _matrix);
		if (!aabb.Intersects(_rayPos, _rayDir, AABBdist)) { return false; }

		// 最大距離外なら範囲外なので中止
		if (AABBdist > _rayRange) { return false; }
	}

	//--------------------------------------------------------
	// レイの逆行列化
	//--------------------------------------------------------
	DirectX::XMVECTOR rayPosInv, rayDirInv;
	float rayRangeInv = 0;
	float scaleInv = 0;

	InvertRayInfo(rayPosInv, rayDirInv, rayRangeInv, scaleInv, _matrix, _rayPos, _rayDir, _rayRange);

	//--------------------------------------------------------
	// ナローフェイズ
	// 　レイ vs 全ての面
	//--------------------------------------------------------

	// ヒット判定
	bool isHit = false;
	float closestDist = FLT_MAX;

	// DEBUGビルドでも速度を維持するため、別変数に拾っておく
	const KdMeshFace* pFaces = &_mesh.GetFaces()[0];
	auto& vertices = _mesh.GetVertexPositions();
	UINT faceNum = _mesh.GetFaces().size();

	// 全ての面(三角形)
	for (UINT faceIdx = 0; faceIdx < faceNum; ++faceIdx)
	{
		// 三角形を構成する３つの頂点のIndex
		const UINT* idx = pFaces[faceIdx].Idx;

		// レイと三角形の判定
		float hitDist = FLT_MAX;
		if (!DirectX::TriangleTests::Intersects(rayPosInv, rayDirInv, vertices[idx[0]], vertices[idx[1]], vertices[idx[2]], hitDist)) { continue; }

		// レイの判定範囲外なら無視
		if (hitDist > rayRangeInv) { continue; }
	
		// CollisionResult無しなら結果は関係ないので当たった時点で返る
		if (!_pResult) { return isHit; }

		// 最短距離の更新判定処理
		closestDist = std::min(hitDist, closestDist);

		isHit = true;
	}

	if (_pResult && isHit) { SetRayResult(*_pResult, isHit, closestDist / scaleInv, _rayPos, _rayDir, _rayRange); }

	return isHit;
}


/* =！=！=！=！=！=！=！= */
/* スフィアの当たり判定 */
/* =！=！=！=！=！=！=！= */

/* = = = = = = = = = = = = = = = */
// スフィアの情報を逆行列化する
/* ===== ===== ===== ===== ===== */
// ・レイと同様の理由
/* = = = = = = = = = = = = = = = */
static void InvertSphereInfo(DirectX::XMVECTOR& _spherePosInv, DirectX::XMVECTOR& _sphereScale, float& _radiusSqr,
	const DirectX::XMMATRIX& _matrix, const DirectX::BoundingSphere& _sphere)
{
	// メッシュの逆行列で、球の中心座標を変換(メッシュの座標系へ変換される)
	DirectX::XMMATRIX invMat = XMMatrixInverse(0, _matrix);
	_spherePosInv = XMVector3TransformCoord(XMLoadFloat3(&_sphere.Center), invMat);

	// 半径の二乗(判定の高速化用)
	_radiusSqr = _sphere.Radius * _sphere.Radius;	// 半径の２乗

	// 行列の各軸の拡大率を取得しておく
	_sphereScale.m128_f32[0] = DirectX::XMVector3Length(_matrix.r[0]).m128_f32[0];
	_sphereScale.m128_f32[1] = DirectX::XMVector3Length(_matrix.r[1]).m128_f32[0];
	_sphereScale.m128_f32[2] = DirectX::XMVector3Length(_matrix.r[2]).m128_f32[0];
	_sphereScale.m128_f32[3] = 0;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* スフィアとポリゴンの最近接点を元に接触しているかどうかを判定 */
// ・次のポリゴンの判定の間に当たらない位置までスフィアを移動させる
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
static bool HitCheckAndPosUpdate(DirectX::XMVECTOR& _finalPos, DirectX::XMVECTOR& _finalHitPos, 
	const DirectX::XMVECTOR& _nearPoint, const DirectX::XMVECTOR& _objScale, float _radiusSqr, float _sphereRadius)
{
	// 最近接点→球の中心　ベクトルを求める
	DirectX::XMVECTOR vToCenter = _finalPos - _nearPoint;

	// XYZ軸が別々の大きさで拡縮されてる状態の場合、球が変形してる状態なため正確な半径がわからない。
	// そこでscaleをかけてXYZ軸のスケールが均等な座標系へ変換する
	vToCenter *= _objScale;

	// 最近接点が半径の2乗より遠い場合は、衝突していない
	if (DirectX::XMVector3LengthSq(vToCenter).m128_f32[0] > _radiusSqr) { return false; }

	// 押し戻し計算
	// めり込んでいるぶんのベクトルを計算
	DirectX::XMVECTOR vPush = DirectX::XMVector3Normalize(vToCenter);
	vPush *= _sphereRadius - DirectX::XMVector3Length(vToCenter).m128_f32[0];

	// 拡縮を考慮した座標系へ戻す
	vPush /= _objScale;

	// 球の中心座標を更新
	_finalPos += vPush;
	_finalHitPos = _nearPoint;

	return true;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = */
/* スフィアとの当たり判定結果をリザルトにセットする */
/* = = = = = = = = = = = = = = = = = = = = = = = = = */
static void SetSphereResult(CollisionMeshResult& _result, bool _isHit, const DirectX::XMVECTOR& _hitPos,
	const DirectX::XMVECTOR& _finalPos, const DirectX::XMVECTOR& _beginPos)
{
	_result.m_hit = _isHit;
	_result.m_hitPos = _hitPos;
	_result.m_hitDir = DirectX::XMVectorSubtract(_finalPos, _beginPos);
	_result.m_overlapDistance = DirectX::XMVector3Length(_result.m_hitDir).m128_f32[0];
	_result.m_hitDir = DirectX::XMVector3Normalize(_result.m_hitDir);
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* スフィア対ポリゴン(KdMesh以外の任意の多角形ポリゴン)の当たり判定本体 */
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
bool PolygonsIntersect(const KdPolygon& _poly, const DirectX::BoundingSphere& _sphere, const DirectX::XMMATRIX& _matrix, CollisionMeshResult* _pResult)
{
	//------------------------------------------
	// 球とポリゴンとの詳細判定
	//------------------------------------------
	// １つでもヒットしたらtrue
	bool isHit = false;

	// 頂点リスト取得
	std::vector<Math::Vector3> positions;
	_poly.GetPositions(positions);
	size_t faceNum = positions.size() - 2;

	DirectX::XMVECTOR finalHitPos = {};	// 当たった座標の中でも最後の座標
	DirectX::XMVECTOR finalPos = {};	// 各面に押されて最終的に到達する座標：判定する球の中心
	DirectX::XMVECTOR objScale = {};	// ターゲットオブジェクトの各軸の拡大率
	float radiusSqr = 0.0f;
	InvertSphereInfo(finalPos, objScale, radiusSqr, _matrix, _sphere);

	// 全ての面と判定
	// ※判定はポリゴンのローカル空間で行われる
	for (UINT faceIndx = 0; faceIndx < faceNum; faceIndx++)
	{
		DirectX::XMVECTOR nearPoint;

		// 点 と 三角形 の最近接点を求める
		KdPointToTriangle(finalPos, positions[faceIndx], positions[faceIndx + 1], positions[faceIndx + 2], nearPoint);

		// 当たっているかどうかの判定と最終座標の更新
		isHit |= HitCheckAndPosUpdate(finalPos, finalHitPos, nearPoint, objScale, radiusSqr, _sphere.Radius);

		// CollisionResult無しなら結果は関係ないので当たった時点で返る
		if (!_pResult && isHit) { return isHit; }
	}

	// リザルトに結果を格納
	if (_pResult && isHit)
	{
		SetSphereResult(*_pResult, isHit, XMVector3TransformCoord(finalHitPos, _matrix),
			XMVector3TransformCoord(finalPos, _matrix), XMLoadFloat3(&_sphere.Center));
	}

	return isHit;
}

/* = = = = = = = = = = = = = = = = = = */
/* スフィア対メッシュの当たり判定本体 */
/* = = = = = = = = = = = = = = = = = = */
bool MeshIntersect(const KdMesh& _mesh, const DirectX::BoundingSphere& _sphere,const DirectX::XMMATRIX& _matrix, CollisionMeshResult* _pResult)
{
	//------------------------------------------
	// ブロードフェイズ
	// 　高速化のため、まずは境界ボックス(AABB)で判定
	// 　この段階でヒットしていないなら、詳細な判定をする必要なし
	//------------------------------------------
	{
		// メッシュのAABBを元に、行列で変換したAABBを作成
		DirectX::BoundingBox aabb;
		_mesh.GetBoundingBox().Transform(aabb, _matrix);

		if (!aabb.Intersects(_sphere)) { return false; }
	}

	//------------------------------------------
	// ナローフェイズ
	// 　球とメッシュとの詳細判定
	//------------------------------------------
	// １つでもヒットしたらtrue
	bool isHit = false;

	// DEBUGビルドでも速度を維持するため、別変数に拾っておく
	const auto* pFaces = &_mesh.GetFaces()[0];
	UINT faceNum = _mesh.GetFaces().size();
	auto& vertices = _mesh.GetVertexPositions();

	DirectX::XMVECTOR finalHitPos = {};	// 当たった座標の中でも最後の座標
	DirectX::XMVECTOR finalPos = {};	// 各面に押されて最終的に到達する座標：判定する球の中心
	DirectX::XMVECTOR objScale = {};	// ターゲットオブジェクトの各軸の拡大率
	float radiusSqr = 0.0f;
	InvertSphereInfo(finalPos, objScale, radiusSqr, _matrix, _sphere);

	// 全ての面と判定
	// ※判定はメッシュのローカル空間で行われる
	for (UINT faceIdx = 0; faceIdx < faceNum; faceIdx++)
	{
		DirectX::XMVECTOR nearPoint;

		// 三角形を構成する３つの頂点のIndex
		const UINT* idx = pFaces[faceIdx].Idx;

		// 点 と 三角形 の最近接点を求める
		KdPointToTriangle(finalPos, vertices[idx[0]], vertices[idx[1]], vertices[idx[2]], nearPoint);

		// 当たっているかどうかの判定と最終座標の更新
		isHit |= HitCheckAndPosUpdate(finalPos, finalHitPos, nearPoint, objScale, radiusSqr, _sphere.Radius);

		// CollisionResult無しなら結果は関係ないので当たった時点で返る
		if (!_pResult && isHit) { return isHit; }
	}

	// リザルトに結果を格納
	if (_pResult && isHit)
	{
		SetSphereResult(*_pResult, isHit, XMVector3TransformCoord(finalHitPos, _matrix), 
			XMVector3TransformCoord(finalPos, _matrix), XMLoadFloat3(&_sphere.Center));
	}

	return isHit;
}

/* = = = = = = = = = = = = = = = = = = = = = = = */
/* 点 vs 面を形成する三角形との最近接点を求める */
/* = = = = = = = = = = = = = = = = = = = = = = = */
void KdPointToTriangle(const XMVECTOR& _p, const XMVECTOR& _a, const XMVECTOR& _b, const XMVECTOR& _c, DirectX::XMVECTOR& _outPt)
{
	// ※参考:[書籍]「ゲームプログラミングのためのリアルタイム衝突判定」

	// pがaの外側の頂点領域の中にあるかどうかチェック
	XMVECTOR ab = _b - _a;
	XMVECTOR ac = _c - _a;
	XMVECTOR ap = _p - _a;

	float d1 = XMVector3Dot(ab, ap).m128_f32[0];//ab.Dot(ap);
	float d2 = XMVector3Dot(ac, ap).m128_f32[0];//ac.Dot(ap);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// 重心座標(1,0,0)
		_outPt = _a;
		return;
	}

	// pがbの外側の頂点領域の中にあるかどうかチェック
	XMVECTOR bp = _p - _b;
	float d3 = XMVector3Dot(ab, bp).m128_f32[0];//ab.Dot(bp);
	float d4 = XMVector3Dot(ac, bp).m128_f32[0];//ac.Dot(bp);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// 重心座標(0,1,0)
		_outPt = _b;
		return;
	}

	// pがabの辺領域の中にあるかどうかチェックし、あればpのab上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;

	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		_outPt = _a + ab * v;	// 重心座標(1-v,v,0)
		return;
	}

	// pがcの外側の頂点領域の中にあるかどうかチェック
	XMVECTOR cp = _p - _c;
	float d5 = XMVector3Dot(ab, cp).m128_f32[0];//ab.Dot(cp);
	float d6 = XMVector3Dot(ac, cp).m128_f32[0];;//ac.Dot(cp);

	if (d6 >= 0.0f && d5 <= d6)
	{
		_outPt = _c;	// 重心座標(0,0,1)
		return;
	}

	// pがacの辺領域の中にあるかどうかチェックし、あればpのac上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;

	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		_outPt = _a + ac * w;	// 重心座標(1-w,0,w)
		return;
	}

	// pがbcの辺領域の中にあるかどうかチェックし、あればpのbc上に対する射影を返す
	float va = d3 * d6 - d5 * d4;

	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		_outPt = _b + (_c - _b) * w;	// 重心座標(0,1-w,w)
		return;
	}

	// pは面領域の中にある。Qをその重心座標(u,v,w)を用いて計算
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	_outPt = _a + ab * v + ac * w;	// = u*a + v*b + w*c, u = va*demon = 1.0f - v - w
}