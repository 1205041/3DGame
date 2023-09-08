#include "TitleScene.h"
#include "../../Object/ObjBase.h"

void TitleScene::Event()
{
	// �V�[���ؑ�(Title��Game)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
//		KdAudioManager::Instance().Play("Asset/Sounds/SE/PushButton.wav");
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
//		KdAudioManager::Instance().StopAllSound();
	}
}

void TitleScene::Init()
{
//	KdAudioManager::Instance().Play("Asset/Sounds/BGM/Title.wav", true);
	
	/* �I�u�W�F�N�g�̏����� */
	// �V�[��
	std::shared_ptr<ObjBase> spObjTitle;
	spObjTitle = std::make_shared<ObjBase>();
	spObjTitle->SetTexture("Asset/Textures/Title/Title.png");
	m_objList.push_back(spObjTitle);
}
