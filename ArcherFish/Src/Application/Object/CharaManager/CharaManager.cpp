#include "CharaManager.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"

void EnemyManager::Update()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		//NULL�łȂ��ꍇ
//		if (enemy[i] != NULL)
		{
//			enemy[i]->Update();
			//�G����ʊO�ɏo���ꍇ
//			if (enemy[i]->GetFlag() == false)
			{
				//�폜���Ă���NULL������
//				delete enemy[i];
//				enemy[i] = NULL;
			}
		}
	}
}

void EnemyManager::DrawLit()
{
}

void EnemyManager::Init()
{
	if (!m_spModelWork)
	{
		for (int i = 0; i < ENEMY_NUM; i++)
		{
//			enemy[i] = &std::make_shared<Enemy>();
		}
//		m_spModelWork = std::make_shared<KdModelWork>();
//		m_spModelWork->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Enemy/SkyEnemy.gltf"));
	}
}