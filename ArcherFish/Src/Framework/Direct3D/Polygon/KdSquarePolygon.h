#pragma once

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 四角形ポリゴンクラス
// テクスチャの一部分を描画したり、描画する板ポリのサイズを設定できる
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class KdSquarePolygon : public KdPolygon
{
public:

	// ポリゴンの原点の位置
	enum class PivotType
	{
		Right_Top,		// 右上
		Right_Middle,	// 右中段
		Right_Bottom,	// 右下
		Center_Top = 10,// 中央上
		Center_Middle,	// 中央中段
		Center_Bottom,	// 中央下
		Left_Top = 20,	// 左上
		Left_Middle,	// 左中段
		Left_Bottom,	// 左下
	};

	KdSquarePolygon() { InitVertices(); }
	KdSquarePolygon(const std::shared_ptr<KdTexture>& _spBaseColTex) 
		: KdPolygon(_spBaseColTex) 
	{
		InitVertices(); SetScale(1.0f);
	}

	KdSquarePolygon(const std::string& _baseColTexName) 
		: KdPolygon(_baseColTexName) 
	{ 
		InitVertices(); SetScale(1.0f); 
	}

	~KdSquarePolygon() override {}

	void SetPivot(PivotType _pivot);

	// 描画の幅と高さの設定
	void SetScale(const Math::Vector2& _scale);
	void SetScale(float _scalar);

	// 頂点の描画色の設定、それぞれの頂点色を指定
	void SetVertexColor(const std::vector<Math::Color>& _vertCols);

	// テクスチャ内の描画エリアの設定
	void SetUVRect(UINT _index);
	void SetUVRect(UINT _x, UINT _y);
	void SetUVRect(const Math::Rectangle& _rect);
	void SetUVRect(const Math::Vector2& _minUV, const Math::Vector2& _maxUV);

	// テクスチャの分割数を設定
	inline void SetSplit(UINT _splitX, UINT _splitY)
	{
		m_splitX = _splitX;
		m_splitY = _splitY;
	}

	UINT GetSplitX() { return m_splitX; }
	UINT GetSplitY() { return m_splitY; }

private:
	void InitVertices();

	UINT m_splitX = 1;
	UINT m_splitY = 1;

	PivotType m_pivotType = PivotType::Center_Middle;
};