#include "Framework/KdFramework.h"

#include "KdHD2DShader.h"
//================================================
// 描画準備
//================================================

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 陰影をつけるオブジェクトの描画の直前処理（不透明な物体やキャラクタの板ポリゴン）
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// シェーダーのパイプライン変更
// LitShaderで使用するリソースのバッファー設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::BeginLit()
{
	// 頂点シェーダーのパイプライン変更
	if (KdShaderManager::GetInstance().SetVertexShader(m_VS_Lit))
	{
		KdShaderManager::GetInstance().SetInputLayout(m_inputLayout);
		KdShaderManager::GetInstance().SetVSConstantBuffer(0, m_cb0_Obj.GetAddress());
		KdShaderManager::GetInstance().SetVSConstantBuffer(1, m_cb1_Mesh.GetAddress());
	}

	// ピクセルシェーダーのパイプライン変更
	if (KdShaderManager::GetInstance().SetPixelShader(m_PS_Lit))
	{
		KdShaderManager::GetInstance().SetPSConstantBuffer(0, m_cb0_Obj.GetAddress());
		KdShaderManager::GetInstance().SetPSConstantBuffer(2, m_cb2_Material.GetAddress());
	}

	// シャドウマップのテクスチャをセット
	KdDirect3D::GetInstance().WorkDevContext()->PSSetShaderResources(10, 1, m_depthMapFromLightRTPack.m_RTTexture->WorkSRViewAddress());

	// 影ぼかし用の比較機能付きサンプラーのセット
	KdShaderManager::GetInstance().ChangeSamplerState(KdSamplerState::Linear_Clamp_Cmp, 1);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 陰影ありオブジェクトの描画修了
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// 影を書き込む用に使用していたGenDepthFromLightで生成した深度SRVの解放
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::EndLit()
{
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	KdDirect3D::GetInstance().WorkDevContext()->PSSetShaderResources(10, 1, &pNullSRV);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 陰影をつけないオブジェクトの描画の直前処理（エフェクトや半透明物）
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// シェーダーのパイプライン変更
// UnLitShaderで使用するリソースのバッファー設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::BeginUnLit()
{
	if (KdShaderManager::GetInstance().SetVertexShader(m_VS_UnLit))
	{
		KdShaderManager::GetInstance().SetInputLayout(m_inputLayout);
		KdShaderManager::GetInstance().SetVSConstantBuffer(0, m_cb0_Obj.GetAddress());
		KdShaderManager::GetInstance().SetVSConstantBuffer(1, m_cb1_Mesh.GetAddress());
	}

	if (KdShaderManager::GetInstance().SetPixelShader(m_PS_UnLit))
	{
		KdShaderManager::GetInstance().SetPSConstantBuffer(0, m_cb0_Obj.GetAddress());
		KdShaderManager::GetInstance().SetPSConstantBuffer(2, m_cb2_Material.GetAddress());
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 陰影なしオブジェクトの描画終了
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::EndUnLit()
{
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 影を生み出すオブジェクトの情報描画（光を遮る物体）
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// シェーダーのパイプライン変更
// GenDepthMapFromLightShaderで使用するリソースのバッファー設定
// 書き込むテクスチャーを深度用の赤一色のテクスチャに切り替え
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::BeginGenerateDepthMapFromLight()
{
	if (KdShaderManager::GetInstance().SetVertexShader(m_VS_GenDepthFromLight))
	{
		KdShaderManager::GetInstance().SetInputLayout(m_inputLayout);
		KdShaderManager::GetInstance().SetVSConstantBuffer(0, m_cb0_Obj.GetAddress());
		KdShaderManager::GetInstance().SetVSConstantBuffer(1, m_cb1_Mesh.GetAddress());
	}

	if (KdShaderManager::GetInstance().SetPixelShader(m_PS_GenDepthFromLight)) { KdShaderManager::GetInstance().SetPSConstantBuffer(0, m_cb0_Obj.GetAddress()); }

	m_depthMapFromLightRTPack.ClearTexture(kRedColor);
	m_depthMapFromLightRTChanger.ChangeRenderTarget(m_depthMapFromLightRTPack);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 影を生み出すオブジェクトの描画終了
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// レンダーターゲットを元に戻す
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::EndGenerateDepthMapFromLight()
{
	m_depthMapFromLightRTChanger.UndoRenderTarget();
}

//================================================
// 描画関数
//================================================

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// メッシュを描画
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// メッシュの頂点データや3Dワールド情報・マテリアル情報をシェーダー(GPU)に転送する
// サブセットごとに描画命令を呼び出す：サブセットの個数分処理が重くなる
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::DrawMesh(const KdMesh* _mesh, const Math::Matrix& _mWorld,const std::vector<KdMaterial>& _materials, const Math::Vector4& _colRate, const Math::Vector3& _emissive)
{
	if (_mesh == nullptr) { return; }

	// メッシュの頂点情報転送
	_mesh->SetToDevice();

	// 3Dワールド行列転送
	m_cb1_Mesh.Work().mW = _mWorld;
	m_cb1_Mesh.Write();

	// 全サブセット
	for (UINT subi = 0; subi < _mesh->GetSubsets().size(); subi++)
	{
		// 面が１枚も無い場合はスキップ
		if (_mesh->GetSubsets()[subi].FaceCount == 0)continue;

		// マテリアルデータの転送
		const KdMaterial& material = _materials[_mesh->GetSubsets()[subi].MaterialNo];
		WriteMaterial(material, _colRate, _emissive);

		//-----------------------
		// サブセット描画
		//-----------------------
		_mesh->DrawSubset(subi);
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// モデルデータを描画（スタティック(アニメーションをしない)なモデル専用
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// データに所属する全ての描画用メッシュを描画する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::DrawModel(const KdModelData& _rModel, const Math::Matrix& _mWorld, const Math::Color& _colRate, const Math::Vector3& _emissive)
{
	// オブジェクト単位の情報転送
	if (m_dirtyCBObj) { m_cb0_Obj.Write(); }

	auto& dataNodes = _rModel.GetOriginalNodes();

	// 全描画用メッシュノードを描画
	for (auto& nodeIdx : _rModel.GetDrawMeshNodeIndices()) { DrawMesh(dataNodes[nodeIdx].m_spMesh.get(), dataNodes[nodeIdx].m_worldTransform * _mWorld, _rModel.GetMaterials(), _colRate, _emissive); }

	// 定数に変更があった場合は自動的に初期状態に戻す
	if (m_dirtyCBObj) { ResetCBObject(); }
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// モデルワークを描画（ダイナミック(アニメーションをしない)なモデルに対応
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// データに所属する全ての描画用メッシュをワークの3D行列に従って描画する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::DrawModel(KdModelWork& _rModel, const Math::Matrix& _mWorld,const Math::Color& _colRate, const Math::Vector3& _emissive)
{
	if (!_rModel.IsEnable()) { return; }

	const std::shared_ptr<KdModelData>& data = _rModel.GetData();

	// データがないときはスキップ
	if (data == nullptr) { return; }

	if (_rModel.NeedCalcNodeMatrices()) { _rModel.CalcNodeMatrices(); }

	// オブジェクト単位の情報転送
	if (m_dirtyCBObj) { m_cb0_Obj.Write(); }

	auto& workNodes = _rModel.GetNodes();
	auto& dataNodes = data->GetOriginalNodes();

	// 全描画用メッシュノードを描画
	for (auto& nodeIdx : data->GetDrawMeshNodeIndices()) { DrawMesh(dataNodes[nodeIdx].m_spMesh.get(), workNodes[nodeIdx].m_worldTransform * _mWorld, data->GetMaterials(), _colRate, _emissive); }

	// 定数に変更があった場合は自動的に初期状態に戻す
	if (m_dirtyCBObj) { ResetCBObject(); }
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ポリゴンを描画（モデル以外のプログラム上で生成された頂点の集合体
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// データに所属する全ての描画用メッシュをワークの3D行列に従って描画する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::DrawPolygon(const KdPolygon& _rPolygon, const Math::Matrix& _mWorld,const Math::Color& _colRate, const Math::Vector3& _emissive)
{
	if (!_rPolygon.IsEnable()) { return; }

	// ポリゴン描画用の頂点取得
	auto& vertices = _rPolygon.GetVertices();

	// 頂点数が3より少なければポリゴンが形成できないので描画不能
	if (vertices.size() < 3) { return; }

	// オブジェクト単位の定数バッファで変更があった場合のみ情報転送
	if (m_dirtyCBObj) { m_cb0_Obj.Write(); }

	// 3Dワールド行列転送
	m_cb1_Mesh.Work().mW = _mWorld;
	m_cb1_Mesh.Write();

	// マテリアルの転送
	if (_rPolygon.GetMaterial()) { WriteMaterial(*_rPolygon.GetMaterial(), _colRate, _emissive); }
	else { WriteMaterial(KdMaterial(), _colRate, _emissive); }

	KdShaderManager::GetInstance().ChangeRasterizerState(KdRasterizerState::CullNone);

	// サンプラーステートの変更:ポリゴンの描画なので、テクスチャの末端が繰り返されると不自然な描画になるため変更が必要
	if (KdShaderManager::GetInstance().IsPixelArtStyle()) { KdShaderManager::GetInstance().ChangeSamplerState(KdSamplerState::Point_Clamp); }
	else { KdShaderManager::GetInstance().ChangeSamplerState(KdSamplerState::Anisotropic_Clamp); }

	// 描画パイプラインのチェック
	ID3D11VertexShader* pNowVS = nullptr;
	KdDirect3D::GetInstance().WorkDevContext()->VSGetShader(&pNowVS, nullptr, nullptr);
	bool isLitShader = false;
	isLitShader = m_VS_Lit == pNowVS;
	KdSafeRelease(pNowVS);

	// 陰影ありのシェーダーで2Dオブジェクトを描画する時
	if (isLitShader && _rPolygon.Is2DObject())
	{
		std::vector<KdPolygon::Vertex> _2DVertices = vertices;

		// ポリゴンの法線を光に向ける処理：どの方向に向いていても光の影響を正面からに受けるように変換
		ConvertNormalsFor2D(_2DVertices, _mWorld);

		// 2DObject用に変換した頂点配列を描画
		KdDirect3D::GetInstance().DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, _2DVertices.size(), &_2DVertices[0], sizeof(KdPolygon::Vertex));
	}
	// 頂点配列を描画
	else { KdDirect3D::GetInstance().DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, vertices.size(), &vertices[0], sizeof(KdPolygon::Vertex)); }

	KdShaderManager::GetInstance().UndoSamplerState();
	KdShaderManager::GetInstance().UndoRasterizerState();

	// 定数に変更があった場合は自動的に初期状態に戻す
	if (m_dirtyCBObj) { ResetCBObject(); }
}

void KdHD2DShader::DrawVertices(const std::vector<KdPolygon::Vertex>& _vertices, const Math::Matrix& _mWorld,const Math::Color& _colRate)
{
	// 頂点数が2より少なければポリゴンが形成できないので描画不能
	if (_vertices.size() < 2) { return; }

	// オブジェクト単位の定数バッファで変更があった場合のみ情報転送
	if (m_dirtyCBObj) { m_cb0_Obj.Write(); }

	// 3Dワールド行列転送
	m_cb1_Mesh.Work().mW = _mWorld;
	m_cb1_Mesh.Write();

	// マテリアルの転送
	WriteMaterial(KdMaterial(), _colRate, Math::Vector3::Zero);

	KdShaderManager::GetInstance().ChangeRasterizerState(KdRasterizerState::CullNone);
	KdShaderManager::GetInstance().ChangeDepthStencilState(KdDepthStencilState::ZDisable);

	// サンプラーステートの変更:ポリゴンの描画なので、テクスチャの末端が繰り返されると不自然な描画になるため変更が必要
	if (KdShaderManager::GetInstance().IsPixelArtStyle()) { KdShaderManager::GetInstance().ChangeSamplerState(KdSamplerState::Point_Clamp); }
	else { KdShaderManager::GetInstance().ChangeSamplerState(KdSamplerState::Anisotropic_Clamp); }

	// 描画パイプラインのチェック
	ID3D11VertexShader* pNowVS = nullptr;
	KdDirect3D::GetInstance().WorkDevContext()->VSGetShader(&pNowVS, nullptr, nullptr);
	bool isLitShader = false;
	isLitShader = m_VS_Lit == pNowVS;
	KdSafeRelease(pNowVS);

	// 頂点配列を描画
	KdDirect3D::GetInstance().DrawVertices(D3D_PRIMITIVE_TOPOLOGY_LINELIST, _vertices.size(), &_vertices[0], sizeof(KdPolygon::Vertex));

	KdShaderManager::GetInstance().UndoSamplerState();
	KdShaderManager::GetInstance().UndoDepthStencilState();
	KdShaderManager::GetInstance().UndoRasterizerState();

	// 定数に変更があった場合は自動的に初期状態に戻す
	if (m_dirtyCBObj) { ResetCBObject(); }
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// KdShaderManagerの初期化時に呼び出される
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// シェーダー本体の生成
// シェーダーで利用する定数バッファの生成
// 影用の光からの深度情報テクスチャを生成
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
bool KdHD2DShader::Init()
{
	//-------------------------------------
	// 頂点シェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdHD2DShader_VS_Lit.shaderInc"
		// 頂点シェーダー作成
		if (FAILED(KdDirect3D::GetInstance().WorkDev()->CreateVertexShader(
			compiledBuffer,
			sizeof(compiledBuffer),
			nullptr,
			&m_VS_Lit)))
		{
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}

		// １頂点の詳細な情報
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// 頂点入力レイアウト作成
		if (FAILED(KdDirect3D::GetInstance().WorkDev()->CreateInputLayout(
			&layout[0],				// 入力エレメント先頭アドレス
			layout.size(),			// 入力エレメント数
			&compiledBuffer[0],		// 頂点バッファのバイナリデータ
			sizeof(compiledBuffer),	// 上記のバッファサイズ
			&m_inputLayout))) 
		{
			assert(0 && "CreateInputLayout失敗");
			Release();
			return false;
		}
	}

	{
#include "KdHD2DShader_VS_GenDepthMapFromLight.shaderInc"
		// 頂点シェーダー作成
		if (FAILED(KdDirect3D::GetInstance().WorkDev()->CreateVertexShader(
			compiledBuffer,
			sizeof(compiledBuffer),
			nullptr,
			&m_VS_GenDepthFromLight)))
		{
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}
	}

	{
#include "KdHD2DShader_VS_UnLit.shaderInc"
		// 頂点シェーダー作成
		if (FAILED(KdDirect3D::GetInstance().WorkDev()->CreateVertexShader(
			compiledBuffer,
			sizeof(compiledBuffer), 
			nullptr,
			&m_VS_UnLit)))
		{
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// ピクセルシェーダ
	//-------------------------------------
	{
#include "KdHD2DShader_PS_Lit.shaderInc"
		if (FAILED(KdDirect3D::GetInstance().WorkDev()->CreatePixelShader(
			compiledBuffer,
			sizeof(compiledBuffer),
			nullptr,
			&m_PS_Lit)))
		{
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	}

	{
#include "KdHD2DShader_PS_GenDepthMapFromLight.shaderInc"
		if (FAILED(KdDirect3D::GetInstance().WorkDev()->CreatePixelShader(
			compiledBuffer,
			sizeof(compiledBuffer),
			nullptr,
			&m_PS_GenDepthFromLight)))
		{
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	} 
	
	{
#include "KdHD2DShader_PS_UnLit.shaderInc"
		if (FAILED(KdDirect3D::GetInstance().WorkDev()->CreatePixelShader(
			compiledBuffer,
			sizeof(compiledBuffer), 
			nullptr,
			&m_PS_UnLit)))
		{
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	}
	//-------------------------------------
	// 定数バッファ作成
	//-------------------------------------
	m_cb0_Obj.Create();
	m_cb1_Mesh.Create();
	m_cb2_Material.Create();

	std::shared_ptr<KdTexture> ds = std::make_shared<KdTexture>();
	ds->CreateDepthStencil(1024, 1024);
	D3D11_VIEWPORT vp = {
		0.0f, 0.0f,
		static_cast<float>(ds->GetWidth()),
		static_cast<float>(ds->GetHeight()),
		0.0f, 1.0f
	};

	m_depthMapFromLightRTPack.CreateRenderTarget(1024, 1024, true, DXGI_FORMAT_R32_FLOAT);
	m_depthMapFromLightRTPack.ClearTexture(kRedColor);

	SetDissolveTexture(*KdAssets::GetInstance().m_textures.GetData("Asset/Textures/System/WhiteNoise.png"));

	return true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// シェーダー本体の解放
// 利用していたコンスタントバッファの解放
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::Release()
{
	KdSafeRelease(m_VS_Lit);
	KdSafeRelease(m_VS_GenDepthFromLight);
	KdSafeRelease(m_VS_UnLit);

	KdSafeRelease(m_inputLayout);
	
	KdSafeRelease(m_PS_Lit);
	KdSafeRelease(m_PS_GenDepthFromLight);
	KdSafeRelease(m_PS_UnLit);

	m_cb0_Obj.Release();
	m_cb1_Mesh.Release();
	m_cb2_Material.Release();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 描画用マテリアル情報の転送
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// それぞれのマテリアルの影響倍率値とテクスチャを設定
// BaseColor：基本色 / Emissive：自己発光色 / Metalic：金属性(テカテカ) / Roughness：粗さ(材質の色の反映度)
// テクスチャは法線マップ以外は未設定なら白1ピクセルのシステムテクスチャを指定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::WriteMaterial(const KdMaterial& _material, const Math::Vector4& _colRate, const Math::Vector3& _emiRate)
{
	//-----------------------
	// マテリアル情報を定数バッファへ書き込む
	//-----------------------
	m_cb2_Material.Work().BaseColor = _material.m_baseColorRate * _colRate;
	m_cb2_Material.Work().Emissive = _material.m_emissiveRate * _emiRate;
	m_cb2_Material.Work().Metallic = _material.m_metallicRate;
	m_cb2_Material.Work().Roughness = _material.m_roughnessRate;
	m_cb2_Material.Write();

	//-----------------------
	// テクスチャセット
	//-----------------------
	ID3D11ShaderResourceView* srvs[4]{};

	srvs[0] = _material.m_baseColorTex ? _material.m_baseColorTex->WorkSRView() : KdDirect3D::GetInstance().GetWhiteTex()->WorkSRView();
	srvs[1] = _material.m_metallicRoughnessTex ? _material.m_metallicRoughnessTex->WorkSRView() : KdDirect3D::GetInstance().GetWhiteTex()->WorkSRView();
	srvs[2] = _material.m_emissiveTex ? _material.m_emissiveTex->WorkSRView() : KdDirect3D::GetInstance().GetWhiteTex()->WorkSRView();
	srvs[3] = _material.m_normalTex ? _material.m_normalTex->WorkSRView() : KdDirect3D::GetInstance().GetNormalTex()->WorkSRView();

	// セット
	KdDirect3D::GetInstance().WorkDevContext()->PSSetShaderResources(0, _countof(srvs), srvs);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ポリゴンがどの方向に向いていても光の影響を正面からに受けるように頂点の法線を変換
// 2Dキャラクタを描画する時などは必要
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::ConvertNormalsFor2D(std::vector<KdPolygon::Vertex>& _target, const Math::Matrix& _mWorld)
{
	// 平行光の向き
	const Math::Vector3& dirLight_Dir = KdShaderManager::GetInstance().GetLightCB().DirLight_Dir;

	// どの角度を向いていても表面は常に光の方向を向いている状態：横向きの板ポリが暗くならない対策
	Math::Vector3 normal = Math::Vector3::TransformNormal(-dirLight_Dir, _mWorld.Invert());
	Math::Vector3 tangent = (normal != Math::Vector3::Up) ?
		normal.Cross(Math::Vector3::Up) : normal.Cross(Math::Vector3::Right);

	for (size_t i = 0; i < _target.size(); ++i)
	{
		_target[i].normal = normal;
		_target[i].tangent = tangent;
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// オブジェクト定数バッファを初期状態に戻す
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdHD2DShader::ResetCBObject()
{
	m_cb0_Obj.Work() = cbObject();
	m_cb0_Obj.Write();
	m_dirtyCBObj = false;
}