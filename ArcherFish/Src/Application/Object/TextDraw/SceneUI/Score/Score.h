#pragma once
#include "../../TextDraw.h"

enum ScoreData
{
	High_Score,
	Current_Score,
	Graze_Score,
	Life_Score
};

class Score :public TextDraw
{
public:
	Score() { Init(); }
	virtual ~Score() {}

	void Update()		override;
	void Init()			override;
	void DrawSprite()	override;

	void SetScore(ScoreData _data, int _val);
	int GetScore(ScoreData _data);
	
private:
	KdTexture	m_txt;

	// グラフィックハンドル
	int g_number[10];
	
	int m_high = 0;
	int m_score = 0;
	int m_graze = 0;
	int m_life = 0;
};