#pragma once
#include "../CamBase.h"

class TPSCam :public CamBase
{
public:
	TPSCam() { Init(); }
	~TPSCam() {}

	void Init()		override;
	void PostUpdate()	override;

	// 当たり判定用
	void RegistHitObject(const std::shared_ptr<KdGameObject>& object) { m_wpHitObject = object; }
private:
	void UpdateRotateByMouse();

	// 衝突判定に伴う座標更新
	void UpdateCollision();

	// 当たり判定用変数
	std::weak_ptr<KdGameObject> m_wpHitObject;
	Math::Vector3	hitPos = Math::Vector3::Zero;
};