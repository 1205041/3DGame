#include "Framework/KdFramework.h"

#include "KdUtility.h"

// viewから画像情報を取得する
void KdGetTextureInfo(ID3D11View* _view, D3D11_TEXTURE2D_DESC& _outDesc)
{
	_outDesc = {};

	ID3D11Resource* res;
	_view->GetResource(&res);

	ID3D11Texture2D* tex2D;
	if (SUCCEEDED(res->QueryInterface<ID3D11Texture2D>(&tex2D)))
	{
		tex2D->GetDesc(&_outDesc);
		tex2D->Release();
	}
	res->Release();
}

bool ConvertRectToUV(const KdTexture* _srcTex, const Math::Rectangle& _src, Math::Vector2& _uvMin, Math::Vector2& _uvMax)
{
	if (!_srcTex) { return false; }

	_uvMin.x = _src.x / (float)_srcTex->GetInfo().Width;
	_uvMin.y = _src.y / (float)_srcTex->GetInfo().Height;

	_uvMax.x = ( _src.width  / (float)_srcTex->GetInfo().Width) + _uvMin.x;
	_uvMax.y = ( _src.height / (float)_srcTex->GetInfo().Height) + _uvMin.y;

	return true;
}