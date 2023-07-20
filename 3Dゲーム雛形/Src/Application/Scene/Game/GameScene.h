#pragma once
#include "../SceneBase/SceneBase.h"

class GameScene :public SceneBase
{
public:
	GameScene() { Init(); }
	~GameScene() {}

private:
	void Event()		override;
	void Init()			override;
	void ImGuiUpdate()	override;

	bool ImGuiFlg = false;
	bool switchFlg = false;
};