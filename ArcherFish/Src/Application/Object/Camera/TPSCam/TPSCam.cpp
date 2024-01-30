#include <Application/main.h>
#include "TPSCam.h"

void TPSCam::Init()
{
	// ��_(�^�[�Q�b�g)�̖ڐ��̈ʒu
	m_transMat = Math::Matrix::CreateTranslation(Math::Vector3(0.9f, 1.3f, -3.5f));

	// �J�[�\��������
	m_FixMousePos.x = 1280 / 2;
	m_FixMousePos.y = 720 / 2;
	
	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
	
	CamBase::Init();
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
