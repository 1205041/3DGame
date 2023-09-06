#include "Player.h"

#include "../Camera/TPS/TPS.h"

// 更新関数
void Player::Update()
{

	if (GetPos().y < -10) { m_isExpired = true; }
	if (m_isExpired)
	{
		KdAudioManager::Instance().StopAllSound();
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Lose);
	}

	// カメラ情報
	std::shared_ptr<CameraBase> spCamera = m_wpCamera.lock();
	if (spCamera) { camRotMat = spCamera->GetRotationYMatrix(); }

	// キャラクターの移動速度
	m_moveVec = Math::Vector3::Zero;

//	if (GetAsyncKeyState('W') & 0x8000)
	{
		// 前方
		Math::Vector3 v = Math::Vector3::TransformNormal
		(Math::Vector3::Backward, camRotMat);

		m_moveVec += v;
	}
	
	m_moveVec.Normalize();
	
	m_nowPos += m_moveVec * m_moveSpd;
}

// 更新後更新関数
void Player::PostUpdate()
{
	// キャラの座標行列
	m_mWorld = camRotMat * Math::Matrix::CreateTranslation(m_nowPos);
	
	UpdateCollision();
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

	m_moveSpd = 0.05f;
	m_push = false;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerCollider", GetPos(), 0.25f, KdCollider::TypeBump);
}
