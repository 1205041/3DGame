#include "Framework/KdFramework.h"

void KdDirect3D::CopyViewportInfo(Math::Viewport& _out) const
{
	UINT numVPs = 1;
	D3D11_VIEWPORT vp;
	m_pDeviceContext->RSGetViewports(&numVPs, &vp);
	_out = vp;
}

void KdDirect3D::ClientToWorld(const POINT& _screenPos, float _porjZ, Math::Vector3& _dst, const Math::Matrix& _mCam, const Math::Matrix& _mProj)
{
	_porjZ = std::clamp(_porjZ, 0.0f, 1.0f);

	// 各行列の逆行列を算出
	Math::Matrix invView = _mCam;
	Math::Matrix invPrj = _mProj.Invert();

	Math::Matrix mVP;
	mVP._11 = m_windowWidth * 0.5f;
	mVP._22 = -m_windowHeight * 0.5f;
	mVP._41 = m_windowWidth * 0.5f;
	mVP._42 = m_windowHeight * 0.5f;

	Math::Matrix invViewport = mVP.Invert();

	// 2D→3D変換用行列合成
	Math::Matrix convertMat = invViewport * invPrj * invView;

	Math::Vector3::Transform({ (float)_screenPos.x, (float)_screenPos.y, _porjZ }, convertMat, _dst);
}

void KdDirect3D::WorldToClient(const Math::Vector3& _srcWorld, POINT& _dst, const Math::Matrix& _mCam, const Math::Matrix& _mProj)
{
	Math::Matrix mVP;
	mVP._11 = m_windowWidth * 0.5f;
	mVP._22 = -m_windowHeight * 0.5f;
	mVP._41 = m_windowWidth * 0.5f;
	mVP._42 = m_windowHeight * 0.5f;

	Math::Matrix convertMat = _mCam.Invert() * _mProj * mVP;

	Math::Vector3 screenPos = Math::Vector3::Transform(_srcWorld, convertMat);

	_dst.x = static_cast<int>(screenPos.x);
	_dst.y = static_cast<int>(screenPos.y);
}

