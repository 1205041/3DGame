#pragma once
#include "../../TextDraw.h"

enum ScoreData
{
	Hige_Score,
	Current_Score,
	Greaze_Score,
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

private:
	// グラフィックハンドル
};