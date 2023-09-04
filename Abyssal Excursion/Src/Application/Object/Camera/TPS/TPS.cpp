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
