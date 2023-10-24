#pragma once
#include "../../ObjBase.h"

class TropicalDome :public ObjBase
{
public:
	TropicalDome() { Init(); }
	~TropicalDome() {};

	void PostUpdate()	override;
	void DrawUnLit()	override;
	void Init()			override;

private:
};
