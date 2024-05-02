#pragma once

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ポリゴンの共通情報を扱う基底クラス
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// それぞれの派生クラスにおいて描画の際にポリゴンを形成する頂点情報を作成する機能を持たせる必要がある
// ポリゴンに反映させるマテリアルの情報を保持している
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class KdPolygon
{
public:
	struct Vertex
	{
		Math::Vector3 pos;
		Math::Vector2 UV;
		unsigned int  color = 0xFFFFFFFF;
		Math::Vector3 normal = Math::Vector3::Backward;
		Math::Vector3 tangent = Math::Vector3::Left;
	};

	KdPolygon() {}
	KdPolygon(const std::shared_ptr<KdTexture>& _spBaseColTex) { SetMaterial(_spBaseColTex); }
	KdPolygon(const std::string& _baseColTexName) { SetMaterial(_baseColTexName); }
	virtual ~KdPolygon(){}

	// 描画時に使用する頂点リストの取得
	const std::vector<Vertex>& GetVertices() const { return m_vertices; };
	// 判定用ポリゴン頂点座標のコピー
	void GetPositions(std::vector<Math::Vector3>& _res) const;

	// マテリアルの設定
	void SetMaterial(const std::shared_ptr<KdMaterial>& _spMaterial) { m_spMaterial = _spMaterial; }
	// ベースカラーテクスチャから
	void SetMaterial(const std::shared_ptr<KdTexture>& _spBaseColTex);
	// ベースカラーファイルパスから
	void SetMaterial(const std::string& _baseColTexName);

	virtual void SetColor(const Math::Color& _col) { if (m_spMaterial) { m_spMaterial->m_baseColorRate = _col; } }

	// マテリアルを取得
	const std::shared_ptr<KdMaterial>& GetMaterial() const { return m_spMaterial; }

	bool IsEnable() const { return (m_enable); }
	void SetEnable(bool _enable) { m_enable = _enable; }

	bool Is2DObject() const { return m_2DObject; }
	void Set2DObject(bool _is2DObject) { m_2DObject = _is2DObject; }

protected:
	// ポリゴンに描画するテクスチャ
	std::shared_ptr<KdMaterial> m_spMaterial = nullptr;
	std::vector<Vertex>			m_vertices;			// 頂点リスト
	bool						m_enable = true;	// 描画有効フラグ
	bool						m_2DObject = true;	// 2Dオブジェクトかどうか
};