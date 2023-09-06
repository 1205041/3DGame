#pragma once
#include "../CameraBase.h"

class TPS :public CameraBase
{
public:
	TPS() { Init(); }
	~TPS() {}

	void Init()		override;
	void Update()	override;

	void RegistHitObject(const std::shared_ptr<KdGameObject>& object) { m_wpHitObject = object; }

private:
	void UpdateRotateByMouse();

	// Õ“Ë”»’è‚É”º‚¤À•WXV
	void UpdateCollision();

	POINT m_FixMousePos;

	std::weak_ptr<KdGameObject> m_wpHitObject;
};