bool KdDirect3D::Init(HWND _hWnd, int _w, int _h, bool _deviceDebug, std::string& _errMsg)
{
	HRESULT hr;

	//=====================================================
	// ファクトリー作成(ビデオ グラフィックの設定の列挙や指定に使用されるオブジェクト)
	//=====================================================
	IDXGIFactory* factory = nullptr;
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&factory));
	if (FAILED(hr)) 
	{
		_errMsg = "ファクトリー作成失敗";

		Release();
		return false;
	}

	//=====================================================
	//デバイス生成(主にリソース作成時に使用するオブジェクト)
	//=====================================================
	UINT creationFlags = 0;

	// Direct3Dのデバッグを有効にする(すごく重いが細かいエラーがわかる)
	if (_deviceDebug) { creationFlags |= D3D11_CREATE_DEVICE_DEBUG; }

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,	// Direct3D 11.1  ShaderModel 5
		D3D_FEATURE_LEVEL_11_0,	// Direct3D 11    ShaderModel 5
		D3D_FEATURE_LEVEL_10_1,	// Direct3D 10.1  ShaderModel 4
		D3D_FEATURE_LEVEL_10_0,	// Direct3D 10.0  ShaderModel 4
		D3D_FEATURE_LEVEL_9_3,	// Direct3D 9.3   ShaderModel 3
		D3D_FEATURE_LEVEL_9_2,	// Direct3D 9.2   ShaderModel 3
		D3D_FEATURE_LEVEL_9_1,	// Direct3D 9.1   ShaderModel 3
	};

	// 使用GPU選択（グラフィック専用メモリの容量の多い物を優先
	IDXGIAdapter* pSelectAdapter = nullptr;
	SIZE_T maxVideoMemory = 0;
	for (UINT index = 0; 1; ++index)
	{
		IDXGIAdapter* adapter = nullptr;
		HRESULT ret = factory->EnumAdapters(index, &adapter);

		if (ret == DXGI_ERROR_NOT_FOUND) { break; }
		DXGI_ADAPTER_DESC desc = {};
		adapter->GetDesc(&desc);

		if (maxVideoMemory < desc.DedicatedVideoMemory)
		{
			maxVideoMemory = desc.DedicatedVideoMemory;
			pSelectAdapter = adapter;
		}
	}

	// デバイスとでデバイスコンテキストを作成
	D3D_FEATURE_LEVEL futureLevel;
	hr = D3D11CreateDevice(
		pSelectAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&m_pDevice,
		&futureLevel,
		&m_pDeviceContext);

	if FAILED(hr)
	{
		_errMsg = "Direct3D11デバイス作成失敗";

		Release();
		return false;
	}

	//=====================================================
	// スワップチェイン作成(フロントバッファに表示可能なバックバッファを持つもの)
	//=====================================================
	DXGI_SWAP_CHAIN_DESC DXGISwapChainDesc = {};		// スワップチェーンの設定データ
	DXGISwapChainDesc.BufferDesc.Width = _w;
	DXGISwapChainDesc.BufferDesc.Height = _h;
	DXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	DXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	DXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	DXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;// DXGI_MODE_SCALING_CENTERED;	// DXGI_MODE_SCALING_UNSPECIFIED
	DXGISwapChainDesc.SampleDesc.Count = 1;
	DXGISwapChainDesc.SampleDesc.Quality = 0;
	DXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	DXGISwapChainDesc.BufferCount = 2;
	DXGISwapChainDesc.OutputWindow = _hWnd;
	DXGISwapChainDesc.Windowed = TRUE;
	DXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	DXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (FAILED(factory->CreateSwapChain(m_pDevice, &DXGISwapChainDesc, &m_pGISwapChain))) {
		_errMsg = "スワップチェイン作成失敗";

		Release();
		return false;
	}
	factory->Release();

	// スワップチェインからバックバッファ取得
	ID3D11Texture2D* pBackBuffer;
	if (FAILED(m_pGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer))) {
		_errMsg = "バックバッファ取得失敗";

		Release();
		return false;
	}

	// バックバッファリソースからビューを作成
	m_backBuffer = std::make_shared<KdTexture>();
	if (m_backBuffer->Create(pBackBuffer) == false)
	{
		_errMsg = "バックバッファ作成失敗";
		Release();
		return false;
	}

	pBackBuffer->Release();

	/*
	// ALT+Enterでフルスクリーン不可にする
	{
		IDXGIDevice* pDXGIDevice;
		m_pDevice->QueryInterface<IDXGIDevice>(&pDXGIDevice);

		IDXGIAdapter* pDXGIAdapter;
		pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);

		IDXGIFactory* pIDXGIFactory;
		pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);

		pIDXGIFactory->MakeWindowAssociation(_hWnd, DXGI_MWA_NO_ALT_ENTER);

		pDXGIDevice->Release();
		pDXGIAdapter->Release();
		pIDXGIFactory->Release();
	}
	*/

	//=========================================================
	// Zバッファ作成
	//=========================================================
	{
		// バッファ作成
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		desc.Width = (UINT)_w;
		desc.Height = (UINT)_h;
		desc.CPUAccessFlags = 0;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		m_zBuffer = std::make_shared<KdTexture>();
		if (m_zBuffer->Create(desc) == false)
		{
			_errMsg = "Zバッファ作成失敗";
			Release();
			return false;
		}
	}

	//=========================================================
	// バックバッファ、Zバッファを描画ターゲットとしてデバイスコンテキストへセットする
	//=========================================================
	{
		ID3D11RenderTargetView* rtvs[] = { m_backBuffer->WorkRTView() };
		m_pDeviceContext->OMSetRenderTargets(1, rtvs, m_zBuffer->WorkDSView());
	}

	//=========================================================
	// ビューポートの設定
	//=========================================================
	{
		// ビューポートの設定
		D3D11_VIEWPORT vp;
		vp.Width = (float)_w;
		vp.Height = (float)_h;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_pDeviceContext->RSSetViewports(1, &vp);
	}

	//=========================================================
	// 描画ステートの初期設定
	//=========================================================

	// ブレンドステートセット
	ID3D11BlendState* bs = CreateBlendState(KdBlendMode::Alpha);
	m_pDeviceContext->OMSetBlendState(bs, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);
	bs->Release();

	// サンプラーステートセット
	ID3D11SamplerState* ss0 = CreateSamplerState(KdSamplerFilterMode::Anisotropic, 4, KdSamplerAddressingMode::Wrap, false);
	// 各シェーダへセット
	m_pDeviceContext->VSSetSamplers(0, 1, &ss0);
	m_pDeviceContext->PSSetSamplers(0, 1, &ss0);
	m_pDeviceContext->GSSetSamplers(0, 1, &ss0);
	m_pDeviceContext->CSSetSamplers(0, 1, &ss0);
	ss0->Release();

	ID3D11SamplerState* ss1 = CreateSamplerState(KdSamplerFilterMode::Anisotropic, 4, KdSamplerAddressingMode::Clamp, false);
	// 各シェーダへセット
	m_pDeviceContext->VSSetSamplers(1, 1, &ss1);
	m_pDeviceContext->PSSetSamplers(1, 1, &ss1);
	m_pDeviceContext->GSSetSamplers(1, 1, &ss1);
	m_pDeviceContext->CSSetSamplers(1, 1, &ss1);
	ss1->Release();

	// ラスタライザーステートセット
	ID3D11RasterizerState* rs = CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, true, false);
	m_pDeviceContext->RSSetState(rs);
	rs->Release();

	// 深度ステンシルステートセット
	ID3D11DepthStencilState* ds = CreateDepthStencilState(true, true);
	m_pDeviceContext->OMSetDepthStencilState(ds, 0);
	ds->Release();

	//=========================================================
	// 1x1の白テクスチャ作成
	//=========================================================
	{
		D3D11_SUBRESOURCE_DATA srd;
		auto whiteCol = kWhiteColor.RGBA();
		srd.pSysMem = &whiteCol;
		srd.SysMemPitch = 4;
		srd.SysMemSlicePitch = 0;

		m_texWhite = std::make_shared<KdTexture>();
		m_texWhite->Create(1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, 1, &srd);
	}

	//=========================================================
	// 1x1のZ向き法線マップ作成
	//=========================================================
	{
		D3D11_SUBRESOURCE_DATA srd;
		auto normalCol = kNormalColor.RGBA();
		srd.pSysMem = &normalCol;
		srd.SysMemPitch = 4;
		srd.SysMemSlicePitch = 0;

		m_texNormal = std::make_shared<KdTexture>();
		m_texNormal->Create(1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, 1, &srd);
	}

	// DrawVertices用頂点バッファを作成
	UINT bufferSize = 80;
	for (int i = 0; i < 10; i++)	// 
	{
		m_tempFixedVertexBuffer[i].Create(D3D11_BIND_VERTEX_BUFFER, bufferSize, D3D11_USAGE_DYNAMIC, nullptr);

		bufferSize *= 2;	// 容量を倍にしていく
	}

	m_windowWidth = _w;
	m_windowHeight = _h;

	return true;
}

