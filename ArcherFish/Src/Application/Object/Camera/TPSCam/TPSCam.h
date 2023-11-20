#pragma once
#include "../CamBase.h"

class TPSCam :public CamBase
{
public:
	TPSCam() { Init(); }
	~TPSCam() {}

	void Init()		override;
	void PostUpdate()	override;

	// �����蔻��p
	void RegistHitObject(const std::shared_ptr<KdGameObject>& object) { m_wpHitObject = object; }
private:
	void UpdateRotateByMouse();

	// �Փ˔���ɔ������W�X�V
	void UpdateCollision();

	// �����蔻��p�ϐ�
	std::weak_ptr<KdGameObject> m_wpHitObject;
	Math::Vector3	hitPos = Math::Vector3::Zero;
};