#include "KdAmbientController.h"

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// シェーダーマネージャで設定したシェーダーの初期値を取得してくる
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::Init()
{
	// 光源のパラメータ
	{
		const KdShaderManager::cbLight& cbLight = KdShaderManager::GetInstance().GetLightCB();

		m_parameter.m_ambientLightColor = cbLight.AmbientLight;

		m_parameter.m_directionalLightDir = cbLight.DirLight_Dir;
		m_parameter.m_directionalLightDir.Normalize();

		m_parameter.m_directionalLightColor = cbLight.DirLight_Color;
	}

	// フォグのパラメータ
	{
		const KdShaderManager::cbFog& cbFog = KdShaderManager::GetInstance().GetFogCB();

		m_parameter.m_distanceFogColor = cbFog.DistanceFogColor;
		m_parameter.m_distanceFogDensity = cbFog.DistanceFogDensity;
		
		m_parameter.m_heightFogColor = cbFog.HeightFogColor;
		m_parameter.m_heightFogTopValue= cbFog.HeightFogTopValue;
		m_parameter.m_heightFogBottomValue = cbFog.HeightFogBottomValue;
		m_parameter.m_heightFogBeginDistance = cbFog.HeightFogBeginDistance;
	}

	// 影用深度バッファ生成情報の初期化
	SetDirLightShadowArea({ 25.0f, 25.0f }, 30.0f);

	// 光のパラメータの反映
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	WriteLightParams();

	// フォグのパラメータの反映
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	WriteFogParams();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 点光源は毎フレーム登録する形なので0クリアする
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::PreUpdate()
{
	m_pointLights.clear();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 描画の直前の更新が終わったタイミングで実施
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::PreDraw()
{
	// 光のパラメータの反映
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	WriteLightParams();

	// フォグのパラメータの反映
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	WriteFogParams();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ポイントライトの追加
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::AddPointLight(const Math::Vector3& _Color, float _Radius, const Math::Vector3& _Pos, bool _IsBright)
{
	m_pointLights.push_back(PointLight(_Color, _Radius, _Pos, _IsBright));
}

void KdAmbientController::AddPointLight(const PointLight& _pointLight)
{
	m_pointLights.push_back(_pointLight);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 平行光影の生成エリアの設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetDirLightShadowArea(const Math::Vector2& _lightingArea, float _dirLightHeight)
{
	m_shadowProj = DirectX::XMMatrixOrthographicLH(_lightingArea.x, _lightingArea.x, 0, _dirLightHeight * 2.0f);

	m_dirLightHeight = _dirLightHeight;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 平行光の設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetDirLight(const Math::Vector3& _dir, const Math::Vector3& _col)
{
	m_parameter.m_directionalLightDir = _dir;
	m_parameter.m_directionalLightDir.Normalize();

	m_parameter.m_directionalLightColor = _col;

	m_dirtyLightDir = true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 環境光の設定
// アルファ値の変更でデフォルトの暗さを指定できる。0にした場合は点光源の周囲以外は真っ暗
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetAmbientLight(const Math::Vector4& _col)
{
	m_parameter.m_ambientLightColor = _col;

	m_dirtyLightAmb = true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// フォグの有効/無効をそれぞれ切り替える
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetFogEnable(bool _distance, bool _height)
{
	KdShaderManager::GetInstance().WriteCBFogEnable(_distance, _height);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 距離フォグの設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetDistanceFog(const Math::Vector3& _col, float _density)
{
	m_parameter.m_distanceFogColor = _col;

	m_parameter.m_distanceFogDensity = _density;

	m_dirtyFogDist = true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 高さフォグの設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetheightFog(const Math::Vector3& _col, float _topValue, float _bottomValue, float _distance)
{
	m_parameter.m_heightFogColor = _col;

	m_parameter.m_heightFogTopValue = _topValue;

	m_parameter.m_heightFogBottomValue = _bottomValue;
	
	m_parameter.m_heightFogBeginDistance = _distance;

	m_dirtyFogHeight = true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 光関連パラメータの書き込み
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::WriteLightParams()
{
	// 環境光
	if (m_dirtyLightAmb)
	{
		KdShaderManager::GetInstance().WriteCBAmbientLight(m_parameter.m_ambientLightColor);

		m_dirtyLightAmb = false;
	}

	// 平行光
	if (m_dirtyLightDir)
	{
		KdShaderManager::GetInstance().WriteCBDirectionalLight(
			m_parameter.m_directionalLightDir, m_parameter.m_directionalLightColor);

		m_dirtyLightDir = false;
	}

	// 点光源
	if (m_pointLights.size())
	{
		KdShaderManager::GetInstance().WriteCBPointLight(m_pointLights);
	}

	// 影描画エリアの更新
	KdShaderManager::GetInstance().WriteCBShadowArea(m_shadowProj, m_dirLightHeight);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// フォグ関連パラメータの書き込み
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::WriteFogParams()
{
	// 距離フォグ
	if (m_dirtyFogDist)
	{
		KdShaderManager::GetInstance().WriteCBDistanceFog(m_parameter.m_distanceFogColor,
			m_parameter.m_distanceFogDensity);

		m_dirtyFogDist = false;
	}

	// 高さフォグ
	if (m_dirtyFogHeight)
	{
		KdShaderManager::GetInstance().WriteCBHeightFog(m_parameter.m_heightFogColor, m_parameter.m_heightFogTopValue, m_parameter.m_heightFogBottomValue, m_parameter.m_heightFogBeginDistance);

		m_dirtyFogHeight = false;
	}
}
