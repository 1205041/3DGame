#pragma once
#include "../SceneBase/SceneBase.h"

class GameScene :public SceneBase
{
public:
	GameScene() { Init(); }
	virtual ~GameScene(){}

private:
	void Event()		override;
	void Init()			override;
	void ImGuiUpdate()	override;

	// �������V�[���֖ؑh�~
	bool m_SceneSwitcResult = false;
	bool m_SceneSwitcTitle = false;

	bool m_pushAct = false;
};