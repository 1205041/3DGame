﻿#pragma once

//============================================================
// HD2D用の基本シェーダー
//============================================================
class KdHD2DShader
{
public:

	// 定数バッファ(オブジェクト単位更新)
	struct cbObject
	{
		// UV操作
		Math::Vector2	UVOffset = { 0.0f, 0.0f };
		Math::Vector2	UVTiling = { 1.0f, 1.0f };
		/* ============================ */

		// フォグ有効
		int				FogEnable = 1;

		// エミッシブのみの描画
		int				OnlyEmissie = 0;

		// ディゾルブ関連
		float			DissolveThreshold = 0.0f;	// 0 ～ 1
		float			DissolveEdgeRange = 0.03f;	// 0 ～ 1
		/* ============================ */

		Math::Vector3	DissolveEmissive = { 0.0f, 1.0f, 1.0f };
		
		// add：色変更
		int				ColorEnable = 0;
		/* ============================ */

		// add：水面表現
		Math::Matrix mR;

		int WaterEnable = 0;
		Math::Vector2 WaterUVOffset;
		float _dummy = 0.0f;
		/* ============================ */
	};

	// 定数バッファ(メッシュ単位更新)
	struct cbMesh
	{
		Math::Matrix	mW;
	};

	// 定数バッファ(マテリアル単位更新)
	struct cbMaterial
	{
		Math::Vector4	BaseColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		Math::Vector3	Emissive = { 1.0f, 1.0f, 1.0f };
		float			Metallic = 0.0f;

		float			Roughness = 1.0f;
		float			_blank[3] = { 0.0f, 0.0f ,0.0f };
	};

	//================================================
	// 設定・取得
	//================================================

	// add：水面表現用 ===============================
	void SetWaterNomalText(KdTexture& _text)
	{
		// テクスチャをGPUに転送
		KdDirect3D::GetInstance().WorkDevContext()->PSSetShaderResources(20, 1, _text.WorkSRViewAddress());
		/* マルチレンダーターゲットが使われている */
	}

	void SetWaterEnable(bool _enable)
	{
		m_cb0_Obj.Work().WaterEnable = _enable;
		m_cb0_Obj.Work().mR = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90.0f));

		m_dirtyCBObj = true;
	}

	void SetWaterUVOffset(const Math::Vector2& _offset)
	{
		m_cb0_Obj.Work().WaterUVOffset = _offset;

		m_dirtyCBObj = true;
	}

	// add：色変更有効/無効
	void SetColorEnable(bool _enable)
	{
		m_cb0_Obj.Work().ColorEnable = _enable;

		m_dirtyCBObj = true;
	}
	/* =============================================== */

	// UVタイリング設定
	void SetUVTiling(const Math::Vector2& _tiling)
	{
		m_cb0_Obj.Work().UVTiling = _tiling;

		m_dirtyCBObj = true;
	}

	// UVオフセット設定
	void SetUVOffset(const Math::Vector2& _offset)
	{
		m_cb0_Obj.Work().UVOffset = _offset;

		m_dirtyCBObj = true;
	}

	// フォグ有効/無効
	void SetFogEnable(bool _enable)
	{
		m_cb0_Obj.Work().FogEnable = _enable;

		m_dirtyCBObj = true;
	}

	// ディゾルブ設定
	void SetDissolve(float _threshold, const float* _range = nullptr, const Math::Vector3* _edgeColor = nullptr)
	{
		auto& cbObj = m_cb0_Obj.Work();

		cbObj.DissolveThreshold = _threshold;

		if (_range) { cbObj.DissolveEdgeRange = *_range; }

		if (_edgeColor) { cbObj.DissolveEmissive = *_edgeColor; }

		m_dirtyCBObj = true;
	}

	// ディゾルブテクスチャ設定
	void SetDissolveTexture(KdTexture& _dissolveMask)
	{
		KdDirect3D::GetInstance().WorkDevContext()->PSSetShaderResources(11, 1, _dissolveMask.WorkSRViewAddress());
	}

	// デフォルトディゾルブテクスチャ設定
	void SetDefaultDissolveTexture(std::shared_ptr<KdTexture>& _spDissolveMask)
	{
		if (!_spDissolveMask) { return; }

		m_dissolveTex = _spDissolveMask;

		SetDissolveTexture(*_spDissolveMask);
	}

	// デフォルトのディゾルブテクスチャに戻す
	void ResetDissolveTexture()
	{
		if (!m_dissolveTex) { return; }

		SetDissolveTexture(*m_dissolveTex);
	}

	//================================================
	// 各定数バッファの取得
	//================================================
	const cbObject& GetObjectCB() const { return m_cb0_Obj.Get(); }

	const cbMesh& MeshCB() const { return m_cb1_Mesh.Get(); }

	const cbMaterial& WorkMaterialCB() const { return m_cb2_Material.Get(); }

	//================================================
	// 描画準備
	//================================================
	// 陰影をつけるオブジェクト等を描画する前後に行う
	void BeginLit();
	void EndLit();

	// 陰影をつけないオブジェクト等を描画する前後に行う
	void BeginUnLit();
	void EndUnLit();

	// 最も初めに行う、光を遮るオブジェクトを描画する前後に行う
	void BeginGenerateDepthMapFromLight();
	void EndGenerateDepthMapFromLight();

	//================================================
	// 描画関数
	//================================================
	// メッシュ描画
	void DrawMesh(const KdMesh* _mesh, const Math::Matrix& _mWorld, 
		const std::vector<KdMaterial>& _materials, const Math::Vector4& _col, 
		const Math::Vector3& _emissive);

	// モデルデータ描画：アニメーションに非対応
	void DrawModel(const KdModelData& _rModel, const Math::Matrix& _mWorld = Math::Matrix::Identity, 
		const Math::Color& _colRate = kWhiteColor, const Math::Vector3& _emissive = Math::Vector3::Zero);

	// モデルワーク描画：アニメーションに対応
	void DrawModel(KdModelWork& _rModel, const Math::Matrix& _mWorld = Math::Matrix::Identity, 
		const Math::Color& _colRate = kWhiteColor, const Math::Vector3& _emissive = Math::Vector3::Zero);

	// 任意の頂点群からなるポリゴン描画
	void DrawPolygon(const KdPolygon& _poly, const Math::Matrix& _mWorld = Math::Matrix::Identity,
		const Math::Color& _colRate = kWhiteColor, const Math::Vector3& _emissive = Math::Vector3::Zero);

	// 任意の頂点群からなるポリゴンライン描画
	void DrawVertices(const std::vector<KdPolygon::Vertex>& _vertices, const Math::Matrix& _mWorld = Math::Matrix::Identity, 
		const Math::Color& _colRate = kWhiteColor);

	//================================================
	// 初期化・解放
	//================================================

	// 初期化
	bool Init();
	// 解放
	void Release();

	~KdHD2DShader() { Release(); }

	std::shared_ptr<KdTexture>& GetDepthTex() { return m_depthMapFromLightRTPack.m_RTTexture; }

