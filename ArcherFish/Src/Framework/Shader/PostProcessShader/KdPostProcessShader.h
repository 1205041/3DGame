#pragma once

class KdPostProcessShader
{
public:
	KdPostProcessShader() {}
	~KdPostProcessShader() { Release(); }

	void SetNearClippingDistance(float _distance) { m_cb0_DoFInfo.Work().NearClippingDistance = _distance; }
	void SetFarClippingDistance(float _distance) { m_cb0_DoFInfo.Work().FarClippingDistance = _distance; }
	void SetFocusDistance(float _distance) { m_cb0_DoFInfo.Work().FocusDistance = _distance; }
	void SetFocusRange(float _fore, float _back) { m_cb0_DoFInfo.Work().FocusForeRange = _fore; m_cb0_DoFInfo.Work().FocusBackRange = _back; }

	void SetBrightThreshold(float _threshold) { m_cb0_BrightInfo.Work().Threshold = _threshold; }

	struct Vertex
	{
		Math::Vector3 Pos;
		Math::Vector2 UV;
	};

	bool Init();

	void Release();

	void PreDraw();

	void BeginBright();
	void EndBright();

	void PostEffectProcess();

	void GenerateBlurTexture(std::shared_ptr<KdTexture>& _spSrcTex, std::shared_ptr<KdTexture>& _spDstTex, D3D11_VIEWPORT& _VP, int _blurRadius);

private:

	void BlurProcess();
	void LightBloomProcess();
	void DepthOfFieldProcess();

	void CreateBlurOffsetList(std::vector<Math::Vector3>& _dstInfo, const std::shared_ptr<KdTexture>& _spSrcTex, int _samplingSize, const Math::Vector2& _dir);

	void DrawTexture(std::shared_ptr<KdTexture>* _spSrcTex, int _srcTexSize, std::shared_ptr<KdTexture> _spDstTex, D3D11_VIEWPORT* _pVP);

	void SetBlurInfo(const std::shared_ptr<KdTexture>& _spSrcTex, int _samplingSize, const Math::Vector2& _dir);
	void SetBlurInfo(const std::vector<Math::Vector3>& _srcInfo);

	void SetBlurToDevice();
	void SetDoFToDevice();
	void SetBrightToDevice();

	ID3D11VertexShader* m_VS = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;

	ID3D11PixelShader* m_PS_Blur = nullptr;
	ID3D11PixelShader* m_PS_DoF = nullptr;
	ID3D11PixelShader* m_PS_Bright = nullptr;

	static const int kBlurSamplingRadius = 8;
	static const int kLightBloomSamplingRadius = 4;

	static const int kMaxSampling = 31;
	struct cbBlur
	{
		Math::Vector4 Info[kMaxSampling];
	
		int SamplingNum = 0;
		int _blank[3] = { 0, 0 ,0 };
	};
	KdConstantBuffer<cbBlur>	m_cb0_BlurInfo;

	struct cbDepthOfField
	{
		float NearClippingDistance = 0.0f;
		float FarClippingDistance = 1000.0f;

		float FocusDistance = 0.0f;
		float FocusForeRange = 0.0f;
		float FocusBackRange = 1000.0f;
		int   _blank[3] = { 0, 0, 0 };
	};
	KdConstantBuffer<cbDepthOfField>	m_cb0_DoFInfo;

	struct cbBrightFilter
	{
		float Threshold = 0.0f;
		int _blank[3] = { 0, 0, 0 };
	};
	KdConstantBuffer<cbBrightFilter>	m_cb0_BrightInfo;

	KdRenderTargetPack	m_postEffectRTPack;

	KdRenderTargetPack	m_blurRTPack;
	KdRenderTargetPack	m_strongBlurRTPack;

	KdRenderTargetPack	m_depthOfFieldRTPack;

	KdRenderTargetPack	m_brightEffectRTPack;
	static const int	kLightBloomNum = 4;
	KdRenderTargetPack	m_lightBloomRTPack[kLightBloomNum];

	KdRenderTargetChanger m_postEffectRTChanger;
	KdRenderTargetChanger m_brightRTChanger;

	Vertex m_screenVert[4];
};