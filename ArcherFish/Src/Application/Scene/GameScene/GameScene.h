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
	Math::Vector3 m_disColor	= { 0.1f,0.1f,0.5f };	// 距離フォグの色
	float		  m_density		= 0.02f;				// フォグ減衰率
	Math::Vector3 m_heigtColor	= { 0.1f,0.5f,0.5f };	// 高さフォグの色
	float		  m_topVal		= 3.0f;	// フォグを開始する上限の高さ
	float		  m_bottomVal	= 0.0f;	// フォグ色に染まる下限の高さ
	float		  m_distance	= 7.0f;	// フォグの開始する距離
};