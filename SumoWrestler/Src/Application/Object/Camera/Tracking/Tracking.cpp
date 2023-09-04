#include "Tracking.h"

void Tracking::Init()
{
	// ��_(�^�[�Q�b�g)����ǂꂾ������Ă��邩
	transMat = Math::Matrix::CreateTranslation(0, 0.5f, -1.0f);
	
	// �ǂꂾ���X���Ă��邩/* ���Ԃ�Y,X,Z */
	rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(25));
	
	m_mWorld = rotMat * transMat;

	CameraBase::Init();
}

void Tracking::Update()
{
	// �^�[�Q�b�g�̍s��(�L���ȏꍇ���p����)
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
		if (spTarget)
		{
			targetMat = spTarget->GetMatrix();
		}
	}

	m_mWorld = rotMat * transMat * targetMat;

	CameraBase::Update();
}