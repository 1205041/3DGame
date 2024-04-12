#pragma once
#include "../../ObjBase.h"

class WaterSurface :public ObjBase
{
public:
	WaterSurface() { Init(); }
	virtual ~WaterSurface() {}

	void Update()		override;
	void PostUpdate()	override;
	void DrawLit()		override;
	void Init()			override;
private:
	Math::Vector2 m_offset;
};