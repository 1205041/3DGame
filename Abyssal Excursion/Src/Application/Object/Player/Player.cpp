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

	m_gravity += 0.01f;
	m_mWorld._42 -= m_gravity;

	// キャラクターの移動速度
	m_nowPos  = GetPos();

	m_moveVec = Math::Vector3::Zero;
	if (GetAsyncKeyState('D') & 0x8000) { m_moveVec.x = 1.0f; }
	if (GetAsyncKeyState('A') & 0x8000) { m_moveVec.x = -1.0f; }
	if (GetAsyncKeyState('W') & 0x8000) { m_moveVec.z = 1.0f; }
	if (GetAsyncKeyState('S') & 0x8000) { m_moveVec.z = -1.0f; }
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!m_push)
		{
			m_moveSpd = 0.1f;
			m_push = true;
		}
	}
	else
	{
		m_moveSpd = 0.05f;
		m_push = false;
	}

	// カメラ情報
	std::shared_ptr<CameraBase> spCamera = m_wpCamera.lock();
	if (spCamera)
	{
		m_moveVec = m_moveVec.TransformNormal
		(m_moveVec, spCamera->GetRotationYMatrix());
	}

	m_moveVec.Normalize();
	m_moveVec *= m_moveSpd;

	m_nowPos += m_moveVec;
}

// 更新後更新関数
void Player::PostUpdate()
{
	// キャラの座標行列
	m_mWorld = Math::Matrix::CreateTranslation(m_nowPos);
	
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

	SetScale({ 0.1f,0.1f,0.1f });

	m_moveSpd = 0.05f;
	m_push = false;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerCollider", GetPos(), 0.25f, KdCollider::TypeBump);
}
