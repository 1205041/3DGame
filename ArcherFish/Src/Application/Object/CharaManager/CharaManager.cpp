#include "CharaManager.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"

void EnemyManager::Update()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		//NULLでない場合
//		if (enemy[i] != NULL)
		{
//			enemy[i]->Update();
			//敵が画面外に出た場合
//			if (enemy[i]->GetFlag() == false)
			{
				//削除してからNULLを入れる
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