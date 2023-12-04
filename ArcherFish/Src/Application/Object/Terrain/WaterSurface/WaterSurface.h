#pragma once
#include "../../ObjBase.h"

class WaterSurface :public ObjBase
{
public:
	WaterSurface() { Init(); }
	~WaterSurface() {}

	void Update()		override;
	void PostUpdate()	override;
	void DrawLit()		override;
	void Init()			override;
private:
	std::shared_ptr<KdSquarePolygon> m_spPolygon = nullptr;

	Math::Vector2 m_offset;
};