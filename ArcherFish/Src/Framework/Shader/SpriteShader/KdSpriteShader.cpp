#include "Framework/KdFramework.h"

#include "KdSpriteShader.h"

bool KdSpriteShader::Init()
{
	Release();

	//-------------------------------------
	// 頂点シェーダ
	//-------------------------------------
	{
// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdSpriteShader_VS.shaderInc"
		// 頂点シェーダー作成
		if (FAILED(KdDirect3D::GetInstance().WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS)))
		{
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}

		// １頂点の詳細な情報
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// 頂点インプットレイアウト作成
		if (FAILED(KdDirect3D::GetInstance().WorkDev()->CreateInputLayout(
				&layout[0],
				layout.size(),
				compiledBuffer,
				sizeof(compiledBuffer),
				&m_VLayout)))
		{
			assert(0 && "CreateInputLayout失敗");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// ピクセルシェーダ
	//-------------------------------------
	{
// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdSpriteShader_PS.shaderInc"
		if (FAILED(KdDirect3D::GetInstance().WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS))) 
		{
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// 定数バッファ作成
	//-------------------------------------
	m_cb0.Create();
	m_cb1.Create();

	return true;
}

void KdSpriteShader::Release()
{
	KdSafeRelease(m_VS);
	KdSafeRelease(m_PS);
	KdSafeRelease(m_VLayout);
	m_cb0.Release();
	m_cb1.Release();
}

void KdSpriteShader::Begin(bool _linear, bool _disableZBuffer)
{
	// 既にBeginしている
	if (m_isBegin)return;
	m_isBegin = true;

	//---------------------------------------
	// 2D用正射影行列作成
	//---------------------------------------
	UINT pNumVierports = 1;
	D3D11_VIEWPORT vp;
	KdDirect3D::GetInstance().WorkDevContext()->RSGetViewports(&pNumVierports, &vp);
	m_mProj2D = DirectX::XMMatrixOrthographicLH(vp.Width, vp.Height, 0, 1);

	// 定数バッファ書き込み
	m_cb1.Work().mProj = m_mProj2D;
	m_cb1.Write();

	//---------------------------------------
	// 使用するステートをセット
	//---------------------------------------
	// Z判定、Z書き込み無効のステートをセット
	if (_disableZBuffer) { KdShaderManager::GetInstance().ChangeDepthStencilState(KdDepthStencilState::ZDisable); }

	// Samplerステートをセット
	if (_linear) { KdShaderManager::GetInstance().ChangeSamplerState(KdSamplerState::Linear_Clamp); }
	else { KdShaderManager::GetInstance().ChangeSamplerState(KdSamplerState::Point_Clamp); }

	// Rasterizerステートをセット
	KdShaderManager::GetInstance().ChangeRasterizerState(KdRasterizerState::CullNone);

	//---------------------------------------
	// シェーダ
	//---------------------------------------
	// シェーダをセット
	KdDirect3D::GetInstance().WorkDevContext()->VSSetShader(m_VS, 0, 0);
	KdDirect3D::GetInstance().WorkDevContext()->PSSetShader(m_PS, 0, 0);

	// 頂点レイアウトセット
	KdDirect3D::GetInstance().WorkDevContext()->IASetInputLayout(m_VLayout);

	// 定数バッファセット
	KdDirect3D::GetInstance().WorkDevContext()->VSSetConstantBuffers(0, 1, m_cb0.GetAddress());
	KdDirect3D::GetInstance().WorkDevContext()->PSSetConstantBuffers(0, 1, m_cb0.GetAddress());

	KdDirect3D::GetInstance().WorkDevContext()->VSSetConstantBuffers(1, 1, m_cb1.GetAddress());
	KdDirect3D::GetInstance().WorkDevContext()->PSSetConstantBuffers(1, 1, m_cb1.GetAddress());
}

void KdSpriteShader::End()
{
	if (!m_isBegin) { return; }
	m_isBegin = false;

	//---------------------------------------
	// 記憶してたステートに戻す
	//---------------------------------------
	KdShaderManager::GetInstance().UndoDepthStencilState();
	KdShaderManager::GetInstance().UndoSamplerState();
	KdShaderManager::GetInstance().UndoRasterizerState();
}

void KdSpriteShader::DrawTex(const KdTexture* _tex, int _x, int _y, int _w, int _h, const Math::Rectangle* _srcRect, const Math::Color* _color, const Math::Vector2& _pivot)
{
	if (_tex == nullptr)return;

	// もし開始していない場合は開始する(最後にEnd())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();

	// テクスチャ(ShaderResourceView)セット
	KdDirect3D::GetInstance().WorkDevContext()->PSSetShaderResources(0, 1, _tex->WorkSRViewAddress());

	// 色
	if (_color) { m_cb0.Work().Color = *_color; }
	m_cb0.Write();

	// UV
	Math::Vector2 uvMin = { 0.0f, 0.0f };
	Math::Vector2 uvMax = { 1.0f, 1.0f };
	if (_srcRect)
	{
		uvMin.x = _srcRect->x / (float)_tex->GetInfo().Width;
		uvMin.y = _srcRect->y / (float)_tex->GetInfo().Height;

		uvMax.x = (_srcRect->x + _srcRect->width) / (float)_tex->GetInfo().Width;
		uvMax.y = (_srcRect->y + _srcRect->height) / (float)_tex->GetInfo().Height;
	}

	// 頂点作成
	float x1 = (float)_x;
	float y1 = (float)_y;
	float x2 = (float)(_x + _w);
	float y2 = (float)(_y + _h);

	// 基準点(Pivot)ぶんずらす
	x1 -= _pivot.x * _w;
	x2 -= _pivot.x * _w;
	y1 -= _pivot.y * _h;
	y2 -= _pivot.y * _h;

	Vertex vertex[] = {
		{ {x1, y1, 0},	{uvMin.x, uvMax.y} },
		{ {x1, y2, 0},	{uvMin.x, uvMin.y} },
		{ {x2, y1, 0},	{uvMax.x, uvMax.y} },
		{ {x2, y2, 0},	{uvMax.x, uvMin.y} }

	};

	// 描画
	KdDirect3D::GetInstance().DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, vertex, sizeof(Vertex));

	// セットしたテクスチャを解除しておく
	ID3D11ShaderResourceView* srv = nullptr;
	KdDirect3D::GetInstance().WorkDevContext()->PSSetShaderResources(0, 1, &srv);

	// この関数でBeginした場合は、Endしておく
	if (!bBgn)End();
}

void KdSpriteShader::DrawPoint(int _x, int _y, const Math::Color* _color)
{
	// もし開始していない場合は開始する(最後にEnd())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();

	// 白テクスチャ
	KdDirect3D::GetInstance().WorkDevContext()->PSSetShaderResources(0, 1, KdDirect3D::GetInstance().GetWhiteTex()->WorkSRViewAddress());

	// 色
	if (_color) { m_cb0.Work().Color = *_color; }
	m_cb0.Write();

	// 描画
	Vertex vertex[] = { { {(float)_x, (float)_y, 0},	{0, 0} }, };
	KdDirect3D::GetInstance().DrawVertices(D3D_PRIMITIVE_TOPOLOGY_POINTLIST, 1, vertex, sizeof(Vertex));

	// この関数でBeginした場合は、Endしておく
	if (!bBgn)End();
}

void KdSpriteShader::DrawLine(int _x1, int _y1, int _x2, int _y2, const Math::Color* _color)
{
	// もし開始していない場合は開始する(最後にEnd())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();

	// 白テクスチャ
	KdDirect3D::GetInstance().WorkDevContext()->PSSetShaderResources(0, 1, KdDirect3D::GetInstance().GetWhiteTex()->WorkSRViewAddress());

	// 色
	if (_color) { m_cb0.Work().Color = *_color; }
	m_cb0.Write();

	// 描画
	Vertex vertex[] = {
		{ {(float)_x1, (float)_y1, 0},	{0, 0} },
		{ {(float)_x2, (float)_y2, 0},	{1, 0} },
	};
	KdDirect3D::GetInstance().DrawVertices(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, 2, vertex, sizeof(Vertex));

	// この関数でBeginした場合は、Endしておく
	if (!bBgn)End();
}

void KdSpriteShader::DrawTriangle(int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, const Math::Color* _color, bool _fill)
{
	// もし開始していない場合は開始する(最後にEnd())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();

	// 白テクスチャ
	KdDirect3D::GetInstance().WorkDevContext()->PSSetShaderResources(0, 1, KdDirect3D::GetInstance().GetWhiteTex()->WorkSRViewAddress());

	// 色
	if (_color) { m_cb0.Work().Color = *_color; }
	m_cb0.Write();


	// 描画
	Vertex vertex[] = {
		{ {(float)_x1, (float)_y1, 0},	{0, 0} },
		{ {(float)_x2, (float)_y2, 0},	{1, 0} },
		{ {(float)_x3, (float)_y3, 0},	{0, 0} },
		{ {(float)_x1, (float)_y1, 0},	{1, 0} },
	};
	KdDirect3D::GetInstance().DrawVertices(
		_fill ? D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP : D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
		4, vertex, sizeof(Vertex));


	// この関数でBeginした場合は、Endしておく
	if (!bBgn)End();
}

void KdSpriteShader::DrawCircle(int _x, int _y, int _radius, const Math::Color* _color, bool _fill)
{
	if (_radius <= 0)return;

	// もし開始していない場合は開始する(最後にEnd())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();

	// 白テクスチャ
	KdDirect3D::GetInstance().WorkDevContext()->PSSetShaderResources(0, 1, KdDirect3D::GetInstance().GetWhiteTex()->WorkSRViewAddress());

	// 色
	if (_color) { m_cb0.Work().Color = *_color; }
	m_cb0.Write();

	// 頂点
	if (_fill)
	{
		int faceNum = _radius + 1;
		if (faceNum > 300)faceNum = 300;
		std::vector<Vertex> vertex(faceNum * 3);		// 半径により頂点数を調整

		// 描画
		for (int i = 0; i < faceNum; i++)
		{
			int idx = i * 3;
			vertex[idx].Pos.x = (float)_x;
			vertex[idx].Pos.y = (float)_y;

			vertex[idx+1].Pos.x = _x + cos(DirectX::XMConvertToRadians(i * (360.0f / (faceNum - 1)))) * (float)_radius;
			vertex[idx+1].Pos.y = _y + sin(DirectX::XMConvertToRadians(i * (360.0f / (faceNum - 1)))) * (float)_radius;
			vertex[idx+1].Pos.z = 0;

			vertex[idx+2].Pos.x = _x + cos(DirectX::XMConvertToRadians((i+1) * (360.0f / (faceNum - 1)))) * (float)_radius;
			vertex[idx+2].Pos.y = _y + sin(DirectX::XMConvertToRadians((i+1) * (360.0f / (faceNum - 1)))) * (float)_radius;
			vertex[idx+2].Pos.z = 0;
		}

		KdDirect3D::GetInstance().DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)vertex.size(), &vertex[0], sizeof(Vertex));
	}
	else
	{
		int numVertex = _radius + 1;
		if (numVertex > 300)numVertex = 300;
		std::vector<Vertex> vertex(numVertex);		// 半径により頂点数を調整

		// 描画
		for (int i = 0; i < numVertex; i++)
		{
			vertex[i].Pos.x = _x + cos(DirectX::XMConvertToRadians(i * (360.0f / (numVertex - 1)))) * (float)_radius;
			vertex[i].Pos.y = _y + sin(DirectX::XMConvertToRadians(i * (360.0f / (numVertex - 1)))) * (float)_radius;
			vertex[i].Pos.z = 0;
		}

		KdDirect3D::GetInstance().DrawVertices(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, numVertex, &vertex[0], sizeof(Vertex));
	}

	// この関数でBeginした場合は、Endしておく
	if (!bBgn)End();
}

