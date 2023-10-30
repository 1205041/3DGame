#pragma once
#include "../../ObjBase.h"

class RiverSide :public ObjBase
{
public:
	RiverSide() { Init(); }
	~RiverSide() {};

	void PostUpdate()	override;
	void DrawUnLit()	override;
	void Init()			override;

private:
};