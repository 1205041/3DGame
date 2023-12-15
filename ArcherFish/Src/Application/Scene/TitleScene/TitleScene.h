#pragma once
#include "../SceneBase/SceneBase.h"

class TitleScene :public SceneBase
{
public:
	TitleScene() { Init(); }
	virtual ~TitleScene() {}

private:
	void Event()		override;
	void Init()			override;
	void ImGuiUpdate()	override;

};