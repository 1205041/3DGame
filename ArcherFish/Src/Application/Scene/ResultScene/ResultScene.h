#pragma once
#include "../SceneBase/SceneBase.h"

class ResultScene :public SceneBase
{
public:
	ResultScene() { Init(); }
	virtual ~ResultScene() {}

private:
	void Event()		override;
	void Init()			override;
	void ImGuiUpdate()	override;

};