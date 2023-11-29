#include "TPSCam.h"

#include "../../Enemy/Enemy.h"

void TPSCam::Init()
{
	// ��_(�^�[�Q�b�g)�̖ڐ��̈ʒu
	m_transMat = Math::Matrix::CreateTranslation(Math::Vector3(0.8f, 1.5f, -3.5f));

	// �J�[�\��������
	KdCSVData windowData("Asset/Data/WindowSettings.csv");
	const std::vector<std::string>& sizeData = windowData.GetLine(0);

	m_FixMousePos.x = atoi(sizeData[0].c_str()) / 2;
	m_FixMousePos.y = atoi(sizeData[1].c_str()) / 2;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	CamBase::Init();
}

void TPSCam::Update()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { ShotRayUpdateCollision(); }
}

void TPSCam::PostUpdate()
{
	// �^�[�Q�b�g�̍s��
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		const std::shared_ptr<KdGameObject>& spTarget = m_wpTarget.lock();
		if (spTarget) { targetMat = Math::Matrix::CreateTranslation(spTarget->GetPos()); }
	}

	// �s�񍇐�
	UpdateRotateByMouse();
	m_rotMat = GetRotCameraMatrix();

	m_mWorld = m_transMat * m_rotMat * targetMat;

	CamBase::PostUpdate();
}

void TPSCam::UpdateRotateByMouse()
{
	// �}�E�X�ʒu�̍����𓾂�
	POINT nowPos{};
	GetCursorPos(&nowPos);

	POINT mouseMove{};
	mouseMove.x = nowPos.x - m_FixMousePos.x;
	mouseMove.y = nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// �J��������]������ׂɊe���̉�]�p�x��ݒ肷��
	m_degAng.x += mouseMove.y * 0.15f;
	m_degAng.y += mouseMove.x * 0.15f;

	// ��]����
	m_degAng.x = std::clamp(m_degAng.x, -FLT_MAX, FLT_MAX);
}

/* ==================== */
/* �e���˃��C�����蔻�� */
/* ==================== */
void TPSCam::ShotRayUpdateCollision()
{
	// �@�����蔻��(���C����)�p�̏����쐬
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = GetPos();	// ���C�̔��ˈʒu��ݒ�

	// �}�E�X�ʒu�̍����𓾂�
	POINT nowPos{};
	GetCursorPos(&nowPos);

	// �J�������
	GetCamera()->GenerateRayInfoFromClientPos(
		nowPos,			// 2D���W
		rayInfo.m_pos,	// ���W
		rayInfo.m_dir,	// ����
		rayInfo.m_range // ���C�̒���
	);
	
	rayInfo.m_type = KdCollider::TypeBump;	// �����蔻����������^�C�v��ݒ�

	/* === �f�o�b�N�p === */
//	m_debugWire.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

	// �AHIT����ΏۃI�u�W�F�N�g�ɑ�������
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retRayList;
				spGameObj->Intersects(rayInfo, &retRayList);

				// �B���ʂ��g���č��W��⊮����
				// ���C�ɓ����������X�g�����ԋ߂��I�u�W�F�N�g�����o
				float maxOverLap = 0.0f;
				bool  hit = false;
				for (auto& ret : retRayList)
				{
					// ���C���Ւf���I�[�o�[������������Ԓ������̂�T��
					if (maxOverLap < ret.m_overlapDistance) { hit = true; }
				}
				if (hit) // ��������ɓ������Ă���
				{
					std::shared_ptr<Enemy> spEnemy = m_wpEnemy.lock();
					if (spEnemy) { spEnemy->SetActFlg(false); }
				}
			}
		}
	}
}
