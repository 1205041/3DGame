#include "Enemy.h"
#include "../Player/Player.h"

// �X�V�֐�
void Enemy::Update()
{
	// �����؂��L���ɂ���
	if (GetPos().y < -10) { m_isExpired = true; }
	if(m_isExpired)
	{
		KdAudioManager::Instance().StopAllSound();
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Win);
	}

	m_gravity += 0.01f;
	m_mWorld._42 -= m_gravity;

	// �Ǐ]�ړ�
	m_nowPos = GetPos();
	m_moveVec = Math::Vector3::Zero;

	if (!m_isExpired)
	{
		std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
		if (spPlayer)
		{
			float targetDeg = GetAngleDeg(m_nowPos, spPlayer->GetPos());

			float diffDeg = targetDeg - m_deg;

			//0�`359���ɕ␳����
			if (diffDeg < 0) { diffDeg += 360; }

			//������������߂�
			if (diffDeg <= 179)
			{
				//�p�x����179�ȉ��͍�����(�����v���)
				m_deg += m_turnDeg;

				//0�`359���ɕ␳����(360�ȏ�ɂȂ��Ă��Ȃ����H)
				if (m_deg >= 360) { m_deg -= 360; }
			}
			else
			{
				//�p�x����180�ȏ�͉E����(���v���)
				m_deg -= m_turnDeg;

				//0�`359���ɕ␳����(0�����ɂȂ��Ă��Ȃ����H)
				if (m_deg < 0) { m_deg += 360; }
			}

			//�ړ��ʂ��ēx���߂�
			m_moveVec.x = cos(DirectX::XMConvertToRadians(m_deg)) * m_moveSpd;
			m_moveVec.z = sin(DirectX::XMConvertToRadians(m_deg)) * m_moveSpd;
		}
	}
	
	m_moveVec.Normalize();
	m_moveVec *= m_moveSpd;

	m_nowPos += m_moveVec;

	m_spPoly->SetUVRect(Walk[(int)m_anime]);
	m_anime += 0.05f;
	if (m_anime >= 4) { m_anime = 0; }
}

// �X�V��X�V�֐�
void Enemy::PostUpdate()
{
	// �L�����̍��W�s��
	m_mWorld = Math::Matrix::CreateTranslation(m_nowPos);
	
	UpdateCollision();
}

// �e�`��֐�
void Enemy::GenerateDepthMapFromLight()
{
	if (!m_spPoly) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

// �|���`��֐�
void Enemy::DrawLit()
{
	if (!m_spPoly) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

// �������֐�
void Enemy::Init()
{
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/Enemy/enemy.png"));

		// �A�j���[�V����
		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
		m_spPoly->SetSplit(4, 4);
	}
	
	m_moveSpd = 0.03f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("EnemyCollider", GetPos(), 0.25f, KdCollider::TypeBump);
}
