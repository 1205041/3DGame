#pragma once
#include "../../TextDraw.h"

class Enter:public TextDraw
{
public:
	Enter() { Init(); }
	~Enter() {}

	void Update()		override;
	void PostUpdate()	override;
	void Init()			override;
private:
	// 色チェンジ
	float m_AlphC = 0.1f;
};