#pragma once
#include "../ObjBase.h"

class SkySphere :public ObjBase
{
public:
	SkySphere() { Init(); }
	~SkySphere() {}

	void PostUpdate()				override;
	void DrawLit()					override;
	void Init()						override;
private:

};