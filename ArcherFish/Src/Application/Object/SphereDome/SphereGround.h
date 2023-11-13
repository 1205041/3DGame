#pragma once
#include "../ObjBase.h"

class SphereGround :public ObjBase
{
public:
	SphereGround() { Init(); }
	~SphereGround() {};

	void PostUpdate()	override;
	void DrawUnLit()	override;
	void Init()			override;

private:
	void SkySphereCollision();
};
