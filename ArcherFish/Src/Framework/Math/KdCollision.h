#pragma once

/* = = = = = = = = = = = = = */
// メッシュの当たり判定結果
/* = = = = = = = = = = = = = */
struct CollisionMeshResult
{
	DirectX::XMVECTOR m_hitPos = {};// 当たった座標
	DirectX::XMVECTOR m_hitDir = {};// 対象への方向ベクトル
	float m_overlapDistance = 0.0f; // 重なっている距離
	bool m_hit = false;				// 当たったかどうか
};

// レイの当たり判定
bool PolygonsIntersect(const KdPolygon& _poly, const DirectX::XMVECTOR& _rayPos, const DirectX::XMVECTOR& _rayDir, float _rayRange,
	const DirectX::XMMATRIX& _matrix, CollisionMeshResult* _pResult = nullptr);
//bool PolygonsIntersect(const KdPolygon& poly);
bool MeshIntersect(const KdMesh& _mesh, const DirectX::XMVECTOR& _rayPos, const DirectX::XMVECTOR& _rayDir, float _rayRange,
	const DirectX::XMMATRIX& _matrix, CollisionMeshResult* _pResult = nullptr);

// スフィアの当たり判定
bool PolygonsIntersect(const KdPolygon& _poly, const DirectX::BoundingSphere& _sphere,
	const DirectX::XMMATRIX& _matrix, CollisionMeshResult* _pResult = nullptr);
bool MeshIntersect(const KdMesh& _mesh, const DirectX::BoundingSphere& _sphere,
	const DirectX::XMMATRIX& _matrix, CollisionMeshResult* _pResult = nullptr);

// 点 vs 三角形面との最近接点を求める
void KdPointToTriangle(const DirectX::XMVECTOR& _point, const DirectX::XMVECTOR& _v1,
	const DirectX::XMVECTOR& _v2, const DirectX::XMVECTOR& _v3, DirectX::XMVECTOR& _nearestPoint);