#pragma once
#include "../ObjBase.h"

class SphereGround :public ObjBase
{
public:
	SphereGround() { Init(); }
	~SphereGround() {};

	void Update()		override;
	void PostUpdate()	override;
	void DrawLit()		override;
	void Init()			override;

private:
	Math::Vector2 m_offset;
};
