#include "Score.h"

void Score::Update()
{
}

void Score::Init()
{
	m_txt.Load("Asset/Textures/SceneUI/Score/board.png");
	m_tex.Load("Asset/Textures/SceneUI/Score/number.png");

	m_pixel = { SCORE_X,0 };
	m_rectAng = { 0,0,200,230 };
	m_color = kWhiteColor;
	m_pivot = { 0.5f, 0.5f };
}

void Score::DrawSprite()
{
	char buf[100];
	int num = 0;
	//�X�R�A�{�[�h�̕����̕`��
	KdShaderManager::GetInstance().m_spriteShader.DrawTex(&m_txt, (int)m_pixel.x, (int)m_pixel.y, &m_rectAng, &m_color, m_pivot);
	
	//�X�R�A�`��
//	num = sprintf(buf, "%d", m_score);
//	for (int i = 0; i < num; ++i) 
//	{ 
//		DrawGraph(SCORE_X + 20 + i * 19, 95, g_number[(buf[i] - '0')], TRUE); 
//	}

//	KdShaderManager::GetInstance().m_spriteShader.DrawTex(&m_tex, (int)m_pixel.x, (int)m_pixel.y, m_waterMax, m_height, &m_rectAng, &m_barColor, m_pivot);

}

void Score::SetScore(ScoreData _data, int _val)
{
	switch (_data)
	{
	case High_Score:
		m_high += _val;
		break;
	case Current_Score:
		m_score += _val;
		break;
	case Graze_Score:
		m_graze += _val;
		break;
	case Life_Score:
		m_life += _val;
		break;
	}
}

int Score::GetScore(ScoreData _data)
{
	switch (_data)
	{
	case High_Score:
		return m_high;
		break;
	case Current_Score:
		return m_score;
		break;
	case Graze_Score:
		return m_graze;
		break;
	case Life_Score:
		return m_life;
		break;
	}

	// �Y�����Ȃ��Ƃ���-1��Ԃ�
	return -1;
}
