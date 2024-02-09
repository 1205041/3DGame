#pragma once
#include "../ObjBase.h"

class Enemy;

#define ENEMY_NUM 10 // “G‘”

class EnemyManager :public ObjBase
{
public:
	EnemyManager() { Init(); }
	virtual ~EnemyManager() {}

	// XV‚ÉŠÖ‚·‚éŠÖ”
//	void PreUpdate()	override;
	void Update()		override;
//	void PostUpdate()	override;

	// •`‰æ‚ÉŠÖ‚·‚éŠÖ”
//	void PreDraw()						override;
	void DrawLit()						override;
//	void PostDraw()						override;

	void Init()	override;
private:
//	std::shared_ptr<Enemy>* enemy[ENEMY_NUM];
};