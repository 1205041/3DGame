#pragma once
#include "../SceneBase/SceneBase.h"

class TitleScene :public SceneBase
{
public:
	TitleScene() { Init(); }
	virtual ~TitleScene() {}

private:
	void Event()override;
	void Init()	override;

	// �������V�[���֖ؑh�~
	bool m_SceneSwitcGame = false;

	bool m_pushAct = false;
};