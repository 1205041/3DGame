#include "TitleScene.h"

void TitleScene::Event()
{
	// ƒV[ƒ“Ø‘Ö(Title¨Game)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(SceneManager::SceneType::Game);
	}
}

void TitleScene::Init()
{
}
