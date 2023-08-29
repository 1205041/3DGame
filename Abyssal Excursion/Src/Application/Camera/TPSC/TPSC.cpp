#include "TPSC.h"

void TPSC::Init()
{
	// ��_(�^�[�Q�b�g)�̖ڐ��̈ʒu
	m_LocalPos = Math::Matrix::CreateTranslation(0.0f, 2.0f, -3.0f);

	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

//	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	CameraBase::Init();
}

void TPSC::Update()
{
	// �^�[�Q�b�g�̍s��(�L���ȏꍇ���p����)
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		const std::shared_ptr<KdGameObject> spTarget = m_wpTarget.lock();
		if (spTarget)
		{
			targetMat = Math::Matrix::CreateTranslation(spTarget->GetPos());
		}
	}

	// �J�����̉�]
	UpdateRotateByMouse();
	m_Rotation = GetRotationMatrix();
	m_mWorld = m_LocalPos * m_Rotation * targetMat;

	CameraBase::Update();
}

void TPSC::UpdateRotateByMouse()
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
	m_DegAng.x += mouseMove.y * 0.15f;
	m_DegAng.y += mouseMove.x * 0.15f;
	*/
	
	if (GetAsyncKeyState(VK_UP) & 0x8000) { m_DegAng.x += 1.0f; }
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { m_DegAng.x -= 1.0f; }
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) { m_DegAng.y += 1.0f; }
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { m_DegAng.y -= 1.0f; }

	// ��]����
	m_DegAng.x = std::clamp(m_DegAng.x, -FLT_MAX, FLT_MAX);
}