#pragma once
#include "../SceneBase/SceneBase.h"

class TitleScene :public SceneBase
{
public:
	TitleScene() { Init(); }
	~TitleScene() {}

private:
	void Event()override;
	void Init()	override;

	const int babbleNum = 100;// –A‚Ì‘”
};