#include "Framework/KdFramework.h"

#include "KdBuffer.h"

bool KdBuffer::Create(UINT _bindFlags, UINT _bufferSize, D3D11_USAGE _bufferUsage, const D3D11_SUBRESOURCE_DATA* _initData)
{
	Release();

	//----------------------------------
	// 定数バッファとして作成する場合はサイズチェックを行う
	// ※定数バッファは16バイトアライメント(16の倍数)にする必要がある
	//----------------------------------
	if (_bindFlags == D3D11_BIND_CONSTANT_BUFFER)
	{

		if (_bufferSize % 16 != 0)
		{
			assert(0 && "コンスタントバッファ作成のサイズは16の倍数バイトでなければならないよ!!(Buffer)");
			return false;
		}

		// サイズを16の倍数バイトにする
		// bufferSize = ((bufferSize + 16 - 1) / 16) * 16;
	}

	//--------------------------------
	// バッファ作成のための詳細データ
	//--------------------------------
	D3D11_BUFFER_DESC desc;
	desc.BindFlags			= _bindFlags;		// デバイスにバインドするときの種類(頂点バッファ、インデックスバッファ、定数バッファなど)
	desc.ByteWidth			= _bufferSize;		// 作成するバッファのバイトサイズ
	desc.MiscFlags			= 0;				// その他のフラグ
	desc.StructureByteStride= 0;				// 構造化バッファの場合、その構造体のサイズ

	desc.Usage				= _bufferUsage;		// 作成するバッファの使用法

	/* 動的ビデオメモリバッファ */
	// ・GPUからWrite× Read○
	// ・CPUからWrite○ Read×
	// ・頻繁に更新されるようなバッファはこれが効率良いが、
	// 　DEFAULTに比べたら少し速度は落ちる
	if (desc.Usage == D3D11_USAGE_DYNAMIC) { desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; }

	/* 静的ビデオメモリバッファ */
	// ・GPUからWrite○ Read○
	// ・CPUからWrite× Read×　(ただしUpdateSubresource()で更新は可能)
	// ・ビデオメモリーのバッファ　頻繁に更新するには向いていないが描画が高速
	else if (desc.Usage == D3D11_USAGE_DEFAULT) { desc.CPUAccessFlags = 0; }

	/* ステージングバッファ */
	// ・GPUからWrite× Read×
	// ・CPUからWrite○ Read○
	// ・Direct3Dへバインドは出来ないが、
	// 　DEFAULTやDYNAMICのバッファに対して読み書き転送が可能
	// (例)DEFAULTバッファの内容を取得したい！ 
	// 　　-> STAGINGバッファを作成し、DEFAULTバッファからコピーしてくる(CopyResource関数)。
	// 　　　　そしてSTAGINGバッファにアクセス(Map/Unmap)する。
	else if (desc.Usage == D3D11_USAGE_STAGING)
	{
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}
	
	//--------------------------------
	// バッファ作成
	//--------------------------------
	if (FAILED(KdDirect3D::GetInstance().WorkDev()->CreateBuffer(&desc, _initData, &m_pBuffer)))
	{
		assert(0 && "バッファ作成失敗(Buffer)");
		return false;
	}

	// 
	m_bufUsage = _bufferUsage;
	m_bufSize = _bufferSize;

	return true;
}

void KdBuffer::WriteData(const void* _pSrcData, UINT _size)
{
	// 動的バッファの場合
	if (m_bufUsage == D3D11_USAGE_DYNAMIC)
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		// 書き込み専用
		if (SUCCEEDED(KdDirect3D::GetInstance().WorkDevContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			memcpy_s(pData.pData, m_bufSize, _pSrcData, _size);

			KdDirect3D::GetInstance().WorkDevContext()->Unmap(m_pBuffer, 0);
		}
	}
	// 静的バッファの場合：バッファに書き込み
	else if (m_bufUsage == D3D11_USAGE_DEFAULT) { KdDirect3D::GetInstance().WorkDevContext()->UpdateSubresource(m_pBuffer, 0, 0, _pSrcData, 0, 0); }
	// ステージングバッファの場合
	else if (m_bufUsage == D3D11_USAGE_STAGING)
	{
		// 読み書き両方
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(KdDirect3D::GetInstance().WorkDevContext()->Map(m_pBuffer, 0, D3D11_MAP_READ_WRITE, 0, &pData)))
		{
			memcpy_s(pData.pData, m_bufSize, _pSrcData, _size);

			KdDirect3D::GetInstance().WorkDevContext()->Unmap(m_pBuffer, 0);
		}
	}

}

void KdBuffer::CopyFrom(const KdBuffer& _srcBuffer)
{
	if (m_pBuffer == nullptr)return;
	if (_srcBuffer.GetBuffer() == nullptr)return;

	KdDirect3D::GetInstance().WorkDevContext()->CopyResource(m_pBuffer, _srcBuffer.GetBuffer());
}
