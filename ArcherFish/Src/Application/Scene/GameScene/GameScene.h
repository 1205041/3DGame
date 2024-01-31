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
	Math::Vector3 disColor	 = { 0.1f,0.1f,0.5f };	// �����t�H�O�̐F
	float		  density	 = 0.015f;				// �t�H�O������
	Math::Vector3 heigtColor = { 0.1f,0.5f,0.5f };	// �����t�H�O�̐F
	float		  topVal	 = 5.0f;	// �t�H�O���J�n�������̍���
	float		  bottomVal  = -23.0f;	// �t�H�O�F�ɐ��܂鉺���̍���
	float		  distance	 = 5.0f;	// �t�H�O�̊J�n���鋗��
};