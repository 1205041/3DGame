#pragma once
#include "../ObjBase.h"

class Ground :public ObjBase
{
public:
	Ground() { Init(); }
	~Ground() {}

	void GenerateDepthMapFromLight()override;
	void DrawLit()					override;

	void Init()						override;
private:
	
};