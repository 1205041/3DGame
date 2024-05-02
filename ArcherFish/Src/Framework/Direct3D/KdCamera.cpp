#include "KdCamera.h"

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* カメラ情報(ビュー・射影行列など)をシェーダへ転送 */
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// ・CPU側に用意してあるシェーダ情報格納用コンテナへデータをコピー
// ・コンテナをシェーダー(GPU)に送信する
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
void KdCamera::SetToShader() const
{
	// カメラの情報をGPUへ転送
	KdShaderManager::GetInstance().WriteCBCamera(m_mCam, m_mProj);

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	// 被写界深度（DepthOfField）加工に使うの情報も更新
	float nearClippingDistance = -(m_mProj._43 / m_mProj._33);
	float farClippingDistance = - (m_mProj._43 / (m_mProj._33 - 1));
	float viewRange = farClippingDistance - nearClippingDistance;

	// フォーカスを合わせる焦点距離をコピー
	KdShaderManager::GetInstance().m_postProcessShader.SetNearClippingDistance(nearClippingDistance);
	KdShaderManager::GetInstance().m_postProcessShader.SetFarClippingDistance(farClippingDistance);
	KdShaderManager::GetInstance().m_postProcessShader.SetFocusDistance((m_focusDistance - nearClippingDistance) / viewRange);
	KdShaderManager::GetInstance().m_postProcessShader.SetFocusRange(m_focusForeRange / viewRange, m_focusBackRange / viewRange);
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* 射影行列の設定：各種パラメータから射影行列を生成して保持する */
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// ・引数：(fov：視野角)
// 　　　　(maxRange：描画する最長距離)
// 　　　　(minRange：描画する最短距離)
// 　　　　(aspectRatio：画面の縦横幅の比率)
// ・視野角以外のパラメータはデフォルト引数が設定されているため、省略可能
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
void KdCamera::SetProjectionMatrix(float _fov, float _maxRange, float _minRange, float _aspectRatio)
{
	float aspect = _aspectRatio;

	// アスペクト比が不正だった場合
	if (aspect <= 0)
	{
		// 自動的にバックバッファからアスペクト比を求める
		if (KdDirect3D::GetInstance().GetBackBuffer()) { aspect = KdDirect3D::GetInstance().GetBackBuffer()->GetAspectRatio(); }
		// バックバッファが生成されてすらいな状況なら射影行列をセットしない
		else { return; }
	}

	m_mProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(_fov), aspect, _minRange, _maxRange);

	SetProjectionMatrix(m_mProj);
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* 被写界深度の情報を設定 */
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// ・カメラから見て焦点を当てる距離の設定
// ・ぼかさずに描画する焦点エリアを前後それぞれ別の距離に設定可能
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
void KdCamera::SetFocus(float _focusDist, float _focusForeRange, float _focusBackRange)
{
	m_focusDistance = _focusDist;
	m_focusForeRange = _focusForeRange;
	m_focusBackRange = _focusBackRange;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* クライアント座標（2D）から3Dワールド座標を求める用のレイ情報を生成 */
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// ・マウスポインタの2D位置にある3Dオブジェクトを選択する時などに使用する
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
void KdCamera::GenerateRayInfoFromClientPos(const POINT& _clientPos, Math::Vector3& _rayPos, Math::Vector3& _rayDir, float& _rayRange)
{
	// レイ判定の最遠座標
	Math::Vector3 farPos;

	// 2D座標を3Dワールド座標に変換する関数を使ってレイの発射座標と最遠座標を求める
	KdDirect3D::GetInstance().ClientToWorld(_clientPos, 0.0f, _rayPos, m_mCam, m_mProj);
	KdDirect3D::GetInstance().ClientToWorld(_clientPos, 1.0f, farPos, m_mCam, m_mProj);

	// 目的地 - 出発地点 でレイを飛ばす方向を求める
	_rayDir = farPos - _rayPos;

	// 2点間の距離を求めてレイの判定距離を求める
	_rayRange = _rayDir.Length();

	// 正規化（長さを1.0fに）して方向ベクトルに変換する
	_rayDir.Normalize();
}