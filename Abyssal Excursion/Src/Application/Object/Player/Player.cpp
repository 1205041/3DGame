#include "Player.h"

void Player::Init()
{
	if (!m_spModel) 
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Player/AirPlane/AirPlane.gltf"));
	}
	
	SetScale({ 1.0f,1.0f,1.0f });
	SetPos({ 0,2.0f,0 });
}

void Player::Update()
{
	
}

void Player::DrawLit()
{
	if (!m_spModel) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}
