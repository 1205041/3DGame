#pragma once
#include "../../ObjBase.h"

class GroundDome :public ObjBase
{
public:
	GroundDome() { Init(); }
	~GroundDome() {};

	void PostUpdate()	override;
	void DrawUnLit()	override;
	void Init()			override;

private:
};
