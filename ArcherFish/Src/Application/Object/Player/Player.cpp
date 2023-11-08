#include "Player.h"

#include "../Camera/TPS/TPS.h"

// 更新関数
void Player::Update()
{
	m_moveVec = Math::Vector3::Zero;

	// カメラ情報
	std::shared_ptr<CameraBase> spCamera = m_wpCamera.lock();
	if (spCamera) { camRotMat = spCamera->GetRotationYMatrix(); }

	if (GetAsyncKeyState('W') & 0x8000)
	{
		// 前方
		Math::Vector3 v = Math::Vector3::TransformNormal
			(Math::Vector3::Backward, camRotMat);
		m_moveVec += v;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		// 下方
		Math::Vector3 v = Math::Vector3::TransformNormal
			(Math::Vector3::Down, camRotMat);
		m_moveVec += v;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		// 上方
		Math::Vector3 v = Math::Vector3::TransformNormal
			(Math::Vector3::Up, camRotMat);
		m_moveVec += v;
	}

	// 正規化
	m_moveVec.Normalize();

	// 移動
	m_pos += m_moveVec * m_moveSpd;
}

// 更新後更新関数
void Player::PostUpdate()
{
	// 拡縮行列
	m_scaleMat = Math::Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	// 回転行列
	m_rotMat = Math::Matrix::CreateRotationY
	(DirectX::XMConvertToRadians(m_ang));

	// 座標行列
	m_transMat = Math::Matrix::CreateTranslation(m_pos);

	// 行列合成(ＳＲＴ)
	m_mWorld = m_scaleMat * m_rotMat * camRotMat * m_transMat;
	
//	RayUpdateCollision();
	SphereUpdateCollision();
}

// 影描画関数
void Player::GenerateDepthMapFromLight()
{
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

// 板ポリ描画関数
void Player::DrawLit()
{
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

// 初期化関数
void Player::Init()
{
	// キャラ初期値
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Player/Player.gltf"));
	}

	m_moveSpd = 0.2f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerColl", GetPos(), 0.25f, KdCollider::TypeBump);
}

void Player::Shot()
{
	Math::Vector3	dir = Math::Vector3::Zero;
	float			range = 0.0f;
	m_wpCamera.lock()->WorkCamera()->GenerateRayInfoFromClientPos
	(
		{ 1280 / 2,720 / 2 },		// 2D座標
		m_wpCamera.lock()->GetPos(),// カメラの座標
		dir,						// 方向
		range						// レイの長さ
	);
}
