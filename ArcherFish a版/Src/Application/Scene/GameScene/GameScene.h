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
	Math::Vector3 m_disColor	= { 0.6f,0.7f,0.8f };	// 距離フォグの色
	float		  m_density		= 0.01f;				// フォグ減衰率
	Math::Vector3 m_heigtColor	= { 0.1f,0.6f,0.8f };	// 高さフォグの色
	float		  m_topVal		= 1.3f;	// フォグを開始する上限の高さ
	float		  m_bottomVal	= 1.0f;	// フォグ色に染まる下限の高さ
	float		  m_distance	= 6.0f;	// フォグの開始する距離
};