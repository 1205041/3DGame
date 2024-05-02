#pragma once

//===========================================
// レンダーターゲット変更
//===========================================
struct KdRenderTargetPack
{
	KdRenderTargetPack() {}

	void CreateRenderTarget(int _width, int _height, bool _needDSV = false, DXGI_FORMAT _format = DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_VIEWPORT* _pVP = nullptr);
	void SetRenderTarget(std::shared_ptr<KdTexture> _RTT, std::shared_ptr<KdTexture> _DST = nullptr, D3D11_VIEWPORT* _pVP = nullptr);

	void SetViewPort(D3D11_VIEWPORT* _pVP);

	void ClearTexture(const Math::Color& _fillColor = kBlueColor);

	std::shared_ptr<KdTexture> m_RTTexture = nullptr;
	std::shared_ptr<KdTexture> m_ZBuffer = nullptr;
	D3D11_VIEWPORT m_viewPort = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
};

struct KdRenderTargetChanger
{
	~KdRenderTargetChanger() { Release(); }

	ID3D11RenderTargetView* m_saveRTV = nullptr;
	ID3D11DepthStencilView* m_saveDSV = nullptr;
	D3D11_VIEWPORT			m_saveVP = {};
	UINT					m_numVP = 1;
	bool					m_changeVP = false;

	bool Validate(ID3D11RenderTargetView* _pRTV);

	bool ChangeRenderTarget(ID3D11RenderTargetView* _pRTV, ID3D11DepthStencilView* _pDSV = nullptr, D3D11_VIEWPORT* _pVP = nullptr);
	bool ChangeRenderTarget(std::shared_ptr<KdTexture> _RTT, std::shared_ptr<KdTexture> _DST = nullptr, D3D11_VIEWPORT* _pVP = nullptr);
	bool ChangeRenderTarget(KdRenderTargetPack& _RTPack);

	void UndoRenderTarget();

	void Release();
};