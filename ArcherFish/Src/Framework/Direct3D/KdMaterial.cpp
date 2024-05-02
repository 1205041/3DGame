#include "KdMaterial.h"

void KdMaterial::SetTextures(const std::shared_ptr<KdTexture>& _spBaseColTex, 
	const std::shared_ptr<KdTexture>& _spMtRfColTex,
	const std::shared_ptr<KdTexture>& _spEmiColTex, 
	const std::shared_ptr<KdTexture>& _spNmlColTex)
{
	m_baseColorTex = _spBaseColTex;
	m_metallicRoughnessTex = _spMtRfColTex;
	m_emissiveTex = _spEmiColTex;
	m_normalTex = _spNmlColTex;

	if (_spMtRfColTex)
	{
		m_metallicRate = 1.0f;
		m_roughnessRate = 1.0f;
	}
}

void KdMaterial::SetTextures(const std::string& _fileDir, const std::string& _baseColName,
	const std::string& _mtRfColName, const std::string& _emiColName,
	const std::string& _nmlColName)
{
	std::shared_ptr<KdTexture>	BaseColorTex = nullptr;
	std::shared_ptr<KdTexture>	MetallicRoughnessTex = nullptr;
	std::shared_ptr<KdTexture>	EmissiveTex = nullptr;
	std::shared_ptr<KdTexture>	NormalTex = nullptr;

	// 基本色テクスチャ
	if (!_baseColName.empty() && KdFileExistence(_fileDir + _baseColName)) { BaseColorTex = KdAssets::GetInstance().m_textures.GetData(_fileDir + _baseColName); }

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 金属性・粗さマップ
	if (!_mtRfColName.empty() && KdFileExistence(_fileDir + _mtRfColName)) { MetallicRoughnessTex = KdAssets::GetInstance().m_textures.GetData(_fileDir + _mtRfColName); }

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 自己発光・エミッシブマップ
	if (!_emiColName.empty() && KdFileExistence(_fileDir + _emiColName)) { EmissiveTex = KdAssets::GetInstance().m_textures.GetData(_fileDir + _emiColName); }

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 法線マップ
	if (!_nmlColName.empty() && KdFileExistence(_fileDir + _nmlColName)) { NormalTex = KdAssets::GetInstance().m_textures.GetData(_fileDir + _nmlColName); }

	SetTextures(BaseColorTex, MetallicRoughnessTex, EmissiveTex, NormalTex);
}