void KdDirect3D::Release()
{
	for (auto&& n : m_tempFixedVertexBuffer) { n.Release(); }
	m_tempVertexBuffer.Release();

	m_backBuffer = nullptr;
	m_zBuffer = nullptr;
	m_texWhite = nullptr;
	m_texNormal = nullptr;

	KdSafeRelease(m_pGISwapChain);
	KdSafeRelease(m_pDeviceContext);
	KdSafeRelease(m_pDevice);
}

ID3D11DepthStencilState* KdDirect3D::CreateDepthStencilState(bool _zEnable, bool _zWriteEnable) const
{
	// Zバッファの動作設定
	D3D11_DEPTH_STENCIL_DESC desc = {};

	desc.DepthEnable = _zEnable;							// 深度テストを使用する
	desc.DepthWriteMask = _zWriteEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	desc.StencilEnable = FALSE;
	desc.StencilReadMask = 0;
	desc.StencilWriteMask = 0;

	// ステートオブジェクトを作成
	ID3D11DepthStencilState* state = nullptr;
	if (FAILED(m_pDevice->CreateDepthStencilState(&desc, &state)))return nullptr;

	return state;
}

ID3D11RasterizerState* KdDirect3D::CreateRasterizerState(D3D11_CULL_MODE _cullMode, D3D11_FILL_MODE _fillMode, bool _depthClipEnable, bool _scissorEnable) const
{
	// ラスタライズの動作設定
	D3D11_RASTERIZER_DESC desc = {};
	desc.FillMode = _fillMode;
	desc.CullMode = _cullMode;
	desc.FrontCounterClockwise = FALSE;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0;
	desc.SlopeScaledDepthBias = 0;
	desc.DepthClipEnable = _depthClipEnable;
	desc.ScissorEnable = _scissorEnable;
	desc.MultisampleEnable = FALSE;
	desc.AntialiasedLineEnable = FALSE;

	// ステートオブジェクト作成
	ID3D11RasterizerState* state = nullptr;
	if (FAILED(m_pDevice->CreateRasterizerState(&desc, &state)))return nullptr;

	return state;
}