void KdSpriteShader::DrawBox(int _x, int _y, int _extentX, int _extentY, const Math::Color* _color, bool _fill)
{
	// もし開始していない場合は開始する(最後にEnd())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();

	// 白テクスチャ
	KdDirect3D::GetInstance().WorkDevContext()->PSSetShaderResources(0, 1, KdDirect3D::GetInstance().GetWhiteTex()->WorkSRViewAddress());

	// 色
	if (_color) { m_cb0.Work().Color = *_color; }
	m_cb0.Write();

	Math::Vector2 p1 = { (float)_x - _extentX, (float)_y - _extentY };
	Math::Vector2 p2 = { (float)_x - _extentX, (float)_y + _extentY };
	Math::Vector2 p3 = { (float)_x + _extentX, (float)_y + _extentY };
	Math::Vector2 p4 = { (float)_x + _extentX, (float)_y - _extentY };

	// 描画
	if (_fill)
	{
		Vertex vertex[] = {
			{ p1, {0,0}},
			{ p2, {0,0}},
			{ p4, {0,0}},
			{ p3, {0,0}}
		};

		KdDirect3D::GetInstance().DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, vertex, sizeof(Vertex));
	}
	else
	{
		Vertex vertex[] = {
			{ p1, {0,0}},
			{ p2, {0,0}},
			{ p3, {0,0}},
			{ p4, {0,0}},
			{ p1, {0,0}}
		};

		KdDirect3D::GetInstance().DrawVertices(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, 5, vertex, sizeof(Vertex));
	}

	// この関数でBeginした場合は、Endしておく
	if (!bBgn)End();
}

// 切り抜き範囲を設定する
// ・rect … 範囲
void KdSpriteShader::SetScissorRect(const Math::Rectangle& _rect)
{
	// ラスタライザステート作成・セット
	ID3D11RasterizerState* rs = KdDirect3D::GetInstance().CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, true, true);
	KdDirect3D::GetInstance().WorkDevContext()->RSSetState(rs);
	rs->Release();

	D3D11_RECT rc{};
	rc.left = _rect.x;
	rc.top = _rect.y;
	rc.right = _rect.x + _rect.width;
	rc.bottom = _rect.y + _rect.height;
	KdDirect3D::GetInstance().WorkDevContext()->RSSetScissorRects(1, &rc);
}

