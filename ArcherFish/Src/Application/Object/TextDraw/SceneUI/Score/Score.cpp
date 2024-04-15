#include "Score.h"

void Score::Update()
{
}

void Score::Init()
{
}

void Score::DrawSprite()
{
	char buf[100];
	int num;
	//�X�R�A�{�[�h�̕����̕`��
	DrawGraph(SCORE_X, 10, g_board[0], TRUE);
	DrawGraph(SCORE_X, 70, g_board[1], TRUE);
	DrawGraph(SCORE_X, 140, g_board[2], TRUE);
	DrawGraph(SCORE_X, 170, g_board[3], TRUE);

	//�n�C�X�R�A�`��
	num = sprintf(buf, "%d", high_score);
	for (int i = 0; i < num; ++i) { DrawGraph(SCORE_X + 20 + i * 19, 35, g_number[(buf[i] - '0')], TRUE); }

	//�X�R�A�`��
	num = sprintf(buf, "%d", score);
	for (int i = 0; i < num; ++i) { DrawGraph(SCORE_X + 20 + i * 19, 95, g_number[(buf[i] - '0')], TRUE); }

	//�O���C�Y���`��
	num = sprintf(buf, "%d", graze);
	for (int i = 0; i < num; ++i) { DrawGraph(SCORE_X + 100 + i * 19, 145, g_number[(buf[i] - '0')], TRUE); }

	//���C�t���`��
	num = sprintf(buf, "%d", life);
	for (int i = 0; i < num; ++i) { DrawGraph(SCORE_X + 100 + i * 19, 175, g_number[(buf[i] - '0')], TRUE); }
}

void Score::SetScore(ScoreData _data, int _val)
{
	switch (_data)
	{
	case Hige_Score:
		high_score += _val;
		break;
	case Current_Score:
		score += _val;
		break;
	case Greaze_Score:
		graze += _val;
		break;
	case Life_Score:
		life += _val;
		break;
	}
}

int Score::GetScore(ScoreData _data)
{
	switch (_data)
	{
	case Hige_Score:
		return high_score;
		break;
	case Current_Score:
		return score;
		break;
	case Greaze_Score:
		return graze;
		break;
	case Life_Score:
		return life;
		break;
	}

	// �Y�����Ȃ��Ƃ���-1��Ԃ�
	return -1;
}