ID3D11SamplerState* KdDirect3D::CreateSamplerState(KdSamplerFilterMode _filterType, UINT _maxAnisotropy, KdSamplerAddressingMode _addressingMode, bool _comparisonModel) const
{
	D3D11_SAMPLER_DESC desc = {};
	// フィルタ
	switch (_filterType) 
	{
		// Point
	case KdSamplerFilterMode::Point:
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
		// Linear
	case KdSamplerFilterMode::Linear:
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
		// Anisotropic
	case KdSamplerFilterMode::Anisotropic:
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		desc.MaxAnisotropy = _maxAnisotropy;
		break;
	}

	// アドレッシングモード
	switch (_addressingMode) 
	{
		// Wrap
	case KdSamplerAddressingMode::Wrap:
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
		// Clamp
	case KdSamplerAddressingMode::Clamp:
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	}

	// 比較モードサンプラ
	if (_comparisonModel) 
	{
		desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;	// とりあえず今回は線形フィルタ固定で
	}
	// 通常サンプラ
	else { desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; }

	// 
	desc.MipLODBias = 0;
	desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	// ステートオブジェクト作成
	ID3D11SamplerState* state = nullptr;
	if (FAILED(m_pDevice->CreateSamplerState(&desc, &state)))return nullptr;

	return state;
}

ID3D11BlendState* KdDirect3D::CreateBlendState(KdBlendMode _mode) const
{
	D3D11_BLEND_DESC desc = {};
	// 有効
	desc.RenderTarget[0].BlendEnable = TRUE;
	// 書き込みマスク　R,G,B,A全て出力する場合はALL
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	desc.IndependentBlendEnable = FALSE;
	desc.AlphaToCoverageEnable = FALSE;

	// ブレンドなし
	if (_mode == KdBlendMode::NoBlend)
	{
		// 色の合成方法
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		// アルファの合成方法
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	}
	// 半透明ブレンド
	else if (_mode == KdBlendMode::Alpha)
	{
		// 色の合成方法
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		// アルファの合成方法
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	}
	// 加算ブレンド
	else if (_mode == KdBlendMode::Add)
	{
		// 色の合成方法
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		// アルファの合成方法
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	}

	// ステートオブジェクト作成
	ID3D11BlendState* state = nullptr;
	if (FAILED(m_pDevice->CreateBlendState(&desc, &state)))return nullptr;

	return state;
}

void KdDirect3D::DrawVertices(D3D_PRIMITIVE_TOPOLOGY _topology, int _vertexCount, const void* _pVertexStream, UINT _stride)
{
	assert(m_pDeviceContext && "デバイスコンテキストが見つかりません");

	// プリミティブトポロジーをセット
	m_pDeviceContext->IASetPrimitiveTopology(_topology);

	// 全頂点の総バイトサイズ
	UINT totalSize = _vertexCount * _stride;

	// 最適な固定長バッファを検索
	KdBuffer* buffer = nullptr;
	for (auto&& n : m_tempFixedVertexBuffer)
	{
		if (totalSize < n.GetBufferSize())
		{
			buffer = &n;
			break;
		}
	}
	// 可変長のバッファを使用
	if (buffer == nullptr)
	{
		buffer = &m_tempVertexBuffer;

		// 頂点バッファのサイズが小さいときは、リサイズのため再作成する
		if (m_tempVertexBuffer.GetBufferSize() < totalSize) { m_tempVertexBuffer.Create(D3D11_BIND_VERTEX_BUFFER, totalSize, D3D11_USAGE_DYNAMIC, nullptr); }
	}

	//============================================================
	/* 単純なDISCARDでの書き込み。 */
	//  ・DISCARDは、新たなバッファを内部で作成し、前回のバッファは使い終わると無効にするものです。
	//  　つまり書き込む度に新規のバッファになる感じです。
	//  ・バッファのサイズが大きいと、その分新規のバッファを作成する時間がかかってしまいます。
	//  　これを改善したい場合は、「DISCARDとNO_OVERWRITEの組み合わせ技」の方法で行うほうがよいです。
	//============================================================

	// 全頂点をバッファに書き込み(DISCARD指定)
	buffer->WriteData(_pVertexStream, totalSize);

	// 頂点バッファーをデバイスへセット
	{
		UINT offset = 0;
		m_pDeviceContext->IASetVertexBuffers(0, 1, buffer->GetAddress(), &_stride, &offset);
	}

	// 描画
	m_pDeviceContext->Draw(_vertexCount, 0);
}

void KdDirect3D::ClearBackBuffer()
{
	// バックバッファクリア
	m_pDeviceContext->ClearRenderTargetView(m_backBuffer->WorkRTView(), m_backBafferClearColor);

	// Zバッファクリア
	m_pDeviceContext->ClearDepthStencilView(m_zBuffer->WorkDSView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}