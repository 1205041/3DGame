#pragma once
#include "../SceneBase/SceneBase.h"

class GameScene :public SceneBase
{
public:
	GameScene() { Init(); }
	virtual ~GameScene(){}

private:
	void Event()		override;
	void Init()			override;

	/* フォグ(霧) */
	Math::Vector3 disColor	 = { 0.1f,0.1f,0.5f };	// 距離フォグの色
	float		  density	 = 0.015f;				// フォグ減衰率
	Math::Vector3 heigtColor = { 0.1f,0.5f,0.5f };	// 高さフォグの色
	float		  topVal	 = 5.0f;	// フォグを開始する上限の高さ
	float		  bottomVal  = -23.0f;	// フォグ色に染まる下限の高さ
	float		  distance	 = 5.0f;	// フォグの開始する距離
};