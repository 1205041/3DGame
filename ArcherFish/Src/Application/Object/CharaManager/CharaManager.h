#pragma once
#include "../ObjBase.h"

class Enemy;

#define ENEMY_NUM 10 // 敵総数

class EnemyManager :public ObjBase
{
public:
	EnemyManager() { Init(); }
	virtual ~EnemyManager() {}

	// 更新に関する関数
//	void PreUpdate()	override;
	void Update()		override;
//	void PostUpdate()	override;

	// 描画に関する関数
//	void PreDraw()						override;
	void DrawLit()						override;
//	void PostDraw()						override;

	void Init()	override;
private:
//	std::shared_ptr<Enemy>* enemy[ENEMY_NUM];
};