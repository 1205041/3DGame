#include "Framework/KdFramework.h"

#include "KdMesh.h"
#include "KdGLTFLoader.h"

//=============================================================
// Mesh
//=============================================================
void KdMesh::SetToDevice() const
{
	// 頂点バッファセット
	UINT stride = sizeof(KdMeshVertex);	// 1頂点のサイズ
	UINT offset = 0;					// オフセット
	KdDirect3D::GetInstance().WorkDevContext()->IASetVertexBuffers(0, 1, m_vertBuf.GetAddress(), &stride, &offset);

	// インデックスバッファセット
	KdDirect3D::GetInstance().WorkDevContext()->IASetIndexBuffer(m_indxBuf.GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

	//プリミティブ・トポロジーをセット
	KdDirect3D::GetInstance().WorkDevContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//=============================================================
// 生成
// 頂点配列、インデックス配列、サブセット配列（マテリアルなど）の生成
//=============================================================
bool KdMesh::Create(const std::vector<KdMeshVertex>& _vertices, const std::vector<KdMeshFace>& _faces, const std::vector<KdMeshSubset>& _subsets, bool _isSkinMesh)
{
	Release();

	//------------------------------
	// サブセット情報
	//------------------------------
	m_subsets = _subsets;

	//------------------------------
	// 頂点バッファ作成
	//------------------------------
	if(_vertices.size() > 0)
	{
		// 書き込むデータ
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &_vertices[0];				// バッファに書き込む頂点配列の先頭アドレス
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		// 頂点バッファ作成
		if (FAILED(m_vertBuf.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(KdMeshVertex) * _vertices.size(), D3D11_USAGE_DEFAULT, &initData)))
		{
			Release();
			return false;
		}

		// 座標のみの配列
		m_positions.resize(_vertices.size());
		for (UINT i = 0; i < m_positions.size(); i++) { m_positions[i] = _vertices[i].Pos; }

		// AA境界データ作成
		DirectX::BoundingBox::CreateFromPoints(m_aabb, m_positions.size(), &m_positions[0], sizeof(Math::Vector3));
		// 境界球データ作成
		DirectX::BoundingSphere::CreateFromPoints(m_bs, m_positions.size(), &m_positions[0], sizeof(Math::Vector3));
	}	

	//------------------------------
	// インデックスバッファ作成
	//------------------------------
	if(_faces.size() > 0)
	{
		// 書き込むデータ
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &_faces[0];				// バッファに書き込む頂点配列の先頭アドレス
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		// バッファ作成
		if (FAILED(m_indxBuf.Create(D3D11_BIND_INDEX_BUFFER, _faces.size() * sizeof(KdMeshFace), D3D11_USAGE_DEFAULT, &initData)))
		{
			Release();
			return false;
		}

		// 面情報コピー
		m_faces = _faces;
	}

	m_isSkinMesh = _isSkinMesh;

	return true;
}

void KdMesh::Release()
{
	m_vertBuf.Release();
	m_indxBuf.Release();
	m_subsets.clear();
	m_positions.clear();
	m_faces.clear();
}


void KdMesh::DrawSubset(int _subsetNo) const
{
	// 範囲外のサブセットはスキップ
	if (_subsetNo >= (int)m_subsets.size())return;
	// 面数が0なら描画スキップ
	if (m_subsets[_subsetNo].FaceCount == 0)return;

	// 描画
	KdDirect3D::GetInstance().WorkDevContext()->DrawIndexed(m_subsets[_subsetNo].FaceCount * 3, m_subsets[_subsetNo].FaceStart * 3, 0);
}