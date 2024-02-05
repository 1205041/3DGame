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

	/* �t�H�O(��) */
	Math::Vector3 m_disColor	= { 0.1f,0.1f,0.5f };	// �����t�H�O�̐F
	float		  m_density		= 0.02f;				// �t�H�O������
	Math::Vector3 m_heigtColor	= { 0.1f,0.5f,0.5f };	// �����t�H�O�̐F
	float		  m_topVal		= 10.0f;	// �t�H�O���J�n�������̍���
	float		  m_bottomVal	= -20.0f;	// �t�H�O�F�ɐ��܂鉺���̍���
	float		  m_distance	= 6.0f;		// �t�H�O�̊J�n���鋗��
};