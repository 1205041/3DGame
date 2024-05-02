#include "KdRenderTargetChange.h"

//===========================================
// レンダーターゲットのパッケージ　RTView-DSView-ViewPort
//===========================================

void KdRenderTargetPack::CreateRenderTarget(int _width, int _height, bool _needDSV, DXGI_FORMAT _format, D3D11_VIEWPORT* _pVP)
{
	m_RTTexture = std::make_shared<KdTexture>();
	m_RTTexture->CreateRenderTarget(_width, _height, _format);

	if (_needDSV)
	{
		m_ZBuffer = std::make_shared<KdTexture>();
		m_ZBuffer->CreateDepthStencil(_width, _height);
	}

	SetViewPort(_pVP);
}

void KdRenderTargetPack::SetRenderTarget(std::shared_ptr<KdTexture> _RTT, std::shared_ptr<KdTexture> _DST, D3D11_VIEWPORT* _pVP)
{
	m_RTTexture = _RTT;
	m_ZBuffer = _DST;

	SetViewPort(_pVP);
}

void KdRenderTargetPack::SetViewPort(D3D11_VIEWPORT* _pVP)
{
	if (_pVP) { m_viewPort = *_pVP; }
	else
	{
		if (!m_RTTexture) { return; }

		// レンダーターゲットテクスチャから生成
		m_viewPort =
		{
			0.0f,
			0.0f,
			static_cast<float>(m_RTTexture->GetWidth()),
			static_cast<float>(m_RTTexture->GetHeight()),
			0.0f,
			1.0f
		};
	}
}

void KdRenderTargetPack::ClearTexture(const Math::Color& _fillColor)
{
	ID3D11DeviceContext* DevCon = KdDirect3D::GetInstance().WorkDevContext();

	// テクスチャの描画クリア
	if (m_RTTexture) { DevCon->ClearRenderTargetView(m_RTTexture->WorkRTView(), _fillColor); }

	if (m_ZBuffer)
	{
		DevCon->ClearDepthStencilView(m_ZBuffer->WorkDSView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}


//===========================================
// レンダーターゲット変更クラス
//===========================================
bool KdRenderTargetChanger::Validate(ID3D11RenderTargetView* _pRTV)
{
	if (!_pRTV)
	{
		assert(0 && "変更先のRenderTargetがありません");

		return false;
	}

	if (m_saveRTV)
	{
		assert(0 && "既にRenderTargetを変更済みです");

		return false;
	}

	return true;
}

bool KdRenderTargetChanger::ChangeRenderTarget(ID3D11RenderTargetView* _pRTV, ID3D11DepthStencilView* _pDSV, D3D11_VIEWPORT* _pVP)
{
	if (!Validate(_pRTV)) { return false; }

	ID3D11DeviceContext* DevCon = KdDirect3D::GetInstance().WorkDevContext();

	// 情報保存
	if (!m_saveRTV) { DevCon->OMGetRenderTargets(1, &m_saveRTV, &m_saveDSV); }

	// レンダーターゲット切替 ----- ----- ----- ----- -----
	DevCon->OMSetRenderTargets(1, &_pRTV, _pDSV);

	if (_pVP)
	{
		// 情報保存
		DevCon->RSGetViewports(&m_numVP, &m_saveVP);
		DevCon->RSSetViewports(1, _pVP);
		m_changeVP = true;
	}

	return true;
}

bool KdRenderTargetChanger::ChangeRenderTarget(std::shared_ptr<KdTexture> _RTT, std::shared_ptr<KdTexture> _DST, D3D11_VIEWPORT* _pVP)
{
	if (!_RTT) { return false; }

	ID3D11DepthStencilView* pDSV = nullptr;

	if (_DST) { pDSV = _DST->WorkDSView(); }

	return ChangeRenderTarget(_RTT->WorkRTView(), pDSV, _pVP);
}

bool KdRenderTargetChanger::ChangeRenderTarget(KdRenderTargetPack& _RTPack)
{
	return ChangeRenderTarget(_RTPack.m_RTTexture, _RTPack.m_ZBuffer, &_RTPack.m_viewPort);
}

void KdRenderTargetChanger::UndoRenderTarget()
{
	// 復帰すべきレンダーターゲットが存在しない
	if (!m_saveRTV) { return; }

	KdDirect3D::GetInstance().WorkDevContext()->OMSetRenderTargets(1, &m_saveRTV, m_saveDSV);

	if (m_changeVP) { KdDirect3D::GetInstance().WorkDevContext()->RSSetViewports(1, &m_saveVP); }

	KdSafeRelease(m_saveRTV);
	KdSafeRelease(m_saveDSV);

	m_changeVP = false;
}

void KdRenderTargetChanger::Release()
{
	KdSafeRelease(m_saveRTV);
	KdSafeRelease(m_saveDSV);
}
