#pragma once
#include "../../../ObjBase.h"

class UnderWater :public ObjBase
{
public:
	UnderWater() { Init(); }
	virtual ~UnderWater() {};

	void Update()		override;
	void PostUpdate()	override;
	void DrawLit()		override;
	void Init()			override;

private:
	Math::Vector2 m_offset;
};
