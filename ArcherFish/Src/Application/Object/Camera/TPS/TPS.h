#pragma once
#include "../CameraBase.h"

class TPS :public CameraBase
{
public:
	TPS() { Init(); }
	~TPS() {}

	void Init()			override;
	void PostUpdate()	override;
	void RegistHitObject(const std::shared_ptr<KdGameObject>& object) { m_wpHitObject = object; }

private:
	void UpdateRotateByMouse();

	// 衝突判定に伴う座標更新
	void UpdateCollision();

	POINT m_FixMousePos;

	// 当たり判定用変数
	std::weak_ptr<KdGameObject> m_wpHitObject;
	Math::Vector3	hitPos = Math::Vector3::Zero;
};