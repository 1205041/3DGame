#include "Player.h"

#include "../Camera/TPS/TPS.h"

// 更新関数
void Player::Update()
{
	// カメラ情報
	std::shared_ptr<CameraBase> spCamera = m_wpCamera.lock();
	if (spCamera) { camRotMat = spCamera->GetRotationYMatrix(); }

	// キャラクターの移動速度
	m_nowPos = GetPos();
}

// 更新後更新関数
void Player::PostUpdate()
{
	m_transMat = Math::Matrix::CreateTranslation(m_nowPos);

	// キャラの座標行列
	m_mWorld = camRotMat * m_transMat;
	
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
