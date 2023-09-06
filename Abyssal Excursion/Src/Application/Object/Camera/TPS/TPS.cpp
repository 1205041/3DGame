#include "TPS.h"

void TPS::Init()
{
	// ��_(�^�[�Q�b�g)�̖ڐ��̈ʒu
	transMat = Math::Matrix::CreateTranslation(Math::Vector3(0.0f, 1.0f, -5.0f));

	// �J�[�\��������
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

//	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	CameraBase::Init();
}

void TPS::Update()
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
	rotMat = GetRotationMatrix();

	m_mWorld = transMat * rotMat * targetMat;

	UpdateCollision();

	CameraBase::Update();
}

void TPS::UpdateRotateByMouse()
{
	/*
	// �}�E�X�ʒu�̍����𓾂�
	POINT nowPos;
	GetCursorPos(&nowPos);

	POINT mouseMove;
	mouseMove.x = nowPos.x - m_FixMousePos.x;
	mouseMove.y = nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// �J��������]������ׂɊe���̉�]�p�x��ݒ肷��
	m_degAng.x += mouseMove.y * 0.15f;
	m_degAng.y += mouseMove.x * 0.15f;
	*/

	if (GetAsyncKeyState(VK_UP) & 0x8000) { m_degAng.x += 1.0f; }
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { m_degAng.x -= 1.0f; }
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) { m_degAng.y += 1.0f; }
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { m_degAng.y -= 1.0f; }

	// ��]����
	m_degAng.x = std::clamp(m_degAng.x, -FLT_MAX, FLT_MAX);
}

void TPS::UpdateCollision()
{
	const std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
	if (!spTarget) { return; }

	// �@���C����̏��쐬
	KdCollider::RayInfo rayInfo;

	// ���C�̔��ˈʒu��ݒ�
	rayInfo.m_pos = spTarget->GetPos();

	// �i���̋��e�͈͂�ݒ�
	rayInfo.m_pos.y += 2.0f;

	// ���C�̔��˕�����ݒ�
	rayInfo.m_dir = GetPos() - rayInfo.m_pos;
	rayInfo.m_dir.Normalize();/* �������݂̂𒊏o */

	// ���C�̒���
	rayInfo.m_range = (GetPos() - rayInfo.m_pos).Length();

	// �����蔻����������^�C�v��ݒ�
	rayInfo.m_type = KdCollider::TypeGround;

	// �AHIT����ΏۃI�u�W�F�N�g�ɑ�������
	std::shared_ptr<KdGameObject> spGameObj = m_wpHitObject.lock();
	if (spGameObj)
	{
		std::list<KdCollider::CollisionResult> retRayList;
		spGameObj->Intersects(rayInfo, &retRayList);

		// �B���ʂ��g���č��W��⊮����
		// ���C�ɓ����������X�g�����ԋ߂��I�u�W�F�N�g�����o
		float maxOverLap = 0;
		Math::Vector3 hitPos = Math::Vector3::Zero;
		bool hit = false;
		for (auto& ret : retRayList)
		{
			// ���C���Ւf���A�I�[�o�[������������Ԓ������̂�T��
			if (maxOverLap < ret.m_overlapDistance)
			{
				maxOverLap = ret.m_overlapDistance;
				hitPos = ret.m_hitPos;
				hit = true;
			}
		}
		// ��������
		if (hit) { SetPos(hitPos); }
	}
}