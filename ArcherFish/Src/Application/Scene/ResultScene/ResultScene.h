#pragma once
#include "../SceneBase/SceneBase.h"

class ResultScene :public SceneBase
{
public:
	ResultScene() { Init(); }
	virtual ~ResultScene() {}

private:
	void Event()override;
	void Init()	override;

	// ’·‰Ÿ‚µƒV[ƒ“Ø‘Ö–h~
	bool m_SceneSwitcTitle = false;
};