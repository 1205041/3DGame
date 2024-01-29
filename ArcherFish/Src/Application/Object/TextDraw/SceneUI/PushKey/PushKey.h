#pragma once
#include "../../TextDraw.h"

class PushKey :public TextDraw
{
public:
	PushKey() { Init(); }
	virtual ~PushKey() {}

	void Update()		override;
	void PostUpdate()	override;

private:
	// 色チェンジ
	float m_AlphC = 0.2f;
};