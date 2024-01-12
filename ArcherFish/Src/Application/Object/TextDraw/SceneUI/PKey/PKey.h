#pragma once
#include "../../TextDraw.h"

class PKey :public TextDraw
{
public:
	PKey() { Init(); }
	virtual ~PKey() {}

	void Update()		override;
	void PostUpdate()	override;
	void Init()			override;

private:
	// 色チェンジ
	float m_AlphC = 0.2f;
};