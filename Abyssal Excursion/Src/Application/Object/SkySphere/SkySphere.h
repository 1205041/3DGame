#pragma once
#include "../ObjBase.h"

class SkySphere :public ObjBase
{
public:
	SkySphere() { Init(); }
	~SkySphere() {};

	void Update()		override;
	void PostUpdate()	override;
	void DrawLit()		override;
	void DrawBright()	override;
	void Init()			override;

private:
	float m_cnt = 0.0f;
};