private:
	// マテリアルのセット
	void WriteMaterial(const KdMaterial& _material, const Math::Vector4& _colRate, const Math::Vector3& _emiRate);

	// ポリゴンの法線情報を2Dように書き換える
	void ConvertNormalsFor2D(std::vector<KdPolygon::Vertex>& _target, const Math::Matrix& _mWorld);

	// 定数バッファを初期状態に戻す
	void ResetCBObject();

	// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
	// Lit：陰影をつけるオブジェクトの描画用（不透明な物体やキャラクタの板ポリなど
	// 平行光・点光源などの影響を受け角度によって色を変化させるオブジェクトを描画するシェーダー
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// UnLit：陰影のつかないオブジェクトの描画用（エフェクトや透明な物体など
	// 光の計算を行わずマテリアルの色をそのまま出力するシェーダー
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// GenDepthFromLight：光から見たオブジェクトの距離を赤で出力用
	// Litシェーダーで影の描画を行うために必要な情報テクスチャを作成するシェーダー
	// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////

	// 頂点シェーダー
	ID3D11VertexShader* m_VS_Lit = nullptr;					// 陰影あり
	ID3D11VertexShader* m_VS_UnLit = nullptr;				// 陰影なし
	ID3D11VertexShader* m_VS_GenDepthFromLight = nullptr;	// 光からの深度

	// 頂点入力レイアウト
	ID3D11InputLayout* m_inputLayout = nullptr;
	
	// ピクセルシェーダー
	ID3D11PixelShader* m_PS_Lit = nullptr;					// 陰影あり
	ID3D11PixelShader* m_PS_UnLit = nullptr;				// 陰影なし
	ID3D11PixelShader* m_PS_GenDepthFromLight = nullptr;	// 光からの深度

	// テクスチャ
	std::shared_ptr<KdTexture>	m_dissolveTex = nullptr;	// ディゾルブで使用するデフォルトテクスチャ

	// 定数バッファ
	KdConstantBuffer<cbObject>		m_cb0_Obj;				// オブジェクト単位で更新
	KdConstantBuffer<cbMesh>		m_cb1_Mesh;				// メッシュ毎に更新
	KdConstantBuffer<cbMaterial>	m_cb2_Material;			// マテリアル毎に更新

	KdRenderTargetPack	m_depthMapFromLightRTPack;
	KdRenderTargetChanger m_depthMapFromLightRTChanger;

	bool		m_dirtyCBObj = false;						// 定数バッファのオブジェクトに変更があったかどうか
};