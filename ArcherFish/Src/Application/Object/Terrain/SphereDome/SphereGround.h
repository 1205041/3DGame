#pragma once
#include "../../ObjBase.h"

class SphereGround :public ObjBase
{
public:
	SphereGround() { Init(); }
	virtual ~SphereGround() {};

	void PostUpdate()	override;
	void DrawUnLit()	override;
	void Init()			override;

private:

};
