#include "CharaManager.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"

void EnemyManager::Update()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		//NULL‚Å‚È‚¢ê‡
//		if (enemy[i] != NULL)
		{
//			enemy[i]->Update();
			//“G‚ª‰æ–ÊŠO‚Éo‚½ê‡
//			if (enemy[i]->GetFlag() == false)
			{
				//íœ‚µ‚Ä‚©‚çNULL‚ğ“ü‚ê‚é
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