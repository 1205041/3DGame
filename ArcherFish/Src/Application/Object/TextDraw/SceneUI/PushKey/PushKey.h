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
	// �F�`�F���W
	float m_AlphC = 0.2f;
};