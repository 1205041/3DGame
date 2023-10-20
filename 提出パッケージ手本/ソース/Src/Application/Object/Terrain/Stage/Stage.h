#pragma once
#include "../../ObjBase.h"

class Stage :public ObjBase
{
public:
	Stage() { Init(); }
	~Stage() {}

	void PostUpdate()	override;
	void DrawLit()		override;
	void Init()			override;

private:

};