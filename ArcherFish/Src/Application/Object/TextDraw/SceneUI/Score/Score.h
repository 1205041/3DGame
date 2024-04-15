#pragma once
#include "../../TextDraw.h"

//�X�R�A�̕����`��̊�ƂȂ�X���W
#define SCORE_X 390

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

	void SetScore(ScoreData _data, int _val);
	int GetScore(ScoreData _data);
	void All();

private:
	void Draw();

	// �O���t�B�b�N�n���h��
	int g_board[4], g_number[10];
	
	int high_score, score, graze, life;
};